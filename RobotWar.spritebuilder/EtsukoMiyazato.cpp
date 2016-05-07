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
    this->previousState = EtsukoMiyazatoAction::FIRING;
    this->actionIndex = 0;
    this->searchingIndex = 0;
    this->isDebug = true;
}

void EtsukoMiyazato::run()
{
    this->actionIndex = 0;
    this->searchingIndex = 0;
    this->lastKnownPosition = RWVec(this->arenaDimensions().width / 2, this->arenaDimensions().height / 2);
    
    while (true)
    {
        while (this->currentState == EtsukoMiyazatoAction::DEFAULT)
        {
            //this->performNextDefaultAction();
            this->performNextMoveStraightAction();
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
        
        while (this->currentState == EtsukoMiyazatoAction::MOVE_STRAIGHT)
        {
            this->performNextMoveStraightAction();
        }
    }
}

/*
 アクション
 */

//デフォルト
void EtsukoMiyazato::performNextDefaultAction()
{
    if (isDebug) printf("[Default]");
    
    /*
    switch (this->actionIndex % 4)
    {
        case 0: this->shoot(); break;
        case 1: this->turnRobotLeft(20); break;
        case 2: this->shoot(); break;
        case 3: this->turnRobotRight(20); break;
    }
    
    this->actionIndex++;
     */
    
    this->shoot();
}

//向きを変える
void EtsukoMiyazato::performNextTurnAroundAction()
{
    if (isDebug) printf("[TurnAround]");
    
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
    this->moveAhead(10);
    
    //前の状態に戻る
    if (this->previousState != EtsukoMiyazatoAction::MOVE_STRAIGHT && this->previousState != EtsukoMiyazatoAction::TURN_AROUND)
    {
        this->setCurrentState(this->previousState);
    } else {
        //状態をFIRINGにセット
        this->setCurrentState(EtsukoMiyazatoAction::FIRING);
    }
}

//撃つアクション
void EtsukoMiyazato::performNextFiringAction()
{
    if (isDebug) printf("[Firing]");
    
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f && searchingIndex <= 3)
    {
        this->setCurrentState(EtsukoMiyazatoAction::SEARCHING);
        //ここでランダムをつくる
        //this->lastKnownPosition = RWVec(<#float xx#>, <#float yy#>);
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
        
        this->searchingIndex = 0;
    }
}

//サーチ
void EtsukoMiyazato::performNextSearchingAction()
{
    if (isDebug) printf("[Search]");
    
    /*
    switch (this->actionIndex % 4)
    {
        case 0: this->moveAhead(50); break;
        case 1: this->turnRobotLeft(20); break;
        case 2: this->moveAhead(50); break;
        case 3: this->turnRobotRight(20); break;
    }
    
    this->actionIndex++;
     */
    
    this->turnRobotLeft(20);
    
    //サーチングインデックスが3超えたら
    this->searchingIndex ++ ;

    //状態をFIRINGにセット
    this->setCurrentState(EtsukoMiyazatoAction::FIRING);
}

//まっすぐ進む
void EtsukoMiyazato::performNextMoveStraightAction()
{
    if (isDebug) printf("[MoveStraight]");
    
    //アリーナサイズ(320,568)
    //printf("%f", this->arenaDimensions().width);
    //printf("%f", this->arenaDimensions().height);
    
    float angle = 0;
    //もしアリーナの右側いたら
    if (this->position().x >= this->arenaDimensions().width / 2) {
        angle = angleBetweenHeadingDirectionAndWorldPosition(RWVec(this->position().x, 0));
    } else {
        angle = angleBetweenHeadingDirectionAndWorldPosition(RWVec(this->position().x, this->arenaDimensions().width));
    }
    
    //まっすぐに戻す
    if (angle >= 0)
    {
        this->turnRobotRight(fabsf(angle));
    }
    else
    {
        this->turnRobotLeft(fabsf(angle));
    }
    
    this->moveAhead(100);
}

/*
 コールバック
 */
//敵の玉が自分にあたった時
void EtsukoMiyazato::gotHit()
{
    //行動をキャンセルする
    this->cancelActiveAction();
    
    //相手のX軸が自分の近くにいたら
    if (this->lastKnownPosition.x - this->position().x <= 30 && this->lastKnownPosition.x - this->position().x >= 0) {
        //状態をMOVE_STRAIGHTにセット
        this->setCurrentState(EtsukoMiyazatoAction::MOVE_STRAIGHT);
    } else if (this->position().x - this->lastKnownPosition.x <= 30 && this->position().x - this->lastKnownPosition.x >= 0) {
        //状態をMOVE_STRAIGHTにセット
        this->setCurrentState(EtsukoMiyazatoAction::MOVE_STRAIGHT);
    } else {
        
    }
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