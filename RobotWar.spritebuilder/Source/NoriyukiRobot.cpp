//
//  NoriyukiRobot.hpp
//  RobotWar
//
//  Created by Noriyuki Matsuoka on 2016/05/06.
//  Copyright © 2016年 MakeGamesWithUs. All rights reserved.
//

#include "NoriyukiRobot.hpp"
#include <math.h>
#include <random>
#include "time.h"

static const float GUN_ANGLE_TOLERANCE = 2.0f;

NoriyukiRobot::NoriyukiRobot()
{
    this->currentState = NoriyukiRobotAction::INIT;
    this->lastAimedPositionNumber = 0;
    srand((unsigned int)time(NULL));
}

void NoriyukiRobot::scannedRobotAtPosition(RWVec position)
{
    if (false){
        float angleBetweenTurretAndEnemy = this->angleBetweenGunHeadingDirectionAndWorldPosition(position);
        
        if (angleBetweenTurretAndEnemy > GUN_ANGLE_TOLERANCE)
        {
            this->cancelActiveAction();
            this->turnGunRight(fabsf(angleBetweenTurretAndEnemy));
        }
        else if (angleBetweenTurretAndEnemy < -GUN_ANGLE_TOLERANCE)
        {
            this->cancelActiveAction();
            this->turnGunLeft(fabsf(angleBetweenTurretAndEnemy));
        }
        
        RWVec myPosition = this->position();
        RWVec middlePosition = RWVec((myPosition.x + position.x) / 2, (myPosition.y + position.y) / 2);
        this->moveDirectly(middlePosition);
        
        this->timeSinceLastEnemyHit = this->currentTimestamp();
        this->currentState = NoriyukiRobotAction::FIRING;
    }
}

void NoriyukiRobot::bulletHitEnemy(RWVec enemyPosition)
{
    this->cancelActiveAction();
    this->timeSinceLastEnemyHit = this->currentTimestamp();
    this->turnGunTo(enemyPosition);
    RWVec myPosition = this->position();
    RWVec middlePosition = RWVec((myPosition.x + enemyPosition.x) / 2, (myPosition.y + enemyPosition.y) / 2);
    this->moveDirectly(middlePosition);
    this->currentState = NoriyukiRobotAction::FIRING;
}

void NoriyukiRobot::gotHit()
{
//    this->cancelActiveAction();
//    this->currentState = NoriyukiRobotAction::CHANGE_POSITION;
}

void NoriyukiRobot::run()
{
    RWSize arena;
    RWVec middle;
    while (true)
    {
        RWVec myPosition =  this->position();
        if (myPosition.x < 1 && myPosition.y < 1 && this->currentState == NoriyukiRobotAction::INIT) {
            // まだarenaなどのオブジェクトが生成されていない
            continue;
        }else if (this->currentState == NoriyukiRobotAction::INIT){
            // 初期化
            arena = this->arenaDimensions();
            middle = RWVec(arena.width / 2, arena.height / 2);
            this->positionsToHit[0] = RWVec(0, 0);
            this->positionsToHit[1] = RWVec(0, arena.height);
            this->positionsToHit[2] = RWVec(arena.width, arena.height);
            this->positionsToHit[3] = RWVec(arena.width, 0);
        }
        switch (this->currentState)
        {
            case NoriyukiRobotAction::INIT:
                this->moveDirectly(whichBestPosition());
                this->currentState = NoriyukiRobotAction::SCAN_AND_HIT;
                break;
                
            case NoriyukiRobotAction::CHANGE_POSITION:
                this->moveDirectly(whichBestPosition());
                this->currentState = NoriyukiRobotAction::SCAN_AND_HIT;
                break;

            case NoriyukiRobotAction::FIRING:
                if (this->currentTimestamp() - this->timeSinceLastEnemyHit > 2.5f)
                {
//                    this->cancelActiveAction();
                    this->currentState = NoriyukiRobotAction::CHANGE_POSITION;
                }
                else
                {
                    this->shoot();
                }
                break;
                
            case NoriyukiRobotAction::SCAN_AND_HIT:
                if (this->currentTimestamp() - this->timeSinceLastEnemyHit > 5.0f)
                {
//                    this->cancelActiveAction();
                    this->currentState = NoriyukiRobotAction::HITTING_AROUND;
                }
                else
                {
                    this->turnGunTo(this->positionsToHit[this->lastAimedPositionNumber]);
                    this->lastAimedPositionNumber = (this->lastAimedPositionNumber + 1) % 4;
                    this->shoot();
                    this->shoot();
                }
                break;
                
            case NoriyukiRobotAction::HITTING_AROUND:
                if (!this->RobotIsPlacedNear(middle)) {
                    this->moveDirectly(middle);
                }
                this->turnRobotRight(20);
                this->shoot();
        }
    }
}

void NoriyukiRobot::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    
}

// 4隅のどこに移動するべきかを決定する
RWVec NoriyukiRobot::whichBestPosition()
{
    RWSize arena = this->arenaDimensions();
    RWVec myPosition =  this->position();
    RWVec bestPosition;
    float yTop = arena.height * 4 / 5;
    float yBottom = arena.height / 5;
    float xRight = arena.width * 4 / 5;
    float xLeft = arena.width / 5;
    if (myPosition.y > arena.height / 2 && fabsf(myPosition.y - yTop) > 10) {
        bestPosition.y = yTop;
    }else{
        bestPosition.y = yBottom;
    }
    if (myPosition.x > arena.width / 2 && fabsf(myPosition.x - xRight) > 10) {
        bestPosition.x = xRight;
    }else{
        bestPosition.x = xLeft;
    }
    return bestPosition;
}

// 指定したポジションに最短距離で移動する
void NoriyukiRobot::moveDirectly(RWVec position)
{
    this->turnHeadingTo(position);
    RWVec myPosition =  this->position();
    float degree = this->angleBetweenHeadingDirectionAndWorldPosition(position);
    float distance = pow(pow(position.x - myPosition.x, 2) + pow(position.y - myPosition.y, 2), 0.5);
    if (degree >= -1 && degree < 1) {
        this->moveAhead(distance);
    }else{
        this->moveBack(distance);
    }
}

// ロボットが指定したポジションに位置しているかどうか
bool NoriyukiRobot::RobotIsPlacedNear(RWVec position)
{
    RWVec myPosition = this->position();
    return 10 > (pow(myPosition.x - position.x, 2) + pow(myPosition.y - position.y, 2));
}

// 指定したポジションに振り向く
void NoriyukiRobot::turnHeadingTo(RWVec position)
{
    float degree = this->angleBetweenHeadingDirectionAndWorldPosition(position);
    if (degree > 0)
    {
        if (degree > 90) {
            this->turnRobotLeft(degree - 90);
        }else{
            this->turnRobotRight(degree);
        }
    }else{
        if( degree < -90){
            this->turnRobotRight(- degree + 90);
        }else{
            this->turnRobotLeft(- degree);
        }
    }
}

// 指定したポジションにガンを向ける
void NoriyukiRobot::turnGunTo(RWVec position)
{
    float degree = this->angleBetweenGunHeadingDirectionAndWorldPosition(position);
    if (degree > 0)
    {
        this->turnGunRight(degree);
    }else{
        this->turnGunLeft(-degree);
    }
}