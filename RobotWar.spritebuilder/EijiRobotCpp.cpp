//
//  EijiRobotCpp.cpp
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#include "EijiRobotCpp.hpp"
#include <math.h>

//static const float GUN_ANGLE_TOLERANCE = 2.0f;

EijiRobotCpp::EijiRobotCpp()
{
    hitWallCount = 0;
    
    RWSize size = this->arenaDimensions();
    RWVec vec = this->position();
    
    if(vec.x < (size.width / 2))
    {
        nowMovement = Movement::LEFT;
    }
    else
    {
        nowMovement = Movement::RIGHT;
    }
    
}

void EijiRobotCpp::run()
{
    while (true)
    {
        this->moveBack(1000);
    }
}

void EijiRobotCpp::gotHit()
{
}

void EijiRobotCpp::scannedRobotAtPosition(RWVec position)
{
    RWVec myVec = this->position();
    
    //自機　敵機
    if(myVec.x < (position.x - 20))
    {
        if(nowMovement == Movement::LEFT)
            myBack();
        return;
    //敵機　自機
    } else if(myVec.x > (position.x + 20))
    {
        if(nowMovement == Movement::RIGHT)
            myBack();
        return;
    } else {
        this->cancelActiveAction();
        shoot();
    }
}

void EijiRobotCpp::myBack()
{
    if(nowMovement == Movement::DOWN)
        nowMovement = Movement::UP;
    else if(nowMovement == Movement::UP)
        nowMovement = Movement::DOWN;
    else if(nowMovement == Movement::LEFT)
        nowMovement = Movement::RIGHT;
    else if(nowMovement == Movement::RIGHT)
        nowMovement = Movement::LEFT;
    
    this->cancelActiveAction();
    this->moveAhead(10);
}

void EijiRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    this->cancelActiveAction();
    
    switch (side)
    {
        case RobotWallHitSide::REAR:
        case RobotWallHitSide::FRONT:
        case RobotWallHitSide::LEFT:
        case RobotWallHitSide::RIGHT:
            
            if(hitWallCount == 0) {
                this->turnGunRight(90);
            }
            hitWallCount++;
            this->turnRobotLeft(90);
            
            break;
            
        case RobotWallHitSide::NONE:
            break;
    }
}