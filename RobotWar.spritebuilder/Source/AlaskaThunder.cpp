//
//  AlaskaThunder.cpp
//  RobotWar
//
//  Created by Shota Oiyama on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include <iostream>
#include "AlaskaThunder.h"
#include <math.h>

AlaskaThunderCpp::AlaskaThunderCpp()
{
    this->currentState = AlaskaThunderCppAction::DEFAULT;
}

void AlaskaThunderCpp::run()
{
    auto dimentions = this->arenaDimensions();
    float distance;
    int rnd;
    
    while (true)
    {
        switch (this->currentState)
        {
            case AlaskaThunderCppAction::DEFAULT:
            
            this->shoot();
            this->moveBack(35);
            this->shoot();
            this->turnGunRight(90);
            this->turnRobotLeft(90);
            this->currentState = AlaskaThunderCppAction::FIRING;
            break;
            
            case AlaskaThunderCppAction::FIRING:
            
            rnd = rand() % 3 + 3;
            distance = (dimentions.height - this->position().y - this->robotBoundingBox().size.height / 2) / rnd;
            std::cout << "rnd:" << rnd << ", dist:" << distance << std::endl;
            for (int i = 0; i < rnd; i++) {
                this->moveAhead(distance);
                this->shoot();
            }
            
            rnd = rand() % 3 + 3;
            distance = (this->position().y - this->robotBoundingBox().size.height / 2) / rnd;
            std::cout << "rnd:" << rnd << ", dist:" << distance << std::endl;
            for (int i = 0; i < rnd; i++) {
                this->moveBack(distance);
                this->shoot();
            }
            
            break;
            
            case AlaskaThunderCppAction::SEARCHING:
            break;
            
            case AlaskaThunderCppAction::TURN_AROUND:
            break;
        }
    }
}

void AlaskaThunderCpp::gotHit()
{
    /*
    this->shoot();
    this->turnRobotLeft(45);
    this->moveAhead(100);
     */
}

void AlaskaThunderCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    /*
    if (this->currentState != AlaskaThunderCppAction::TURN_AROUND)
    {
        this->cancelActiveAction();
        
        AlaskaThunderCppAction::AlaskaThunderCppAction previousState = this->currentState;
        this->currentState = AlaskaThunderCppAction::TURN_AROUND;
        
        
        if (hitAngle >= 0)
        {
            this->turnRobotLeft(fabs(hitAngle));
        }
        else
        {
            this->turnRobotRight(fabs(hitAngle));
        }
        
        this->moveAhead(20);
        
        this->currentState = previousState;
    }
     */
}

void AlaskaThunderCpp::bulletHitEnemy(RWVec enemyPosition)
{
}

void AlaskaThunderCpp::scannedRobotAtPosition(RWVec position)
{
    /*
    if (this->currentState == AlaskaThunderCppAction::FIRING)
    {
        this->cancelActiveAction();
    }
    
    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    this->currentState = AlaskaThunderCppAction::FIRING;
     */
    
}