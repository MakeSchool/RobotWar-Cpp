//
//  SimpleRobotCpp.cpp
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#include "EtsukoMiyazato.hpp"

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
    //打つ
    this->shoot();
}

//向きを変える
void EtsukoMiyazato::performNextTurnAroundAction()
{
    
}

//撃つ
void EtsukoMiyazato::performNextFiringAction()
{
    
}

//サーチ
void EtsukoMiyazato::performNextSearchingAction()
{
    
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
    //行動をキャンセルする
    this->cancelActiveAction();
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