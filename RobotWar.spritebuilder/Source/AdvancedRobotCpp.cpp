//
//  AdvancedRobotCpp.cpp
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#include "AdvancedRobotCpp.h"
#include <math.h>

AdvancedRobotCpp::AdvancedRobotCpp()
{
    this->currentState = AdvancedRobotCppAction::DEFAULT;
}

void AdvancedRobotCpp::run()
{
    while (true)
    {
        switch (this->currentState)
        {
            case AdvancedRobotCppAction::FIRING:
                if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f)
                {
                    this->currentState = AdvancedRobotCppAction::SEARCHING;
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
                
            case AdvancedRobotCppAction::SEARCHING:
                this->moveAhead(50);
                this->turnRobotLeft(20);
                this->moveAhead(50);
                this->turnRobotRight(20);
                break;
                
            case AdvancedRobotCppAction::DEFAULT:
                this->moveAhead(100);
                break;
                
            case AdvancedRobotCppAction::TURN_AROUND:
                break;
        }
    }
}

void AdvancedRobotCpp::gotHit()
{
    this->shoot();
    this->turnRobotLeft(45);
    this->moveAhead(100);
}

void AdvancedRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    if (this->currentState != AdvancedRobotCppAction::TURN_AROUND)
    {
        this->cancelActiveAction();
        
        AdvancedRobotCppAction::AdvancedRobotCppAction previousState = this->currentState;
        this->currentState = AdvancedRobotCppAction::TURN_AROUND;
        
        
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

void AdvancedRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    
}

void AdvancedRobotCpp::scannedRobotAtPosition(RWVec position)
{
    if (this->currentState != AdvancedRobotCppAction::FIRING)
    {
        this->cancelActiveAction();
    }
    
    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    this->currentState = AdvancedRobotCppAction::FIRING;
    
}