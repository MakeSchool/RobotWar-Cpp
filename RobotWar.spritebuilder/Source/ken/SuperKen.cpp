//
//  SuperKen.cpp
//  RobotWar
//
//  Created by Ken Watanabe on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include "SuperKen.h"
#include <math.h>
#include <stdlib.h>

static const float GUN_ANGLE_TOLERANCE = 2.0f;

SuperKen::SuperKen()
{
    this->currentState = SuperKenAction::INIT;
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
    auto dimentions = this->arenaDimensions();
    float distance;
    int rnd;
    
    
    while (true)
    {
        switch (this->currentState)
        {
            case SuperKenAction::INIT:
                this->turnRobotLeft(90);
                this->turnGunRight(90);
                this->currentState = SuperKenAction::SCANNING;
                break;
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
                rnd = rand() % 5 + 5;
                distance = (dimentions.height - this->position().y - this->robotBoundingBox().size.height / 2) / rnd;
                for (int i = 0; i < rnd; i++) {
                    this->moveAhead(distance);
                    this->shoot();
                }
                
                rnd = rand() % 5 + 5;
                distance = (this->position().y - this->robotBoundingBox().size.height / 2) / rnd;
                for (int i = 0; i < rnd; i++) {
                    this->moveBack(distance);
                    this->shoot();
                }
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