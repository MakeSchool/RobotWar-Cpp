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
}

void WatanabeRobotCpp::run()
{
    while (true)
    {
        switch (this->currentState) {
            case WatanabeRobotState::DEFAULT:
                break;
            case WatanabeRobotState::FIRING:
                break;
            case WatanabeRobotState::SEARCHING:
                break;
        }
    }
}

void WatanabeRobotCpp::gotHit()
{
    this->shoot();
    this->turnRobotLeft(30);
    this->moveAhead(100);
}

void WatanabeRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(enemyPosition);
    
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

void WatanabeRobotCpp::scannedRobotAtPosition(RWVec position)
{
    this->cancelActiveAction();
    float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(position);
    
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

void WatanabeRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    if (hitAngle >= 0)
    {
        this->turnRobotLeft(fabs(hitAngle));
    }
    else
    {
        this->turnRobotRight(fabs(hitAngle));
    }
    
    this->moveAhead(20);
}