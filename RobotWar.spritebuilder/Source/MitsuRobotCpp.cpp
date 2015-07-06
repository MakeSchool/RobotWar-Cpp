//
//  MitsuRobotCpp.cpp
//  RobotWar
//
//  Created by Mitsushige Komiya on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <string>
#include <random>

#include "MitsuRobotCpp.h"

#define DEBUG_MODE 1

#if DEBUG_MODE == 1
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) {}
#endif

#pragma mark -
#pragma mark Lifecycle

MitsuRobotCpp::MitsuRobotCpp()
{
    this->changeState(MitsuRobotCppAction::INITIAL);
    this->damagedPlayer       = 0;
    this->damagedEnemy        = 0;
    this->lastGotHitTimestamp = this->currentTimestamp();
    this->isTargetedByEnemy   = false;
}

#pragma mark -
#pragma mark Robot Delegate Methods

void MitsuRobotCpp::run()
{
    while (true)
    {
        switch (this->currentState) {
            case MitsuRobotCppAction::INITIAL:             this->runInitial();          break;
            case MitsuRobotCppAction::OFFENSE_FIRING:      this->runOffenseFiring();    break;
            case MitsuRobotCppAction::OFFENSE_SEARCHING:   this->runOffenseSearching(); break;
            case MitsuRobotCppAction::OFFENSE_TURN_AROUND: break;
            case MitsuRobotCppAction::ASSAULT_FIRING:      this->runAssaultFiring();    break;
            case MitsuRobotCppAction::DEBUGGING:           this->runDebugging();        break;
        }
        
        if (this->currentTimestamp() - this->lastGotHitTimestamp > 1.0f)
        {
            this->isTargetedByEnemy = false;
        }
    }
}

void MitsuRobotCpp::gotHit()
{
    DEBUG_PRINT("got hit!\n");
    this->damagedPlayer++;
    this->isTargetedByEnemy = true;
    
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f)
    {
        this->turnRobotLeft(30);
        this->moveAhead(50);
        this->turnRobotLeft(20);
        this->moveAhead(50);
        this->turnRobotRight(20);
        
        this->changeState(MitsuRobotCppAction::OFFENSE_SEARCHING);
    }
    else
    {
        this->shoot();
    }
}

void MitsuRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    DEBUG_PRINT("hit wall!\n");
    
    if (this->currentState != MitsuRobotCppAction::OFFENSE_TURN_AROUND)
    {
        this->cancelActiveAction();
        
        MitsuRobotCppAction::MitsuRobotCppAction previousState = this->currentState;
        this->changeState(MitsuRobotCppAction::OFFENSE_TURN_AROUND);

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

void MitsuRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    DEBUG_PRINT("bullet hit enemy!\n");
    
    this->damagedEnemy++;
    DEBUG_PRINT("damaged: %d\n", this->damagedEnemy);

    if (this->currentState == MitsuRobotCppAction::OFFENSE_SEARCHING)
    {
        this->cancelActiveAction();
    }
    
    this->lastKnownPosition = enemyPosition;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    
    if (this->isWinningLife() ||
        (this->currentState == MitsuRobotCppAction::OFFENSE_SEARCHING && !this->isTargetedByEnemy))
    {
        this->changeState(MitsuRobotCppAction::ASSAULT_FIRING);
    }
    else if (this->currentState != MitsuRobotCppAction::OFFENSE_FIRING)
    {
        this->changeState(MitsuRobotCppAction::OFFENSE_FIRING);
    }
}

void MitsuRobotCpp::scannedRobotAtPosition(RWVec position)
{
    DEBUG_PRINT("scanned enemy!\n");
    
    if (this->currentState != MitsuRobotCppAction::OFFENSE_FIRING ||
        this->currentState != MitsuRobotCppAction::ASSAULT_FIRING)
    {
        this->cancelActiveAction();
    }
    
    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    
    if (this->currentState != MitsuRobotCppAction::ASSAULT_FIRING) {
        this->changeState(MitsuRobotCppAction::OFFENSE_FIRING);
    }
}

#pragma mark -
#pragma mark Private Methods

#pragma mark Running Methods

void MitsuRobotCpp::runInitial()
{
    this->moveAhead(500);
}

void MitsuRobotCpp::runOffenseFiring()
{
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f)
    {
        this->turnGunToAhead();
        this->changeState(MitsuRobotCppAction::OFFENSE_SEARCHING);
    }
    else
    {
        float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
        this->turnGun(angle);
        this->shoot();
    }
}

void MitsuRobotCpp::runOffenseSearching()
{
    this->moveAhead(50);
    this->turnRobotLeft(40);
    this->shoot();
    this->moveAhead(50);
    this->turnRobotRight(40);
    this->shoot();
}

void MitsuRobotCpp::runAssaultFiring()
{
    DEBUG_PRINT("ASSAULT!\n");
    
    float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
    this->turnGun(angle);
    this->shoot();
    this->shoot();
    this->shoot();
    
    this->changeState(MitsuRobotCppAction::OFFENSE_SEARCHING);
}

void MitsuRobotCpp::runDebugging()
{
}

#pragma mark Action Helper Methods

void MitsuRobotCpp::turnGunToAhead()
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

void MitsuRobotCpp::turnGun(float angle)
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

void MitsuRobotCpp::turnRobot(float angle)
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

bool MitsuRobotCpp::isWinningLife()
{
    return (this->damagedEnemy > this->damagedPlayer);
}

void MitsuRobotCpp::changeState(MitsuRobotCppAction::MitsuRobotCppAction nextState)
{
    this->currentState = nextState;
    
    std::string nextStateStr;
    switch (this->currentState) {
        case MitsuRobotCppAction::INITIAL:             nextStateStr = "INITIAL";             break;
        case MitsuRobotCppAction::OFFENSE_FIRING:      nextStateStr = "OFFENSE_FIRING";      break;
        case MitsuRobotCppAction::OFFENSE_SEARCHING:   nextStateStr = "OFFENSE_SEARCHING";   break;
        case MitsuRobotCppAction::OFFENSE_TURN_AROUND: nextStateStr = "OFFENSE_TURN_AROUND"; break;
        case MitsuRobotCppAction::ASSAULT_FIRING:      nextStateStr = "ASSAULT!!";           break;
        case MitsuRobotCppAction::DEBUGGING:           nextStateStr = "DEBUGGING";           break;
    }
    
    DEBUG_PRINT("change state: %s\n", nextStateStr.c_str());
}

#pragma mark Math Helper Methods

float MitsuRobotCpp::getVectorLenght(RWVec vec)
{
    return pow((vec.x * vec.x) + (vec.y * vec.y), 0.5);
}

float MitsuRobotCpp::getDotProduct(RWVec vec1, RWVec vec2)
{
    return vec1.x * vec2.x + vec2.y * vec1.y;
}

float MitsuRobotCpp::convertRadianToDigree(float radian)
{
    return radian * 180.0f / M_PI;
}