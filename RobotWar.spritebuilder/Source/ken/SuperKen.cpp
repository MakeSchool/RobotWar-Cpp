//
//  SuperKen.cpp
//  RobotWar
//
//  Created by Ken Watanabe on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include "SuperKen.h"
#include "KMath.h"
#include <math.h>
#include <stdlib.h>

using namespace SuperKenMath;

//static const float GUN_ANGLE_TOLERANCE = 2.0f;

SuperKen::SuperKen()
{
    this->currentState = SuperKenAction::INIT;

    // unsure
    this->enemyHitPoint = this->hitPoints();
}

void SuperKen::run()
{
    auto dimentions = this->arenaDimensions();
    auto box = this->robotBoundingBox();
    RWVec position;
    float targetX, targetY;
    int rnd;
    
    while (true)
    {
        switch (this->currentState)
        {
            case SuperKenAction::INIT:
                this->currentState = SuperKenAction::SCANNING;
                position = this->position();
                targetY = (this->isUp(position)) ? dimentions.height - box.size.height / 2 : box.size.height / 2;
                this->goToPoint(RWVec(position.x, targetY));
                break;
            case SuperKenAction::VERTICAL_ESCAPE:
                this->currentState = SuperKenAction::SCANNING;
                position = this->position();
                targetY = (this->isUp(position)) ? box.size.height / 2 : dimentions.height - box.size.height / 2;
                this->goToPoint(RWVec(position.x, targetY));
                break;
            case SuperKenAction::HORIZONTAL_ESCAPE:
                this->currentState = SuperKenAction::VERTICAL_ESCAPE;
                position = this->position();
                targetX = (this->isLeft(position)) ? dimentions.width - box.size.width / 2 : box.size.width / 2;
                this->goToPoint(RWVec(targetX, position.y));
                break;
            case SuperKenAction::FIRING:
                if (this->currentTimestamp() - this->timeSinceLastEnemyHit > 2.5f)
                {
                    this->cancelActiveAction();
                    this->currentState = SuperKenAction::SCANNING;
                }
                else
                {
                    this->shootPoint(this->lastEnemyPosition);
                }
                break;
                
            case SuperKenAction::SCANNING:
                targetX = (this->isLeft(position)) ? dimentions.width - box.size.width / 2 : box.size.width / 2;
                rnd = rand() % 30 + 30;
                for (float y = box.size.width / 2; y <= dimentions.height - box.size.width / 2; y += rnd) {
                    shootPoint(RWVec(targetX, y));
                    if (this->currentState != SuperKenAction::SCANNING) {
                        break;
                    }
                }
                if (this->currentState == SuperKenAction::SCANNING) {
                    this->currentState = SuperKenAction::VERTICAL_ESCAPE;
                }
                break;
            default:
                break;
        }
    }
}

#pragma - delegate

void SuperKen::bulletHitEnemy(RWVec enemyPosition)
{
    this->lastEnemyPosition = enemyPosition;
    this->timeSinceLastEnemyHit = this->currentTimestamp();
    if (this->currentState != SuperKenAction::FIRING) {
        this->cancelActiveAction();
    }
    this->currentState = SuperKenAction::FIRING;
}

void SuperKen::scannedRobotAtPosition(RWVec position)
{
    this->cancelActiveAction();
    this->lastEnemyPosition = position;
    this->timeSinceLastEnemyHit = this->currentTimestamp();
    this->currentState = SuperKenAction::FIRING;
}

void SuperKen::gotHit()
{
    if (this->currentState == SuperKenAction::FIRING && this->hitPoints() > this->enemyHitPoint) {
        
    } else {
        this->cancelActiveAction();
        this->currentState = this->getRandomEscape();
    }
}

#pragma - other method

void SuperKen::goToPoint(RWVec point)
{
    auto firstState = this->currentState;
    float angle = this->angleBetweenHeadingDirectionAndWorldPosition(point);
    if (angle < 0) {
        this->turnRobotLeft(-angle);
    } else if (angle > 0) {
        this->turnRobotRight(angle);
    }
    if (firstState != this->currentState) {
        return;
    }
    
   float d = KMath::distance(point, this->position());
    moveAhead(d);
    
}

bool SuperKen::isUp(RWVec point)
{
    auto dimentions = this->arenaDimensions();
    if (point.y > dimentions.height / 2) {
        return true;
    } else {
        return false;
    }
}

bool SuperKen::isLeft(RWVec point)
{
    auto dimentions = this->arenaDimensions();
    if (point.x < dimentions.width / 2) {
        return true;
    } else {
        return false;
    }
}

void SuperKen::shootPoint(RWVec point)
{
    auto firstState = this->currentState;
    float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(point);
        if (angle < 0) {
        this->turnGunLeft(-angle);
    } else if (angle > 0) {
        this->turnGunRight(angle);
    }
        if (firstState != this->currentState) {
        return;
    }

    this->shoot();
}

SuperKenAction::SuperKenAction SuperKen::getRandomEscape()
{
    int rnd = rand() % 2;
    if (rnd == 0) {
        return SuperKenAction::VERTICAL_ESCAPE;
    } else {
        return SuperKenAction::HORIZONTAL_ESCAPE;
    }
}
