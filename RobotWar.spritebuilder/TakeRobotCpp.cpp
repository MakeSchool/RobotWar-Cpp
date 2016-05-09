//
//  TakeRobotCpp.cpp
//  RobotWar
//
//  Created by Takehiro Yoshihama on 2016/05/06.
//  Copyright © 2016年 MakeGamesWithUs. All rights reserved.
//

#include "TakeRobotCpp.hpp"

TakeRobotCpp::TakeRobotCpp() {
}

void TakeRobotCpp::run()
{
    this->shoot();
    while (true)
        this->turnGunLeft(180);
}

void TakeRobotCpp::scannedRobotAtPosition(RWVec enemyPosition)
{
    float angle;
    this->cancelActiveAction();
    this->epos = enemyPosition;
    this->lpos = this->gunHeadingDirection();
    angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->epos);
    if (angle >= 0)
        this->turnGunRight(angle);
    else
        this->turnGunLeft(-angle);
    this->shoot();
}

void TakeRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    float angle;
    this->cancelActiveAction();
    this->epos = enemyPosition;
    angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->epos);
    if (angle >= 0)
        this->turnGunRight(angle);
    else
        this->turnGunLeft(-angle);
    this->shoot();
}

void TakeRobotCpp::gotHit()
{
    this->cancelActiveAction();
    this->moveAhead(100);
}

void TakeRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    this->cancelActiveAction();
    switch (side)
    {
        case RobotWallHitSide::FRONT:
            this->turnRobotRight(180);
            this->moveAhead(80);
            break;
            
        case RobotWallHitSide::REAR:
            this->moveAhead(80);
            break;
            
        case RobotWallHitSide::LEFT:
            this->turnRobotRight(90);
            this->moveAhead(80);
            break;
            
        case RobotWallHitSide::RIGHT:
            this->turnRobotLeft(90);
            this->moveAhead(80);
            break;
            
        case RobotWallHitSide::NONE:
            break;
     }
}