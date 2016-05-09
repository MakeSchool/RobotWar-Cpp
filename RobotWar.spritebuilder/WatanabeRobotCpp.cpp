//
//  WatanabeRobotCpp.cpp
//  RobotWar
//
//  Created by Shintaro Watanabe on 2016/05/06.
//  Copyright © 2016年 MakeGamesWithUs. All rights reserved.
//

#include "WatanabeRobotCpp.hpp"
#include <math.h>

WatanabeRobotCpp::WatanabeRobotCpp()
{
    this->currentState = WatanabeRobotState::DEFAULT;
    this->currentVectorState = vectorState::Left;
    this->lastVectorState = vectorState::Left;
}

void WatanabeRobotCpp::run()
{
    while (true)
    {
        switch (this->currentState) {
            case WatanabeRobotState::DEFAULT:
                defaultAction();
                break;
            case WatanabeRobotState::FIRING:
                firingAction();
                break;
            case WatanabeRobotState::SEARCHING:
                searchAction();
                break;
        }
    }
}

void WatanabeRobotCpp::defaultAction()
{
    if (this->currentVectorState == this->lastVectorState)
    {
        this->moveBack(999999);
    }
    else
    {
        this->moveAhead(999999);
        this->lastVectorState = this->currentVectorState;
    }
}

void WatanabeRobotCpp::searchAction()
{
    if (this->currentVectorState == this->lastVectorState)
    {
        
        this->moveBack(999999);
    }
    else
    {
        this->moveAhead(999999);
        this->lastVectorState = this->currentVectorState;
    }
    RWVec center = RWVec(this->arenaDimensions().width / 2, this->arenaDimensions().height / 2);
    float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(center);
    if (angle >= 0)
    {
        this->turnGunRight(fabsf(angle));
    }
    else
    {
        this->turnGunLeft(fabsf(angle));
    }
}

void WatanabeRobotCpp::firingAction()
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
    this->bulletCount++;
    if (bulletCount > 2)
    {
        this->currentState = WatanabeRobotState::SEARCHING;
        if (this->currentVectorState == vectorState::Left)
        {
            this->currentVectorState = vectorState::Right;
        }
        else
        {
            this->currentVectorState = vectorState::Left;
        }
    }
}

void WatanabeRobotCpp::gotHit()
{
    if (this->currentState != WatanabeRobotState::SEARCHING)
    {
        this->cancelActiveAction();
        this->currentState = WatanabeRobotState::SEARCHING;
    }
}

void WatanabeRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
}

void WatanabeRobotCpp::scannedRobotAtPosition(RWVec position)
{
    if (this->currentState != WatanabeRobotState::FIRING)
    {
        this->cancelActiveAction();
        this->currentState = WatanabeRobotState::FIRING;
    }
    this->lastKnownPosition = position;
}

void WatanabeRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    if (this->currentState != WatanabeRobotState::SEARCHING)
    {
        this->currentState = WatanabeRobotState::SEARCHING;
    }
    this->cancelActiveAction();
    switch (side) {
        case RobotWallHitSide::REAR:
            if (lastVectorState == vectorState::Left)
            {
                this->turnRobotRight(90);
            }
            else
            {
                this->turnRobotLeft(90);
            }
            break;
        case RobotWallHitSide::FRONT:
            if (lastVectorState == vectorState::Left)
            {
                this->turnRobotLeft(90);
            }
            else
            {
                this->turnRobotRight(90);
            }
            break;
        default:
            break;
    }
}