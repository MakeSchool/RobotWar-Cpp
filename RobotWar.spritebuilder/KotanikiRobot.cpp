//
//  KotanikiRobot.cpp
//  RobotWar
//
//  Created by Yuki Kotani on 2016/05/06.
//  Copyright © 2016年 MakeGamesWithUs. All rights reserved.
//

#include "KotanikiRobot.hpp"
#include <math.h>

KotanikiRobot::KotanikiRobot(){
    this->currentState = KotanikiRobotAction::FIRST_MOVE;
}

void KotanikiRobot::run()
{
    while (true)
    {
        while (this->currentState == KotanikiRobotAction::FIRING)
        {
            if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f)
            {
                this->setCurrentState(KotanikiRobotAction::SEARCHING);
                this->gunHeadingDirection();
            }
            float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
            
            if (angle >= 0)
            {
                this->turnGunRight(fabsf(angle));
            }
            else
            {
                this->turnGunLeft(fabsf(angle));
            }
            
            this->shoot();
        }
        
        while (this->currentState == KotanikiRobotAction::SEARCHING)
        {
            //if (this->currentState == KotanikiRobotAction::SEARCHING)
            //   this->turnGunRight(this->angleBetweenGunHeadingDirectionAndWorldPosition(this->headingDirection()));
            if (this->currentState == KotanikiRobotAction::SEARCHING)this->shoot();
            if (this->currentState == KotanikiRobotAction::SEARCHING)this->turnGunRight(45);
            if (this->currentState == KotanikiRobotAction::SEARCHING)this->shoot();
            if (this->currentState == KotanikiRobotAction::SEARCHING)this->turnGunRight(45);
            if (this->currentState == KotanikiRobotAction::SEARCHING)this->gunHeadingDirection();
            if (this->currentState == KotanikiRobotAction::SEARCHING)this->shoot();
            if (this->currentState == KotanikiRobotAction::SEARCHING)this->turnGunLeft(45);
            if (this->currentState == KotanikiRobotAction::SEARCHING)this->shoot();
            if (this->currentState == KotanikiRobotAction::SEARCHING)this->turnGunLeft(45);
        }
        
        while (this->currentState == KotanikiRobotAction::FIRST_MOVE || this->currentState == KotanikiRobotAction::SECOND_MOVE)
        {
            this->moveBack(100);
        }
    }
}

void KotanikiRobot::gotHit()
{
    
}


void KotanikiRobot::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
// 壁にぶつかった際の処理
{
    this->cancelActiveAction();
    
    if (this->currentState == KotanikiRobotAction::FIRST_MOVE) {
        this->turnRobotLeft(90);
        this->setCurrentState(KotanikiRobotAction::SECOND_MOVE);
    }else if (this->currentState == KotanikiRobotAction::SECOND_MOVE) {
        this->setCurrentState(KotanikiRobotAction::SEARCHING);
    }else {
    
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
}

void KotanikiRobot::scannedRobotAtPosition(RWVec position)
{
    if (this->currentState != KotanikiRobotAction::FIRING)
    {
        this->cancelActiveAction();
    }
        
    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
        
    this->setCurrentState(KotanikiRobotAction::FIRING);
        
}

void KotanikiRobot::setCurrentState(KotanikiRobotAction::KotanikiRobotAction newState)
{
    this->currentState = newState;
    
    //this->actionIndex = 0;
}