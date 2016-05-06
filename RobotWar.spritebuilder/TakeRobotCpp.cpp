//
//  TakeRobotCpp.cpp
//  RobotWar
//
//  Created by Takehiro Yoshihama on 2016/05/06.
//  Copyright © 2016年 MakeGamesWithUs. All rights reserved.
//

#include "TakeRobotCpp.hpp"

void TakeRobotCpp::run()
{
    while (true)
    {
        //RWVec vec = this->position();
        this->turnGunLeft(180);
        //this->moveAhead(80);
        //this->turnRobotRight(20);
        //this->moveAhead(100);
        //this->shoot();
        //this->turnRobotLeft(10);
    }
}

void TakeRobotCpp::scannedRobotAtPosition(RWVec enemyPosition)
{
    this->cancelActiveAction();
    this->epos = enemyPosition;
    this->lpos = this->gunHeadingDirection();
    //this->
    this->shoot();
}

void TakeRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    this->epos = enemyPosition;
    this->moveBack(10);
}


void TakeRobotCpp::gotHit()
{
    this->moveAhead(10);
    //this->ldegree = this->angleBetweenGunHeadingDirectionAndWorldPosition(<#RWVec position#>);
    //this->shoot();
    //this->
    //this->turnRobotLeft(45);
    //this->moveAhead(100);
}

void TakeRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    /*
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
     */
}