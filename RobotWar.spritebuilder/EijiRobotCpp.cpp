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
    scannedCount = 0;
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
        myMove(1000);
        scannedCount = 0;
    }
}


void EijiRobotCpp::gotHit()
{
}


void EijiRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    scannedCount -= 2;
    RWVec myVec = this->position();
    
    if(isNeedTurn(myVec, enemyPosition))
    {
        myTurn();
        return;
    }
    if(isNeedShoot(myVec, enemyPosition))
    {
        this->cancelActiveAction();
        shoot();
        return;
    }
    
}

void EijiRobotCpp::scannedRobotAtPosition(RWVec position)
{
    scannedCount++;
    RWVec myVec = this->position();
    
    if(scannedCount > 4)
        return;
    
    if(isNeedTurn(myVec, position))
    {
        myTurn();
        return;
    }
       
    if(isNeedShoot(myVec,position))
    {
        this->cancelActiveAction();
        shoot();
        return;
    }
    
}

bool EijiRobotCpp::isNeedShoot(RWVec myVec, RWVec enemyVec)
{
    
    if((myVec.x > (enemyVec.x - 20)) && (myVec.x < (enemyVec.x + 20)))
    {
        return true;
    }
    if((myVec.y > (enemyVec.y - 20)) && ((myVec.y < (enemyVec.y + 20))))
    {
        return true;
    }
    return false;
}

bool EijiRobotCpp::isNeedTurn(RWVec myVec, RWVec enemyVec)
{
    switch(nowMovement)
    {
        case Movement::UP:
            if(myVec.y > (enemyVec.y + 20))
                return true;
            break;
        case Movement::DOWN:
            if(myVec.y < (enemyVec.y - 20))
                return true;
            break;
        case Movement::LEFT:
            if(myVec.x < (enemyVec.x - 20))
                return true;
            break;
        case Movement::RIGHT:
            if(myVec.x > (enemyVec.x + 20))
                return true;
            break;
        case Movement::STOP:
        default:
            break;
    }
    return false;
}

void EijiRobotCpp::myTurn()
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
    
    myMove(10);
    isAhead = !isAhead;
}

void EijiRobotCpp::myMove(int movement)
{
    if(isAhead)
        this->moveAhead(movement);
    else
        this->moveBack(movement);
    
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
            nowMovement = Movement::RIGHT;
            return;
        case Movement::UP:
            nowMovement = Movement::LEFT;
            return;
        case Movement::LEFT:
            nowMovement = Movement::DOWN;
            return;
        case Movement::RIGHT:
            nowMovement = Movement::UP;
            return;
        case Movement::STOP:
            break;
    }
}