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
    while (true) {
        while (this->currentState == KotanikiRobotAction::FIRING) {
            performFiring();
        }
        
        while (this->currentState == KotanikiRobotAction::SEARCHING) {
            performSerching();
        }
        
        while (this->currentState == KotanikiRobotAction::FIRST_MOVE){
            performMove();
        }
        
        while (this->currentState == KotanikiRobotAction::SEARCH_DUSH){
            moveAhead(100);
            setCurrentState(KotanikiRobotAction::FIRST_MOVE);
        }
        
        while (this->currentState == KotanikiRobotAction::HIT_TURN) {
            turnGunLeft(90);
            setCurrentState(KotanikiRobotAction::FIRST_MOVE);
        }
    }
}

void KotanikiRobot::gotHit()
{
    moveAhead(100);
    setCurrentState(KotanikiRobotAction::HIT_TURN);
}


void KotanikiRobot::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
// 壁にぶつかった際の処理
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
            this->moveBack(20);
            break;
            
        case RobotWallHitSide::RIGHT:
            this->turnRobotLeft(90);
            this->moveBack(20);
            break;
            
        case RobotWallHitSide::NONE:
            break;
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
    
    this->actionIndex = 0;
}

void KotanikiRobot::swayRight()
{
    this->turnRobotRight(15);
    this->turnRobotLeft(15);
}

void KotanikiRobot::performSerching()
{
    switch (this->actionIndex % 2) {
        case 0:
            moveAhead(300);
            break;
            
        case 1:
            turnRobotRight(90);
            break;
    }
    
    this->actionIndex++;
}

void KotanikiRobot::performMove()
{
    switch (this->actionIndex % 4)
    {
        case 1:
        {
            RWVec currentPosition = this->position();
            RWSize arenaSize = this->arenaDimensions();
            //float bodyLength = this->robotBoundingBox().size.width;
            
            swayRight();
            
            if (currentPosition.y < arenaSize.height / 2.0f)
            {
                this->moveAhead(currentPosition.x);
            }
            else {
                this->moveAhead(arenaSize.height - currentPosition.y);
            }
        }
            break;
            
        case 0:
        {
            RWVec currentPosition = this->position();
            RWSize arenaSize = this->arenaDimensions();
            
            if (currentPosition.y < arenaSize.height / 2.0f)
            {
                if (currentPosition.x < arenaSize.width / 2.0f)
                {
                    // bottom left
                    this->turnRobotRight(90);
                }
                else
                {
                    // bottom right
                    this->turnRobotLeft(90);
                }
            }
            else
            {
                if (currentPosition.x < arenaSize.width / 2.0f)
                {
                    // top left
                    this->turnRobotLeft(90);
                }
                else
                {
                    // top right
                    this->turnRobotRight(90);
                }
            }
        }
            break;
            
        case 2:
        {
            RWVec currentPosition = this->position();
            RWSize arenaSize = this->arenaDimensions();
            float bodyLength = this->robotBoundingBox().size.width;
         
        }
            break;
            
            
        case 3:
            this->shoot();
            this->setCurrentState(KotanikiRobotAction::SEARCHING);
            break;
    }
    
    this->actionIndex++;
}

void KotanikiRobot::performFiring()
{
    if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f) {
        this->setCurrentState(KotanikiRobotAction::SEARCH_DUSH);
        float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->gunHeadingDirection());
        
        if (angle >= 0) {
            this->turnGunRight(fabsf(angle));
        }else{
            this->turnGunLeft(fabsf(angle));
        }
        
    } else {
        float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);
        
        if (angle >= 0) {
            this->turnGunRight(fabsf(angle));
        } else {
            this->turnGunLeft(fabsf(angle));
        }
        
        this->shoot();
    }
}