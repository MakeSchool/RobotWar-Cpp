//
//  NewAdvancedRobotCpp.cpp
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#include "NewAdvancedRobotCpp.h"
#include <math.h>

NewAdvancedRobotCpp::NewAdvancedRobotCpp()
{
    this->currentState = NewAdvancedRobotCppAction::DEFAULT;
    this->actionIndex = 0;
}

void NewAdvancedRobotCpp::run()
{
    this->actionIndex = 0;
    
    while (true)
    {
        while (this->currentState == NewAdvancedRobotCppAction::FIRING)
        {
            this->performNextFiringAction();
        }
        
        while (this->currentState == NewAdvancedRobotCppAction::SEARCHING)
        {
            this->performNextSearchingAction();
        }
        
        while (this->currentState == NewAdvancedRobotCppAction::DEFAULT)
        {
            this->performNextDefaultAction();
        }
    }
}

void NewAdvancedRobotCpp::performNextDefaultAction()
{
    switch (this->actionIndex % 1)
    {
        case 0:
            this->moveAhead(100);
            break;
    }
    
    this->actionIndex++;
}

void NewAdvancedRobotCpp::performNextFiringAction()
{
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f)
    {
        this->setCurrentState(NewAdvancedRobotCppAction::SEARCHING);
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

void NewAdvancedRobotCpp::performNextSearchingAction()
{
    switch (this->actionIndex % 4)
    {
        case 0: this->moveAhead(50); break;
        case 1: this->turnRobotLeft(20); break;
        case 2: this->moveAhead(50); break;
        case 3: this->turnRobotRight(20); break;
    }
    
    this->actionIndex++;
}

void NewAdvancedRobotCpp::gotHit()
{
    this->shoot();
    this->turnRobotLeft(45);
    this->moveAhead(100);
}

void NewAdvancedRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    if (this->currentState != NewAdvancedRobotCppAction::TURN_AROUND)
    {
        this->cancelActiveAction();
        
        NewAdvancedRobotCppAction::NewAdvancedRobotCppAction previousState = this->currentState;
        this->setCurrentState(NewAdvancedRobotCppAction::TURN_AROUND);
        
        if (hitAngle >= 0)
        {
            this->turnRobotLeft(fabs(hitAngle));
        }
        else
        {
            this->turnRobotRight(fabs(hitAngle));
        }
        
        this->moveAhead(20);
        
        this->setCurrentState(previousState);
    }
}

void NewAdvancedRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    
}

void NewAdvancedRobotCpp::scannedRobotAtPosition(RWVec position)
{
    if (this->currentState != NewAdvancedRobotCppAction::FIRING)
    {
        this->cancelActiveAction();
    }
    
    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    
    this->setCurrentState(NewAdvancedRobotCppAction::FIRING);
    
}

void NewAdvancedRobotCpp::setCurrentState(NewAdvancedRobotCppAction::NewAdvancedRobotCppAction newState)
{
    this->currentState = newState;
    
    this->actionIndex = 0;
}