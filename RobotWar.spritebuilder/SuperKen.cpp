//
//  SuperKen.cpp
//  RobotWar
//
//  Created by Ken Watanabe on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include "SuperKen.h"
#include <math.h>

static const float GUN_ANGLE_TOLERANCE = 2.0f;

SuperKen::SuperKen()
{
    this->currentState = SuperKenAction::SCANNING;
}

void SuperKen::scannedRobotAtPosition(RWVec position)
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
    this->currentState = SuperKenAction::FIRING;
}

void SuperKen::run()
{
    auto fuga = this->position();
    auto hoge = this->arenaDimensions();
    
    
    while (true)
    {
        switch (this->currentState)
        {
            case SuperKenAction::FIRING:
                if (this->currentTimestamp() - this->timeSinceLastEnemyHit > 2.5f)
                {
                    this->cancelActiveAction();
                    this->currentState = SuperKenAction::SCANNING;
                }
                else
                {
                    this->shoot();
                }
                break;
                
            case SuperKenAction::SCANNING:
                this->turnGunRight(90);
                break;
            default:
                break;
        }
    }
}

void SuperKen::bulletHitEnemy(RWVec enemyPosition)
{
    this->timeSinceLastEnemyHit = this->currentTimestamp();
}