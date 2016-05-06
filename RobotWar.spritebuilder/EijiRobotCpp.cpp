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
    isAhead = false;
    
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
        isAhead = false;
        this->moveBack(1000);
    }
}

void EijiRobotCpp::gotHit()
{
}

void EijiRobotCpp::scannedRobotAtPosition(RWVec position)
{
    RWVec myVec = this->position();
    
    switch(nowMovement)
    {
        case Movement::DOWN:
            if(myVec.y < (position.y - 20))
            {
                myBack();
                return;
            }
            break;
        case Movement::UP:
            if(myVec.y > (position.y + 20))
            {
                myBack();
                return;
            }
            break;
        case Movement::LEFT:
            if(myVec.x < (position.x - 20))
            {
                myBack();
                return;
            }
            break;
        case Movement::RIGHT:
            if(myVec.x > (position.x + 20))
            {
                myBack();
                return;
            }
            break;
        case Movement::STOP:
            break;
    }
    
    if((myVec.x > (position.x - 20)) && (myVec.x < (position.x + 20)))
    {
        this->cancelActiveAction();
        shoot();
        return;
    }
    if((myVec.y > (position.y - 20)) && ((myVec.y < (position.y + 20))))
    {
        this->cancelActiveAction();
        shoot();
        return;
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
    
    if(isAhead)
    {
        this->moveBack(10);
        isAhead = false;
    }
    else
    {
        this->moveAhead(10);
        isAhead = true;
    }
    
}

void EijiRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    this->cancelActiveAction();
    
    if(hitWallCount == 0) {
        this->turnGunRight(90);
    }
    hitWallCount++;
    this->turnRobotLeft(90);
            
    switch(nowMovement)
    {
        case Movement::DOWN:
            nowMovement = Movement::UP;
            return;
        case Movement::UP:
            nowMovement = Movement::DOWN;
            return;
        case Movement::LEFT:
            nowMovement = Movement::RIGHT;
            return;
        case Movement::RIGHT:
            nowMovement = Movement::LEFT;
            return;
        case Movement::STOP:
            break;
    }
}