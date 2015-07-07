//
//  CookieRobotCpp.cpp
//  RobotWar
//
//  Created by Tingting Gu on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include "CookieRobotCpp.h"
#include <math.h>
#include <random>
#include <iostream>

CookieRobotCpp::CookieRobotCpp()
{
    this->currentState = CookieRobotCppAction::INIT;
}

#pragma mark -
#pragma mark Lifecycle

void CookieRobotCpp::run()
{
    while (true)
    {
        switch (this->currentState) {
            case CookieRobotCppAction::INIT:
                this->gameInit();
                break;
                
            case CookieRobotCppAction::FIRING:
                this->gunFiring();
                break;
                
            case CookieRobotCppAction::SEARCHING:
                this->searching();
                break;
                
            case CookieRobotCppAction::ESCAPING:
                this->escaping();
                break;
                
            case CookieRobotCppAction::TURN_AROUND:
                break;
        }
    }
}

#pragma mark -
#pragma mark Public Methods

void CookieRobotCpp::gotHit()
{
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f)
    {
        this->turnGunRight(30);
        this->moveAhead(50);
    }
    this->shoot();
}

void CookieRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    if (this->currentState != CookieRobotCppAction::TURN_AROUND)
    {
        this->cancelActiveAction();
        
        CookieRobotCppAction::CookieRobotCppAction previousState = this->currentState;
        this->setCurrentState(CookieRobotCppAction::TURN_AROUND);
        
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

void CookieRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    
}

void CookieRobotCpp::scannedRobotAtPosition(RWVec position)
{
    if (this->currentState != CookieRobotCppAction::FIRING)
    {
        this->cancelActiveAction();
    }
    
    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    
    this->setCurrentState(CookieRobotCppAction::FIRING);
}

#pragma mark -
#pragma mark Private Methods

void CookieRobotCpp::gameInit()
{
    this->enemysHitPoints = this->hitPoints();
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    this->actionIndex = 0;
    this->moveAhead(100);
    this->turnRobotRight(30);
    this->turnGunLeft(30);
    this->actionIndex++;
}

void CookieRobotCpp::gunFiring()
{
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 2.0f)
    {
        this->currentState = CookieRobotCppAction::SEARCHING;
    }
    else if (this->currentTimestamp() - this->lastKnownPositionTimestamp <= 1.0f)
    {
        float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
        
        // if the angle between gun direction and the enemy's position is positive
        
        if (angle >= 0)
            
        {
            // the gun shot direction should move according to the enemy's movement
            
            this->moveAhead(10);
            this->turnGunRight(fabsf(angle));
        }
        else
        {
            this->turnGunLeft(fabsf(angle));
        }
        this->shoot();
    }
    else if (2.0f > this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f)
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

void CookieRobotCpp::searching()
{
    switch (this->actionIndex % 4)
    {
        case 0: this->moveAhead(100); break;
        case 1: this->turnRobotLeft(20); break;
        case 2: this->moveAhead(50); break;
        case 3: this->turnRobotRight(20); break;
    }
    this->actionIndex++;
}

void CookieRobotCpp::escaping()
{
    
}

void CookieRobotCpp::setCurrentState(CookieRobotCppAction::CookieRobotCppAction newState)
{
    this->currentState = newState;
    
    this->actionIndex = 0;
}
