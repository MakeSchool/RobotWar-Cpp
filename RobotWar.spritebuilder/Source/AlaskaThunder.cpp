//
//  AlaskaThunder.cpp
//  RobotWar
//
//  Created by Shota Oiyama on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include "AlaskaThunder.h"
#include <math.h>

AlaskaThunderCpp::AlaskaThunderCpp()
{
    this->currentState = AlaskaThunderCppAction::DEFAULT;
}

void AlaskaThunderCpp::run()
{
    while (true)
    {
        switch (this->currentState)
        {
            case AlaskaThunderCppAction::FIRING:
            if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f)
            {
                this->currentState = AlaskaThunderCppAction::SEARCHING;
            }
            else
            {
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
            }
            break;
            
            case AlaskaThunderCppAction::SEARCHING:
            this->moveAhead(50);
            this->turnRobotLeft(20);
            this->moveAhead(50);
            this->turnRobotRight(20);
            break;
            
            case AlaskaThunderCppAction::DEFAULT:
            this->moveAhead(100);
            break;
            
            case AlaskaThunderCppAction::TURN_AROUND:
            break;
        }
    }
}

void AlaskaThunderCpp::gotHit()
{
    this->shoot();
    this->turnRobotLeft(45);
    this->moveAhead(100);
}

void AlaskaThunderCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    if (this->currentState != AlaskaThunderCppAction::TURN_AROUND)
    {
        this->cancelActiveAction();
        
        AlaskaThunderCppAction::AlaskaThunderCppAction previousState = this->currentState;
        this->currentState = AlaskaThunderCppAction::TURN_AROUND;
        
        
        if (hitAngle >= 0)
        {
            this->turnRobotLeft(fabs(hitAngle));
        }
        else
        {
            this->turnRobotRight(fabs(hitAngle));
        }
        
        this->moveAhead(20);
        
        this->currentState = previousState;
    }
}

void AlaskaThunderCpp::bulletHitEnemy(RWVec enemyPosition)
{
    
}

void AlaskaThunderCpp::scannedRobotAtPosition(RWVec position)
{
    if (this->currentState == AlaskaThunderCppAction::FIRING)
    {
        this->cancelActiveAction();
    }
    
    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    this->currentState = AlaskaThunderCppAction::FIRING;
    
}