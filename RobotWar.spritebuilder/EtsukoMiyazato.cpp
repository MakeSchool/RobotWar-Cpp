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
#include <stdlib.h>

float const MY_ANGLE = 30;

EtsukoMiyazato::EtsukoMiyazato()
{
    this->isDebug = false;
}

void EtsukoMiyazato::run()
{
    while (true)
    {
        
        if (!isInitialize) {
            this->currentState = EtsukoMiyazatoAction::DEFAULT;
            this->previousState = EtsukoMiyazatoAction::DEFAULT;
            this->actionIndex = 0;
            this->searchingIndex = 0;
            this->firstActionIndex = 0;
            this->isFirstAction = false;
            this->isInitialize = false;
            
            this->lastKnownPosition = RWVec(this->arenaDimensions().width / 2, this->arenaDimensions().height / 2);
            isInitialize = true;
            
            RWVec position;
            int distance;
            //フィールドの上の方にいたら上に移動
            if (this->position().y >= this->arenaDimensions().height / 2) {
                position = RWVec(this->position().x, this->arenaDimensions().height);
                distance = fabs(this->arenaDimensions().height - this->position().y + 1);
            } else {
                position = RWVec(this->position().x, 0);
                distance = fabs(this->position().y + 1);
            }
            
            //向きを変える
            float angle = this->angleBetweenHeadingDirectionAndWorldPosition(position);
            if (angle >= 0) {
                this->turnRobotRight(fabsf(angle));
            } else {
                this->turnRobotLeft(fabsf(angle));
            }
            
            //まっすぐ進む
            this->moveAhead(distance);
        }
        
        this->actionIndex = 0;
        this->searchingIndex = 0;
        
        
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
    
    //前の状態がデフォルトではなかったら
    if (this->previousState != EtsukoMiyazatoAction::DEFAULT) {
        isFirstAction = false;
    }
    
    //始めの処理
    if (!this->isFirstAction) {
        
        this->lastKnownPosition = RWVec(this->arenaDimensions().width / 2, this->arenaDimensions().height / 2);
        //アリーナの真ん中にガンを向ける
        float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
        
        if (angle >= 0) {
            this->isTurnGunRight = true;
        } else {
            this->isTurnGunRight = false;
        }
        
        this->isFirstAction = true;
        this->firstActionIndex = 0;
        this->firstActionCount = 0;
    }
    
    this->shoot();
    
    this->firstActionIndex ++;
    
    //5回でデフォルトのままだったら
    if (this->firstActionIndex >= 3) {
        
        //this->setCurrentState(EtsukoMiyazatoAction::MOVE_STRAIGHT);
        
        /*
         if (this->isTurnGunRight)
         {
         this->turnGunLeft(fabsf(MY_ANGLE));
         }
         else
         {
         this->turnGunRight(fabsf(MY_ANGLE));
         }
         */
        
        this->isTurnGunRight = !this->isTurnGunRight;
        
        this->firstActionIndex = 0;
        
        this->firstActionCount ++;
        
        if (this->firstActionCount >= 3) {
            this->setCurrentState(EtsukoMiyazatoAction::MOVE_STRAIGHT);
            this->firstActionCount = 0;
        }
    }
    
    //trueだったら右回転
    if (this->isTurnGunRight)
    {
        this->turnGunRight(fabsf(MY_ANGLE));
    }
    else
    {
        this->turnGunLeft(fabsf(MY_ANGLE));
    }
    
    this->previousState = EtsukoMiyazatoAction::DEFAULT;
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
    
    //前に10進む
    //this->moveAhead(10);
    
    this->lastKnownPosition = RWVec(this->arenaDimensions().width / 2, this->arenaDimensions().height / 2);
    /*
     //アリーナの正面にガンを向ける
     float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
     if (angle >= 0)
     {
     this->turnGunRight(fabsf(angle));
     }
     else
     {
     this->turnGunLeft(fabsf(angle));
     }
     */
    
    
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
        this->setCurrentState(EtsukoMiyazatoAction::DEFAULT);
        
        //ここでランダムをつくる
        float random = rand() % 2; //0から1の値を返す
        if (random == 0) {
            this->lastKnownPosition = RWVec(this->lastKnownPosition.x + MY_ANGLE, this->lastKnownPosition.y);
        } else {
            this->lastKnownPosition = RWVec(this->lastKnownPosition.x, this->lastKnownPosition.y + MY_ANGLE);
        }
        
    }
    else
    {
        //ガンをラストポシションに向ける
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
    
    //ランダム生成
    float random = rand() % 2; //0から1の値を返す
    if (random == 0) {
        this->turnRobotRight(fabsf(MY_ANGLE));
    } else {
        this->turnRobotLeft(fabsf(MY_ANGLE));
    }
    
    //サーチングインデックスが3超えたらショットに切り替える
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
        angle = angleBetweenHeadingDirectionAndWorldPosition(RWVec(0, this->position().y));
    } else {
        angle = angleBetweenHeadingDirectionAndWorldPosition(RWVec(this->arenaDimensions().width, this->position().y));
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
    
    //左側にいたら
    if (this->position().x <= this->arenaDimensions().width / 2) {
        this->moveAhead(fabsf(this->arenaDimensions().width - this->position().x + 1));
    } else {
        this->moveAhead(fabsf(this->position().x + 1));
    }
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
        //状態をMOVE_STRAIGHTにセット
        this->setCurrentState(EtsukoMiyazatoAction::MOVE_STRAIGHT);
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