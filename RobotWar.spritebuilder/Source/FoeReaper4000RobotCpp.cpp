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
    lastEnemyPos.x = -1;
    lastEnemyPos.y = -1;
    keepStay = false;
}

//Size of the robot 50*35
//bullet speed :400 pix /second

#pragma mark -
#pragma mark CallBack

void FoeReaper4000RobotCpp::run()
{
    while (true)
    {
        randomWalk();
    }
}

void FoeReaper4000RobotCpp::scannedRobotAtPosition(RWVec position)
{
    this->cancelActiveAction();
    lastEnemyPos = position;
    keepStay = true;
    randomWalk();

}
void FoeReaper4000RobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    this->cancelActiveAction();
    lastEnemyPos = enemyPosition;
    keepStay = true;
    randomWalk();
}

void FoeReaper4000RobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    this->cancelActiveAction();
    RWSize areaSize = arenaDimensions();
    switch (side)
    {
        case RobotWallHitSide::FRONT:
        {
            float x = generateRandomNumber(areaSize.width/4,areaSize.width/4*3);
            float y = 0.0f;
            int angle = (int) angleBetweenHeadingDirectionAndWorldPosition(RWVec(x, y));
            randomWalk(angle,angle,150,200);
        }
            break;
            
        case RobotWallHitSide::REAR:
        {
            float x = generateRandomNumber(areaSize.width/4,areaSize.width/4*3);
            float y = areaSize.height;
            int angle = (int) angleBetweenHeadingDirectionAndWorldPosition(RWVec(x, y));
            randomWalk(angle,angle,150,200);

        }
            break;
            
        case RobotWallHitSide::LEFT:
        {
            float x = 0.0;
            float y = generateRandomNumber(areaSize.height/4, areaSize.height/4*3);
            int angle = (int) angleBetweenHeadingDirectionAndWorldPosition(RWVec(x, y));
            randomWalk(angle,angle,150,200);

        }
            break;
        case RobotWallHitSide::RIGHT:
        {
            float x = areaSize.width;
            float y = generateRandomNumber(areaSize.height/4, areaSize.height/4*3);
            int angle = (int) angleBetweenHeadingDirectionAndWorldPosition(RWVec(x, y));
            randomWalk(angle,angle,150,200);

        }
            break;
      
        case RobotWallHitSide::NONE:
            break;
    }
}


//if got hit, make a random move
void FoeReaper4000RobotCpp::gotHit() {
    //TODO: if enemy is in the area of scanner, do not move to enemy?
   keepStay = false;
   randomWalk(0,360,150,200);
    
}



#pragma mark -
#pragma mark Util Func

//int beginDegree = 0, int endDegree = 360, int beginDistance = 50, int EndDistance = 100
void FoeReaper4000RobotCpp::randomWalk(int beginDegree, int endDegree, int beginDistance, int endDistance)
{
    int nextDistance = generateRandomNumber(beginDistance, endDistance);
    int nextDegree = generateRandomNumber(beginDegree, endDegree);
    if (! keepStay) {
        optimizeMove(nextDegree,nextDistance);
    }
    optimizeGunPosition();
}

int FoeReaper4000RobotCpp::generateRandomNumber(int begin, int end)
{
    std::default_random_engine dre;
    std::uniform_int_distribution<int> dr(begin, end);
    return dr(dre);
}
float FoeReaper4000RobotCpp::generateRandomNumber(float begin, float end)
{
    std::default_random_engine dre;
    std::uniform_real_distribution<float> dr(begin, end);
    return dr(dre);
}
// move the gun base on the scan
//if not move to the side which has more space
void FoeReaper4000RobotCpp::optimizeGunPosition()
{
    //TODO: check if scanner find the enemy
    if (keepStay) {
        shootToPos(lastEnemyPos);
        return;
    }
    
    RWVec robotPos = position();
    RWSize areaSize = arenaDimensions();
    float x,y;
    if (robotPos.x > (areaSize.width - robotPos.x)) {
        x = generateRandomNumber(0.0f, robotPos.x);
    } else {
        x = generateRandomNumber(robotPos.x, areaSize.width);
    }
    
    if (robotPos.y > (areaSize.height - robotPos.y)) {
        y = generateRandomNumber(0.0f, robotPos.y);
    } else {
        y = generateRandomNumber(robotPos.y, areaSize.height);
    }

    shootToPos(RWVec(x, y));
}
void FoeReaper4000RobotCpp::shootToPos(RWVec pos)
{
    float angle = angleBetweenGunHeadingDirectionAndWorldPosition(pos);
    if (angle < 0) {
        this->turnGunLeft((int) -angle);
    } else {
        this->turnGunRight((int) angle);
    }
    this->shoot();
}

//moving it with minimize cost
void FoeReaper4000RobotCpp::optimizeMove(int degree, int distance)
{

    if (degree < 0) {
        degree = degree + 360;
    }
    switch (degree/90) {
        case 0:
        {
            this->moveAhead(distance);
            this->turnRobotRight(degree);
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
