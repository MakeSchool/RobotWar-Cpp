//
//  FoeReaper4000.cpp
//  RobotWar
//
//  Created by Liang Fan on 7/7/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#include "FoeReaper4000RobotCpp.h"
#include <random>
#include <iostream>

FoeReaper4000RobotCpp::FoeReaper4000RobotCpp()
{

}

//Size of the robot 50*35
//bullet speed :400 pix /second
void FoeReaper4000RobotCpp::run()
{
    RWRect robotBox = robotBoundingBox();
    while (true)
    {
        this->moveAhead(80);
        this->turnRobotRight(20);
        this->moveAhead(100);
        this->shoot();
        this->turnRobotLeft(10);
    }
}

void FoeReaper4000RobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    this->cancelActiveAction();
    
    switch (side)
    {
        case RobotWallHitSide::FRONT:
            this->turnRobotRight(180);
            this->moveAhead(20);
            break;
            
        case RobotWallHitSide::REAR:
            this->moveAhead(80);
            break;
            
        case RobotWallHitSide::LEFT:
            this->turnRobotRight(90);
            this->moveAhead(20);
            break;
            
        case RobotWallHitSide::RIGHT:
            this->turnRobotLeft(90);
            this->moveAhead(20);
            break;
            
        case RobotWallHitSide::NONE:
            break;
    }
}
#pragma mark -
#pragma makr CallBack


//if got hit, make a random move
void FoeReaper4000RobotCpp::gotHit() {
    //TODO: if enemy is in the area of scanner, do not move to enemy?
    
    randomWalk();
    
    
}



#pragma mark -
#pragma mark Util Func

//int beginDegree = 0, int endDegree = 360, int beginDistance = 0, int EndDistance = 100
void FoeReaper4000RobotCpp::randomWalk(int beginDegree, int endDegree, int beginDistance, int endDistance)
{
    int nextDegree = generateRandomNumber(beginDegree, endDegree);
    int nextDistance = generateRandomNumber(beginDistance, endDistance);
    if () {
        <#statements#>
    }
    this->turnRobotLeft(nextDegree);
    this->moveAhead(nextDistance);
}

bool HitThe
int FoeReaper4000RobotCpp::generateRandomNumber(int begin, int end)
{
    std::default_random_engine dre;
    std::uniform_real_distribution<int> dr(begin, end);
    return dr(dre);
}

//moving it with minimize cost
void FoeReaper4000RobotCpp::optimizeMove(int degree, int distance)
{
    switch (degree/90) {
        case 0:
        {
            this->turnRobotRight(degree);
            this->moveAhead(distance);
        }
            break;
        case 1:
        {
            this->turnRobotLeft(180 - degree);
            this->moveBack(distance);
        }
            break;
        case 2:
        {
            this->turnRobotRight(degree - 180 );
            this->moveBack(distance);
        }
            break;
        case 3:
        {
            this->turnRobotLeft(360 - degree);
            this->moveAhead(distance);
        }
            break;
    }
}
