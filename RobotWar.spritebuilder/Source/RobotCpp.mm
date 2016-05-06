//
//  RobotCpp.cpp
//  RobotWar
//
//  Created by Daniel Haaser on 7/5/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#include "RobotCpp.h"
#include "RobotWrapper.h"


#pragma mark -
#pragma mark Public Interface

RobotCpp::RobotCpp()
{
    this->robot = 0;
}

void RobotCpp::setObjCRobot(RobotWrapper *pRobot)
{
    this->robot = pRobot;
}

#pragma mark -
#pragma mark Robot Commands

void RobotCpp::turnGunLeft(int degrees)
{
    if (this->robot)
    {
        [this->robot turnGunLeft:degrees];
    }
}

void RobotCpp::turnGunRight(int degrees)
{
    if (this->robot)
    {
        [this->robot turnGunRight:degrees];
    }
}

void RobotCpp::turnRobotLeft(int degrees)
{
    if (this->robot)
    {
        [this->robot turnRobotLeft:degrees];
    }
}

void RobotCpp::turnRobotRight(int degrees)
{
    if (this->robot)
    {
        [this->robot turnRobotRight:degrees];
    }
}

void RobotCpp::moveAhead(int distance)
{
    if (this->robot)
    {
        [this->robot moveAhead:distance];
    }
}

void RobotCpp::moveBack(int distance)
{
    if (this->robot)
    {
        [this->robot moveBack:distance];
    }
}

void RobotCpp::shoot()
{
    if (this->robot)
    {
        [this->robot shoot];
    }
}

void RobotCpp::cancelActiveAction()
{
    if (this->robot)
    {
        [this->robot cancelActiveAction];
    }
}

int RobotCpp::hitPoints()
{
    int hp = -1;
    
    if (this->robot)
    {
        hp = (int) [this->robot hitPoints];
    }
    
    return hp;
}

RWRect RobotCpp::robotBoundingBox()
{
    RWRect retRect;
    
    if (this->robot)
    {
        CGRect rect = [this->robot robotBoundingBox];
        
        retRect.origin = RWVec(rect.origin.x, rect.origin.y);
        retRect.size = RWSize(rect.size.width, rect.size.height);
        
        printf("width : %f", rect.size.width);
        printf("height : %f", rect.size.height);
    }
    
    return retRect;
}

RWVec RobotCpp::position()
{
    RWVec pos;
    
    if (this->robot)
    {
        CGPoint position = [this->robot position];
        
        pos = RWVec(position.x, position.y);
    }
    
    return pos;
}

RWVec RobotCpp::headingDirection()
{
    RWVec headingDirection;
    
    if (this->robot)
    {
        CGPoint heading = [this->robot headingDirection];
        
        headingDirection = RWVec(heading.x, heading.y);
    }
    
    return headingDirection;
}

float RobotCpp::angleBetweenHeadingDirectionAndWorldPosition(RWVec position)
{
    float retAngle = 0.0f;
    
    if (this->robot)
    {
        CGPoint pPoint;
        
        pPoint.x = position.x;
        pPoint.y = position.y;
        
        retAngle = [this->robot angleBetweenHeadingDirectionAndWorldPosition:pPoint];
    }
    
    return retAngle;
}

RWVec RobotCpp::gunHeadingDirection()
{
    RWVec retHeading;
    
    if (this->robot)
    {
        CGPoint heading = [this->robot gunHeadingDirection];
        
        retHeading = RWVec(heading.x, heading.y);
    }
    
    return retHeading;
}

float RobotCpp::angleBetweenGunHeadingDirectionAndWorldPosition(RWVec position)
{
    float retAngle = 0.0f;
    
    if (this->robot)
    {
        CGPoint pPoint;
        
        pPoint.x = position.x;
        pPoint.y = position.y;
        
        retAngle = [this->robot angleBetweenGunHeadingDirectionAndWorldPosition:pPoint];
    }
    
    return retAngle;
}

float RobotCpp::currentTimestamp()
{
    float currentTimestamp = 0.0f;
    
    if (this->robot)
    {
        currentTimestamp = [this->robot currentTimestamp];
    }
    
    return currentTimestamp;
}

RWSize RobotCpp::arenaDimensions()
{
    RWSize arenaDimensions;
    
    if (this->robot)
    {
        CGSize size = [this->robot arenaDimensions];
        
        arenaDimensions = RWSize(size.width, size.height);
    }
    
    return arenaDimensions;
}