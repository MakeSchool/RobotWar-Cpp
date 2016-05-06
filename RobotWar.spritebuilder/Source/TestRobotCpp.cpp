//
//  TestRobotCpp.cpp
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#include "TestRobotCpp.h"

void TestRobotCpp::run()
{
    while (true)
    {
        this->moveAhead(80);
        this->turnRobotRight(20);
        this->moveAhead(100);
        this->shoot();
        this->turnRobotLeft(10);
    }
}

void TestRobotCpp::gotHit()
{
    this->shoot();
    this->turnRobotLeft(45);
    this->moveAhead(100);
}

void TestRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
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