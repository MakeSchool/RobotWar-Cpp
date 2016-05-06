//
//  Tokutya.cpp
//  RobotWar
//
//  Created by Masahiro Ito on 2016/05/06.
//  Copyright © 2016年 MakeGamesWithUs. All rights reserved.
//

#include "Tokutya.hpp"
#include <math.h>


Tokutya::Tokutya()
{
    this->currentState = TokutyaAction::DEFAULT;
}

void Tokutya::run()
{
    while (true)
    {
    
    }
}

void Tokutya::gotHit()
{
    this->shoot();
    this->turnRobotLeft(45);
    this->moveAhead(100);
}

void Tokutya::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    if (this->currentState != TokutyaAction::TURN_AROUND)
    {
        this->cancelActiveAction();
        
        TokutyaAction::TokutyaAction previousState = this->currentState;
        this->currentState = TokutyaAction::TURN_AROUND;
        
        
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
}

void Tokutya::bulletHitEnemy(RWVec enemyPosition)
{
    
}

void Tokutya::scannedRobotAtPosition(RWVec position)
{
    if (this->currentState != TokutyaAction::FIRING)
    {
        this->cancelActiveAction();
    }
    
    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    this->currentState = TokutyaAction::FIRING;
    
}