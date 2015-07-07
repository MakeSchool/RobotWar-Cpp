//
//  AlaskaThunder.cpp
//  RobotWar
//
//  Created by Shota Oiyama on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

static const float GUN_ANGLE_TOLERANCE = 2.f;

#include <iostream>
#include "AlaskaThunderCpp.h"
#include <math.h>

AlaskaThunderCpp::AlaskaThunderCpp()
{
    this->currentState = AlaskaThunderCppAction::DEFAULT;
    this->gunAngleMoved = 0.0f;
}

void AlaskaThunderCpp::run()
{
    auto dimentions = this->arenaDimensions();
    float distance;
    int rnd;
    
    while (true)
    {
        switch (this->currentState)
        {
            case AlaskaThunderCppAction::DEFAULT:
                
                this->shoot();
                this->moveBack(35);
                this->turnGunRight(90);
                this->turnRobotLeft(90);
                this->currentState = AlaskaThunderCppAction::BLAST;
                break;
            
            case AlaskaThunderCppAction::BLAST:
            
                rnd = rand() % 3 + 3;
                /*
                 GO UP
                 */
                distance = (dimentions.height - this->position().y - this->robotBoundingBox().size.height / 2) / rnd;
                for (int i = 0; i < rnd; i++) {
                    this->moveAhead(distance);
                    this->shoot();
                }
                //std::cout << "rnd:" << rnd << ", dist:" << distance << std::endl;
            
                /*
                 GO DOWN
                 */
                rnd = rand() % 3 + 3;
                distance = (this->position().y - this->robotBoundingBox().size.height / 2) / rnd;
                //std::cout << "rnd:" << rnd << ", dist:" << distance << std::endl;
                for (int i = 0; i < rnd; i++) {
                    this->moveBack(distance);
                    this->shoot();
                }
            
                break;
            
            case AlaskaThunderCppAction::FIRING:
                if (this->currentTimestamp() - this->timeSinceLastEnemyHit > 7.f) {
                    this->cancelActiveAction();
                    this->currentState = AlaskaThunderCppAction::BLAST;
                    
                    lookRight();
                } else if (this->currentTimestamp() - this->timeSinceLastEnemyHit > 3.f) {
                    this->cancelActiveAction();
                    this->currentState = AlaskaThunderCppAction::BLAST;
                    
                    lookRight();
                } else {
                    this->shoot();
                }
            
                break;
        }
    }
}

void AlaskaThunderCpp::gotHit()
{
    /*
     DONT CARE
     JUST DO WHAT YOU ARE SUPPOSED TO DO
     */
    this->shoot();
}

void AlaskaThunderCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    /*
     DO NOTHING
     */
}

void AlaskaThunderCpp::bulletHitEnemy(RWVec enemyPosition)
{
    /*
     YEAY
     */
    float angleBetweenTurretAndEnemy = this->angleBetweenGunHeadingDirectionAndWorldPosition(enemyPosition);
    this->gunAngleMoved += angleBetweenTurretAndEnemy;
    
    if (angleBetweenTurretAndEnemy > GUN_ANGLE_TOLERANCE)
    {
        this->turnGunRight(fabsf(angleBetweenTurretAndEnemy));
    }
    else if (angleBetweenTurretAndEnemy < -GUN_ANGLE_TOLERANCE)
    {
        this->turnGunLeft(fabsf(angleBetweenTurretAndEnemy));
    }

    this->timeSinceLastEnemyHit = this->currentTimestamp();
    this->currentState = AlaskaThunderCppAction::FIRING;
    this->shoot();
}

void AlaskaThunderCpp::scannedRobotAtPosition(RWVec position)
{
    float angleBetweenTurretAndEnemy = this->angleBetweenGunHeadingDirectionAndWorldPosition(position);
    this->gunAngleMoved += angleBetweenTurretAndEnemy;
    
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
    this->currentState = AlaskaThunderCppAction::FIRING;
}

void AlaskaThunderCpp::lookRight()
{
    // restore the position of the gun.
    float angle = this->gunAngleMoved;
    if (angle >= 0.0f) {
        this->turnGunLeft(fabsf(angle));
    } else {
        this->turnGunRight(fabsf(angle));
    }
    this->gunAngleMoved = 0.0;
}