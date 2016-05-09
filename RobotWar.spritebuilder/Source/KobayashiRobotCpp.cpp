//
//  KobayashiRobotCpp.cpp
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#include "KobayashiRobotCpp.h"
#include <math.h>

static const float GUN_ANGLE_TOLERANCE = 2.0f;

KobayashiRobotCpp::KobayashiRobotCpp()
{
    this->currentAction = KobayashiRobotCppAction::SCANNING;
}

void KobayashiRobotCpp::run()
{
    while(true)
    {
        switch (this->currentAction)
        {
            case KobayashiRobotCppAction::SCANNING:
            {
                this->scaningAction();
                break;
            }
                
            case KobayashiRobotCppAction::FIRING:
            {
                this->firingAction();
                break;
            }
            default:
                break;
        }
    }
}

void KobayashiRobotCpp::scaningAction()
{
    this->moveAhead(1);
    this->moveBack(40);
    float angleBetweenTurretAndCenter =
    this->angleBetweenGunHeadingDirectionAndWorldPosition(this->getCenterPoint());
    if (angleBetweenTurretAndCenter > GUN_ANGLE_TOLERANCE)
    {
        this->cancelActiveAction();
        this->turnGunRight(fabsf(angleBetweenTurretAndCenter));
    }
    else if (angleBetweenTurretAndCenter < -GUN_ANGLE_TOLERANCE)
    {
        this->cancelActiveAction();
        this->turnGunLeft(fabsf(angleBetweenTurretAndCenter));
    }
    this->shoot();
}

void KobayashiRobotCpp::firingAction()
{
    if (this->currentTimestamp() - this->timeSinceLastEnemyHit > 2.5f)
    {
        this->cancelActiveAction();
        this->currentAction = KobayashiRobotCppAction::SCANNING;
    }
    else
    {
        this->turnGunRight(15.0f);
        this->shoot();
    }
}

RWVec KobayashiRobotCpp::getCenterPoint()
{
    RWSize size = this->arenaDimensions();
    return RWVec(size.width/2, size.height/2);
}

void KobayashiRobotCpp::gotHit()
{
    this->cancelActiveAction();
    this->moveBack(200);
}

void KobayashiRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    this->cancelActiveAction();
    
    switch (side)
    {
        case RobotWallHitSide::FRONT:
            this->turnRobotRight(90);
            break;
            
        case RobotWallHitSide::REAR:
            this->turnRobotRight(90);
            break;
            
        case RobotWallHitSide::LEFT:
            
            break;
            
        case RobotWallHitSide::RIGHT:
            break;
            
        case RobotWallHitSide::NONE:
            break;
    }
}

void KobayashiRobotCpp::scannedRobotAtPosition(RWVec enemyPos)
{
    this->cancelActiveAction();
    
    float angleBetweenTurretAndCenter =
    this->angleBetweenGunHeadingDirectionAndWorldPosition(enemyPos);
    if (angleBetweenTurretAndCenter > GUN_ANGLE_TOLERANCE)
    {
        this->cancelActiveAction();
        this->turnGunRight(fabsf(angleBetweenTurretAndCenter));
    }
    else if (angleBetweenTurretAndCenter < -GUN_ANGLE_TOLERANCE)
    {
        this->cancelActiveAction();
        this->turnGunLeft(fabsf(angleBetweenTurretAndCenter));
    }
    this->shoot();
    this->currentAction = KobayashiRobotCppAction::FIRING;
}

void KobayashiRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    this->currentAction = KobayashiRobotCppAction::FIRING;
    this->timeSinceLastEnemyHit = this->currentTimestamp();
}