//
//  TurretRobotCpp.cpp
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#include "TurretRobotCpp.h"
#include <math.h>

static const float GUN_ANGLE_TOLERANCE = 2.0f;

TurretRobotCpp::TurretRobotCpp()
{
    this->currentState = TurretRobotCppAction::FIRING;
}

void TurretRobotCpp::scannedRobotAtPosition(RWVec position)
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
    this->currentState = TurretRobotCppAction::FIRING;
}

void TurretRobotCpp::run()
{
    while (true)
    {
        this->robotBoundingBox();
        switch (this->currentState)
        {
            case TurretRobotCppAction::FIRING:
                if (this->currentTimestamp() - this->timeSinceLastEnemyHit > 2.5f)
                {
                    this->cancelActiveAction();
                    this->currentState = TurretRobotCppAction::SCANNING;
                }
                else
                {
                    this->shoot();
                }
                break;
                
            case TurretRobotCppAction::SCANNING:
                this->turnGunRight(90);
                break;
        }
    }
    
}

void TurretRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    this->timeSinceLastEnemyHit = this->currentTimestamp();
    this->robotBoundingBox();
}