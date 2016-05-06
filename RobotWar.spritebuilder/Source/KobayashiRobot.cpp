//
//  KobayashiRobot.cpp
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#include "KobayashiRobot.h"
#include <math.h>

KobayashiRobot::KobayashiRobot()
{
    this->currentState = KobayashiRobotAction::DEFAULT;
    this->actionIndex = 0;
}

void KobayashiRobot::run()
{
    this->actionIndex = 0;
    
    while (true)
    {
        while (this->currentState == KobayashiRobotAction::FIRING)
        {
            this->performNextFiringAction();
        }
        
        while (this->currentState == KobayashiRobotAction::SEARCHING)
        {
            this->performNextSearchingAction();
        }
        
        while (this->currentState == KobayashiRobotAction::DEFAULT)
        {
            this->performNextDefaultAction();
        }
    }
}

void KobayashiRobot::performNextDefaultAction()
{
    switch (this->actionIndex % 1)
    {
        case 0:
            this->moveAhead(100);
            break;
    }
    
    this->actionIndex++;
}

void KobayashiRobot::performNextFiringAction()
{
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f)
    {
        this->setCurrentState(KobayashiRobotAction::SEARCHING);
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

void KobayashiRobot::performNextSearchingAction()
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

void KobayashiRobot::gotHit()
{
    this->shoot();
    this->turnRobotLeft(45);
    this->moveAhead(100);
}

void KobayashiRobot::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    if (this->currentState != KobayashiRobotAction::TURN_AROUND)
    {
        this->cancelActiveAction();
        
        KobayashiRobotAction::KobayashiRobotAction previousState = this->currentState;
        this->setCurrentState(KobayashiRobotAction::TURN_AROUND);
        
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

void KobayashiRobot::bulletHitEnemy(RWVec enemyPosition)
{
    
}

void KobayashiRobot::scannedRobotAtPosition(RWVec position)
{
    if (this->currentState != KobayashiRobotAction::FIRING)
    {
        this->cancelActiveAction();
    }
    
    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    
    this->setCurrentState(KobayashiRobotAction::FIRING);
    
}

void KobayashiRobot::setCurrentState(KobayashiRobotAction::KobayashiRobotAction newState)
{
    this->currentState = newState;
    
    this->actionIndex = 0;
}