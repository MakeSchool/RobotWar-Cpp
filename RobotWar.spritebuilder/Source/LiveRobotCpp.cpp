//
//  LiveRobotCpp.cpp
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#include "LiveRobotCpp.h"
#include <math.h>

void LiveRobotCpp::run()
{
    this->setCurrentState(LiveRobotCppAction::FIRST_MOVE);
    
    while (true)
    {
        while (this->currentState == LiveRobotCppAction::FIRING)
        {
            this->performNextFiringAction();
        }
        
        while (this->currentState == LiveRobotCppAction::FIRST_MOVE)
        {
            this->performNextFirstMove();
        }
    }
}

void LiveRobotCpp::performNextFirstMove()
{
    switch (this->actionIndex % 4)
    {
        case 0:
        {
            RWVec currentPosition = this->position();
            RWSize arenaSize = this->arenaDimensions();
            
            if (currentPosition.y < arenaSize.height / 2.0f)
            {
                if (currentPosition.x < arenaSize.width / 2.0f)
                {
                    // bottom left
                    this->turnRobotLeft(90);
                }
                else
                {
                    // bottom right
                    this->turnRobotRight(90);
                }
            }
            else
            {
                if (currentPosition.x < arenaSize.width / 2.0f)
                {
                    // top left
                    this->turnRobotRight(90);
                }
                else
                {
                    // top right
                    this->turnRobotLeft(90);
                }
            }
        }
            break;
            
        case 1:
        {
            RWVec currentPosition = this->position();
            RWSize arenaSize = this->arenaDimensions();
            float bodyLength = this->robotBoundingBox().size.width;
            
            if (currentPosition.y < arenaSize.height / 2.0f)
            {
                this->moveBack(currentPosition.y - bodyLength);
            }
            else
            {
                this->moveBack((arenaSize.height - (currentPosition.y + bodyLength)));
            }
        }
            break;
            
        case 2:
        {
            RWSize arenaSize = this->arenaDimensions();
            float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(RWVec(arenaSize.width / 2.0f, arenaSize.height / 2.0f));
            
            if (angle < 0.0f)
            {
                this->turnGunLeft(fabsf(angle));
            }
            else
            {
                this->turnGunRight(fabsf(angle));
            }
        }
            break;
            
        case 3:
            this->shoot();
            this->setCurrentState(LiveRobotCppAction::WAITING);
            break;
    }
    
    this->actionIndex++;
}

void LiveRobotCpp::performNextFiringAction()
{
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f)
    {
        this->setCurrentState(LiveRobotCppAction::SEARCHING);
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
}

void LiveRobotCpp::gotHit()
{
    this->shoot();
    this->turnRobotLeft(45);
    this->moveAhead(100);
}

void LiveRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{

}

void LiveRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    this->shoot();
}

void LiveRobotCpp::scannedRobotAtPosition(RWVec position)
{
    if (this->currentState != LiveRobotCppAction::FIRING)
    {
        this->cancelActiveAction();
    }
    
    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    
    this->setCurrentState(LiveRobotCppAction::FIRING);
    
}

void LiveRobotCpp::setCurrentState(LiveRobotCppAction::LiveRobotCppAction newState)
{
    this->currentState = newState;
    
    this->actionIndex = 0;
}