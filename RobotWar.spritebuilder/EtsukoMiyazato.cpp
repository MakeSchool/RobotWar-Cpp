//
//  SimpleRobotCpp.cpp
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#include "EtsukoMiyazato.hpp"
#include <math.h>
#include <stdio.h>

EtsukoMiyazato::EtsukoMiyazato()
{
    this->currentState = EtsukoMiyazatoAction::DEFAULT;
    this->actionIndex = 0;
}

void EtsukoMiyazato::run()
{
    this->actionIndex = 0;
    
    while (true)
    {
        while (this->currentState == EtsukoMiyazatoAction::DEFAULT)
        {
            this->performNextDefaultAction();
        }
        
        while (this->currentState == EtsukoMiyazatoAction::TURN_AROUND)
        {
            this->performNextTurnAroundAction();
        }
        
        while (this->currentState == EtsukoMiyazatoAction::FIRING)
        {
            this->performNextFiringAction();
        }
        
        while (this->currentState == EtsukoMiyazatoAction::SEARCHING)
        {
            this->performNextSearchingAction();
        }
    }
}

/*
 アクション
 */

//デフォルト
void EtsukoMiyazato::performNextDefaultAction()
{
    switch (this->actionIndex % 4)
    {
        case 0: this->shoot(); break;
        case 1: this->turnRobotLeft(20); break;
        case 2: this->shoot(); break;
        case 3: this->turnRobotRight(40); break;
    }
    
    this->actionIndex++;
}

//向きを変える
void EtsukoMiyazato::performNextTurnAroundAction()
{
    //0より大きかったら(右側があたったら)
    if (this->lastHitWallAngle >= 0)
    {
        this->turnRobotLeft(fabs(this->lastHitWallAngle));
    }
    //0より小さかったら(左側があたったら)
    else
    {
        this->turnRobotRight(fabs(this->lastHitWallAngle));
    }
    
    //前に20進む
    this->moveAhead(20);
    //前の状態に戻る
    this->setCurrentState(previousState);
}

//撃つアクション
void EtsukoMiyazato::performNextFiringAction()
{
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f)
    {
        this->setCurrentState(EtsukoMiyazatoAction::SEARCHING);
    }
    else
    {
        float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
        
        if (angle >= 0)
        {
            this->turnGunRight(fabsf(angle));
        }
        else
        {
            this->turnGunLeft(fabsf(angle));
        }
        
        this->shoot();
    }
}

//サーチ
void EtsukoMiyazato::performNextSearchingAction()
{
    switch (this->actionIndex % 4)
    {
        case 0: this->moveAhead(50); break;
        case 1: this->turnRobotLeft(20); break;
        case 2: this->moveAhead(50); break;
        case 3: this->turnRobotRight(20); break;
    }
    
    this->actionIndex++;
}

/*
 コールバック
 */
//的の玉があたった時
void EtsukoMiyazato::gotHit()
{
    //行動をキャンセルする
    this->cancelActiveAction();
}

//敵をスキャンしたとき(相手のposition)
void EtsukoMiyazato::scannedRobotAtPosition(RWVec position)
{
    if (this->currentState != EtsukoMiyazatoAction::FIRING)
    {
        //行動をキャンセルする
        this->cancelActiveAction();
    }
    
    //相手のポジション
    this->lastKnownPosition = position;
    //相手のポジションを取得した時間
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    //状態をFIRINGにセット
    this->setCurrentState(EtsukoMiyazatoAction::FIRING);
}

//壁にぶつかったとき(当たったサイド、当たった角度)
void EtsukoMiyazato::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    if (this->currentState != EtsukoMiyazatoAction::TURN_AROUND)
    {
        //行動をキャンセルする
        this->cancelActiveAction();
    }
    
        //壁に対するサイド
        this->lastWallSide = side;
        //壁に対する角度
        this->lastHitWallAngle = hitAngle;
        //現在ステートを前のステートに保存
        this->previousState = this->currentState;
        
        //状態をTURN_AROUNDにセット
        this->setCurrentState(EtsukoMiyazatoAction::TURN_AROUND);
}


//敵に玉が的中したとき(相手のposition)
void EtsukoMiyazato::bulletHitEnemy(RWVec enemyPosition)
{
    //行動をキャンセルする
    this->cancelActiveAction();
    
    //相手のポジション
    this->lastKnownPosition = enemyPosition;
    //相手のポジションを取得した時間
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    //自機の頭との敵の方向に対する角度取得
    //float angle = angleBetweenHeadingDirectionAndWorldPosition(this->lastKnownPosition);
    //自機の頭との敵の方向に対する角度取得
    //float angle =angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
    
    //状態をFIRINGにセット
    this->setCurrentState(EtsukoMiyazatoAction::FIRING);
}

/*
 状態
 */
//現在の状態をセット
void EtsukoMiyazato::setCurrentState(EtsukoMiyazatoAction::EtsukoMiyazatoAction newState)
{
    this->currentState = newState;
    
    this->actionIndex = 0;
}