//
//  NoriyukiRobot.hpp
//  RobotWar
//
//  Created by Noriyuki Matsuoka on 2016/05/06.
//  Copyright © 2016年 MakeGamesWithUs. All rights reserved.
//

#include "NoriyukiRobot.hpp"
#include <math.h>
using namespace std;

static const float GUN_ANGLE_TOLERANCE = 2.0f;

NoriyukiRobot::NoriyukiRobot()
{
    this->currentState = whichBetterBottomOrTop();
}

void NoriyukiRobot::scannedRobotAtPosition(RWVec position)
{
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
    
    this->timeSinceLastEnemyHit = this->currentTimestamp();
    this->currentState = NoriyukiRobotAction::FIRING;
}

void NoriyukiRobot::gotHit()
{
    
}

void NoriyukiRobot::run()
{
    while (true)
    {
        RWSize arena = this->arenaDimensions();
        RWVec myPosition =  this->position();
        float a;
        float b;
        float c;
        a = this->angleBetweenHeadingDirectionAndWorldPosition(RWVec(0, 0));
        b = this->angleBetweenHeadingDirectionAndWorldPosition(RWVec(100, 100));
        c = this->angleBetweenHeadingDirectionAndWorldPosition(RWVec(600, 0));
        switch (this->currentState)
        {
            case NoriyukiRobotAction::MOVETOBOTTOM:
                if (myPosition.x > arena.width * 4 / 5)
                {
                }else if (myPosition.x < arena.width * 3 / 5){
                }
                if (myPosition.y > arena.height / 5) {
                }
                break;

            case NoriyukiRobotAction::FIRING:
                if (this->currentTimestamp() - this->timeSinceLastEnemyHit > 2.5f)
                {
                    this->cancelActiveAction();
                    this->currentState = NoriyukiRobotAction::SCANNING;
                }
                else
                {
                    this->shoot();
                }
                break;
                
            case NoriyukiRobotAction::SCANNING:
                this->turnGunRight(90);
                break;
        }
    }
}

void NoriyukiRobot::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    
}

void NoriyukiRobot::bulletHitEnemy(RWVec enemyPosition)
{
    this->timeSinceLastEnemyHit = this->currentTimestamp();
}

NoriyukiRobotAction::NoriyukiRobotAction NoriyukiRobot::whichBetterBottomOrTop()
{
    RWSize arena = this->arenaDimensions();
    RWVec myPosition =  this->position();
    NoriyukiRobotAction::NoriyukiRobotAction state;
    if (myPosition.y > arena.height / 2) {
        state = NoriyukiRobotAction::MOVETOTOP;
    }else{
        state = NoriyukiRobotAction::MOVETOBOTTOM;
    }
    return state;
}

// 左はマイナス（-180まで）？
// 右はプラス？
//void NoriyukiRobot::moveDirectly(RWVec position)
//{
//    float degree = this->angleBetweenHeadingDirectionAndWorldPosition(position);
//    if (degree > 180)
//    if (degree > 0)
//    {
//        turnRobotLeft(degree);
//    }else{
//        turnRobotRight(-degree);
//    }
//}