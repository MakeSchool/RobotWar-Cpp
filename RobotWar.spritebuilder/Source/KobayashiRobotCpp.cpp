//
//  KobayashiRobotCpp.cpp
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#include "KobayashiRobotCpp.h"
#include <math.h>
#include <stdio.h>

static const float GUN_ANGLE_TOLERANCE = 2.0f;

KobayashiRobotCpp::KobayashiRobotCpp()
{
    this->currentAction = KobayashiRobotCppAction::SCANNING;
    RWSize size = this->arenaDimensions();
    printf("%f %f\n", size.width, size.height);
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
    this->moveAhead(5);
    this->moveBack(45);
    this->eimingPos(this->getCenterPoint());
    this->shoot();
}

void KobayashiRobotCpp::firingAction()
{
    if (this->currentTimestamp() - this->timeSinceLastEnemyHit > 1.0f)
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

void KobayashiRobotCpp::eimingPos(RWVec pos)
{
    float angleBetweenTurretAndCenter =
    this->angleBetweenGunHeadingDirectionAndWorldPosition(pos);
    this->cancelActiveAction();
    if (angleBetweenTurretAndCenter > GUN_ANGLE_TOLERANCE)
    {
        this->turnGunRight(fabsf(angleBetweenTurretAndCenter));
    }
    else if (angleBetweenTurretAndCenter < -GUN_ANGLE_TOLERANCE)
    {
        this->turnGunLeft(fabsf(angleBetweenTurretAndCenter));
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
    this->moveBack(250);
}

void KobayashiRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    this->cancelActiveAction();
    printf("Side %d\n", side);
    printf("hitAngle %f\n", hitAngle);

    switch (side)
    {
        case RobotWallHitSide::FRONT:
            this->turnRobotRight(180 - hitAngle);
            break;
            
        case RobotWallHitSide::REAR:
            this->turnRobotRight(90 - hitAngle);
            break;
            
        case RobotWallHitSide::LEFT:
            this->turnRobotLeft(hitAngle);
            break;
            
        case RobotWallHitSide::RIGHT:
            this->turnRobotRight(hitAngle);
            break;
            
        case RobotWallHitSide::NONE:
            break;
    }
}

void KobayashiRobotCpp::scannedRobotAtPosition(RWVec enemyPos)
{
    this->bulletHitEnemy(enemyPos);
}

void KobayashiRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    this->cancelActiveAction();
    this->eimingPos(enemyPosition);
    this->currentAction = KobayashiRobotCppAction::FIRING;
    this->timeSinceLastEnemyHit = this->currentTimestamp();
}