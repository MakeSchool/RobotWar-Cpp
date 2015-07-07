//
//  RobotTARS.cpp
//  RobotWar
//
//  Created by Mitsushige Komiya on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <string>
#include <random>

#include "RobotTARS.h"

#define DEBUG_MODE 0

#if DEBUG_MODE == 1
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) {}
#endif

#pragma mark -
#pragma mark Lifecycle

RobotTARS::RobotTARS()
{
    this->changeState(RobotTARSAction::INITIAL);
    this->damagedPlayer       = 0;
    this->damagedEnemy        = 0;
    this->lastGotHitTimestamp = this->currentTimestamp();
    this->isTargetedByEnemy   = false;
}

#pragma mark -
#pragma mark Robot Delegate Methods

void RobotTARS::run()
{
    while (true)
    {
        switch (this->currentState) {
            case RobotTARSAction::INITIAL:             this->runInitial();          break;
            case RobotTARSAction::OFFENSE_FIRING:      this->runOffenseFiring();    break;
            case RobotTARSAction::OFFENSE_SEARCHING:   this->runOffenseSearching(); break;
            case RobotTARSAction::OFFENSE_TURN_AROUND: break;
            case RobotTARSAction::ASSAULT_FIRING:      this->runAssaultFiring();    break;
            case RobotTARSAction::DEBUGGING:           this->runDebugging();        break;
        }
        
        if (this->currentTimestamp() - this->lastGotHitTimestamp > 1.0f)
        {
            this->isTargetedByEnemy = false;
        }
    }
}

void RobotTARS::gotHit()
{
    DEBUG_PRINT("got hit!\n");
    this->damagedPlayer++;
    this->isTargetedByEnemy = true;
    
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f)
    {
        this->changeState(RobotTARSAction::OFFENSE_SEARCHING);
    }
    else
    {
        float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
        this->turnGun(angle);
        this->shoot();
        this->shoot();
        this->shoot();
        
        this->changeState(RobotTARSAction::OFFENSE_SEARCHING);
    }
}

void RobotTARS::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    DEBUG_PRINT("hit wall!\n");
    
    if (this->currentState != RobotTARSAction::OFFENSE_TURN_AROUND)
    {
        this->cancelActiveAction();
        
        RobotTARSAction::RobotTARSAction previousState = this->currentState;
        this->changeState(RobotTARSAction::OFFENSE_TURN_AROUND);

        if (hitAngle >= 0)
        {
            this->turnRobotLeft(fabs(hitAngle));
        }
        else
        {
            this->turnRobotRight(fabs(hitAngle));
        }
        this->moveAhead(20);
        
        this->changeState(previousState);
    }
}

void RobotTARS::bulletHitEnemy(RWVec enemyPosition)
{
    DEBUG_PRINT("bullet hit enemy!\n");
    
    this->damagedEnemy++;
    DEBUG_PRINT("damaged: %d\n", this->damagedEnemy);

    if (this->currentState == RobotTARSAction::OFFENSE_SEARCHING)
    {
        this->cancelActiveAction();
    }
    
    this->lastKnownPosition = enemyPosition;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    
    if (this->isWinningLife() ||
        (this->currentState == RobotTARSAction::OFFENSE_SEARCHING && !this->isTargetedByEnemy))
    {
        this->cancelActiveAction();
        this->changeState(RobotTARSAction::ASSAULT_FIRING);
    }
    else if (this->currentState != RobotTARSAction::OFFENSE_FIRING)
    {
        this->changeState(RobotTARSAction::OFFENSE_FIRING);
    }
}

void RobotTARS::scannedRobotAtPosition(RWVec position)
{
    DEBUG_PRINT("scanned enemy!\n");
    
    if (this->currentState != RobotTARSAction::OFFENSE_FIRING ||
        this->currentState != RobotTARSAction::ASSAULT_FIRING)
    {
        this->cancelActiveAction();
    }
    
    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    
    if (this->currentState != RobotTARSAction::ASSAULT_FIRING) {
        this->changeState(RobotTARSAction::OFFENSE_FIRING);
    }
}

#pragma mark -
#pragma mark Private Methods

#pragma mark Running Methods

void RobotTARS::runInitial()
{
    this->shoot();
    this->moveAhead(500);
}

void RobotTARS::runOffenseFiring()
{
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f)
    {
        this->turnGunToAhead();
        this->changeState(RobotTARSAction::OFFENSE_SEARCHING);
    }
    else
    {
        float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
        this->turnGun(angle);
        this->shoot();
    }
}

void RobotTARS::runOffenseSearching()
{
    this->moveAhead(50);
    this->turnRobotLeft(40);
    this->shoot();
    this->moveAhead(50);
    this->turnRobotRight(40);
}

void RobotTARS::runAssaultFiring()
{
    DEBUG_PRINT("ASSAULT!\n");
    
    float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
    this->turnGun(angle);
    this->shoot();
    this->shoot();
    this->shoot();
    
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f) {
        this->changeState(RobotTARSAction::OFFENSE_SEARCHING);
    }
}

void RobotTARS::runDebugging()
{
}

#pragma mark Action Helper Methods

void RobotTARS::turnGunToAhead()
{
    RWVec gunDirection = this->gunHeadingDirection();
    RWVec robotDirection = this->headingDirection();
    
    float lengthGunDirection   = this->getVectorLenght(gunDirection);
    float lengthRobotDirection = this->getVectorLenght(robotDirection);
    float dotProduct = this->getDotProduct(gunDirection, robotDirection);
    float cosSita    = dotProduct / (lengthGunDirection * lengthRobotDirection);
    float sita       = this->convertRadianToDigree(acosf(cosSita));
    
    DEBUG_PRINT("cos sita: %f\n", cosSita);
    DEBUG_PRINT("sita: %f\n", sita);
    
    if (isnan(sita)) {
        return ;
    }
    
    if (cosSita >= 0.0f) {
        this->turnGunRight(fabsf(sita));
    } else {
        this->turnGunLeft(fabsf(sita));
    }
}

void RobotTARS::turnGun(float angle)
{
    if (angle >= 0)
    {
        this->turnGunRight(fabsf(angle));
    }
    else
    {
        this->turnGunLeft(fabsf(angle));
    }
}

void RobotTARS::turnRobot(float angle)
{
    if (angle >= 0)
    {
        this->turnRobotRight(fabsf(angle));
    }
    else
    {
        this->turnRobotLeft(fabsf(angle));
    }
}

bool RobotTARS::isWinningLife()
{
    return (this->damagedEnemy > this->damagedPlayer);
}

void RobotTARS::changeState(RobotTARSAction::RobotTARSAction nextState)
{
    this->currentState = nextState;
    
    std::string nextStateStr;
    switch (this->currentState) {
        case RobotTARSAction::INITIAL:             nextStateStr = "INITIAL";             break;
        case RobotTARSAction::OFFENSE_FIRING:      nextStateStr = "OFFENSE_FIRING";      break;
        case RobotTARSAction::OFFENSE_SEARCHING:   nextStateStr = "OFFENSE_SEARCHING";   break;
        case RobotTARSAction::OFFENSE_TURN_AROUND: nextStateStr = "OFFENSE_TURN_AROUND"; break;
        case RobotTARSAction::ASSAULT_FIRING:      nextStateStr = "ASSAULT!!";           break;
        case RobotTARSAction::DEBUGGING:           nextStateStr = "DEBUGGING";           break;
    }
    
    DEBUG_PRINT("change state: %s\n", nextStateStr.c_str());
}

#pragma mark Math Helper Methods

float RobotTARS::getVectorLenght(RWVec vec)
{
    return pow((vec.x * vec.x) + (vec.y * vec.y), 0.5);
}

float RobotTARS::getDotProduct(RWVec vec1, RWVec vec2)
{
    return vec1.x * vec2.x + vec2.y * vec1.y;
}

float RobotTARS::convertRadianToDigree(float radian)
{
    return radian * 180.0f / M_PI;
}