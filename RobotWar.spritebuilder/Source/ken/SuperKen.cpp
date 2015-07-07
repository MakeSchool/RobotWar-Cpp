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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// I copied mitsu code...
#define DEBUG_MODE 0

#if DEBUG_MODE == 1
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) {}
#endif


using namespace SuperKenMath;

SuperKen::SuperKen()
{
    this->currentStrategy = SuperKenStrategy::CORNER_SHOT;
    this->currentState = SuperKenAction::MOVE_VERTICAL;
    this->lastEnemyPosition = RWVec(-1, -1);
    this->comboDamage = 0;
    srand((int)time(NULL));
}

void SuperKen::run()
{
    this->enemyHitPoint = this->hitPoints();

    while (true)
    {
        switch (this->currentStrategy) {
            case SuperKenStrategy::ALASKA_THUNDER:
                this->stepAlaskaThunder();
                break;
            case SuperKenStrategy::CORNER_SHOT:
                this->stepCornerShot();
                break;
        }
    }
}

#pragma - delegate

void SuperKen::bulletHitEnemy(RWVec enemyPosition)
{
    DEBUG_PRINT("HIT ENEMY! (x = %f, y = %f)\n", enemyPosition.x, enemyPosition.y);
    this->comboDamage = 0;
    this->enemyHitPoint--;
    this->lastEnemyPosition = enemyPosition;
    this->timeSinceLastEnemyHit = this->currentTimestamp();

    switch (this->currentStrategy) {
        case SuperKenStrategy::ALASKA_THUNDER:
            this->bulletHitEnemyWithAlaskaThunder(enemyPosition);
            break;
        case SuperKenStrategy::CORNER_SHOT:
            this->bulletHitEnemyWithCornerShot(enemyPosition);
            break;
    }
}

void SuperKen::scannedRobotAtPosition(RWVec position)
{
    DEBUG_PRINT("SCAN ENEMY! (x = %f, y = %f)\n", position.x, position.y);
    this->lastEnemyPosition = position;
    this->timeSinceLastEnemyHit = this->currentTimestamp();

    switch (this->currentStrategy) {
        case SuperKenStrategy::ALASKA_THUNDER:
            this->scannedRobotAtPositionWithAlaskaThunder(position);
            break;
        case SuperKenStrategy::CORNER_SHOT:
            this->scannedRobotAtPositionWithCornerShot(position);
            break;
    }
}

void SuperKen::gotHit()
{
    this->comboDamage++;
    DEBUG_PRINT("GOT HIT!\n");

    switch (this->currentStrategy) {
        case SuperKenStrategy::ALASKA_THUNDER:
            this->gotHitWithAlaskaThunder();
            break;
        case SuperKenStrategy::CORNER_SHOT:
            this->gotHitWithCornerShot();
            break;
    }
}

#pragma - AlaskaThunder

void SuperKen::stepAlaskaThunder()
{
    RWVec position, p1, p2;
    RWRect box = this->robotBoundingBox();
    RWSize dimentions = this->arenaDimensions();
    float oppositeX = 0.0f, oppositeY = 0.0f;
    int division;
    position = this->position();
    oppositeX = this->getOppositeX(position.x);
    oppositeY = this->getOppositeY(position.y);
    switch (this->currentState)
    {        
        case SuperKenAction::MOVE_VERTICAL:
            DEBUG_PRINT("MOVE VERTICAL\n");
            this->goToPoint(RWVec(position.x, oppositeY));
            this->currentState = SuperKenAction::MOVE_AND_SHOT;
            break;
        case SuperKenAction::MOVE_OTHERSIDE:
            DEBUG_PRINT("MOVE OTHERSIDE\n");
            this->goToPoint(RWVec(oppositeX, position.y));
            this->currentState = SuperKenAction::MOVE_AND_SHOT;
            break;
        case SuperKenAction::MOVE_AND_SHOT:
            division = rand() % 4 + 7;
            p1 = RWVec(position.x, position.y);
            p2 = RWVec(position.x, oppositeY);
            for (int n = 1; n <= division - 2; n++) {
                RWVec moveTo = KMath::divideInternally(p1, p2, division - 1 - n, n);
                this->goToPoint(moveTo);
                RWVec nowPosition = this->position();
                RWVec target = RWVec(oppositeX, (this->isUp(nowPosition.y) ? dimentions.height - box.size.height : box.size.height));
                this->shootPoint(target);
                if (this->currentState != SuperKenAction::MOVE_AND_SHOT) {
                    break;
                }
            }
            this->goToPoint(RWVec(position.x, oppositeY));
            break;
        case SuperKenAction::FIRING:
            DEBUG_PRINT("FIRE\n");
            if (this->currentTimestamp() - this->timeSinceLastEnemyHit > 2.5f)
            {
                this->cancelActiveAction();
                this->currentState = SuperKenAction::SCANNING;
            }
            else
            {
                int rnd = rand() % 5 - 2;
                RWVec target = RWVec(this->lastEnemyPosition.x, this->lastEnemyPosition.y + 50 * rnd);
                this->shootPoint(target);
            }
            break;
            
        default:
            this->currentState = SuperKenAction::MOVE_AND_SHOT;
            break;
    }
}

void SuperKen::bulletHitEnemyWithAlaskaThunder(RWVec enemyPosition)
{
    float diffX = fabs(enemyPosition.x - this->position().x);

    switch (this->currentState) {
    case SuperKenAction::MOVE_OTHERSIDE:
        DEBUG_PRINT("KEEP ON MOVING\n");
        break;
    case SuperKenAction::MOVE_AND_SHOT:
    default:
        this->cancelActiveAction();
    case SuperKenAction::FIRING:
        if (diffX < 200) {
            this->currentState = SuperKenAction::MOVE_OTHERSIDE;
        } else {
            this->currentState = SuperKenAction::FIRING;
        }
        break;
    }
}

void SuperKen::scannedRobotAtPositionWithAlaskaThunder(RWVec position)
{
    DEBUG_PRINT("SCAN\n");
    switch (this->currentState) {
    case SuperKenAction::MOVE_OTHERSIDE:
        DEBUG_PRINT("KEEP MOVING\n");
        break;
    case SuperKenAction::MOVE_AND_SHOT:
    case SuperKenAction::FIRING:
    default:
        DEBUG_PRINT("ESCAPE\n");
        this->currentState = SuperKenAction::MOVE_OTHERSIDE;
        break;
    }
}

void SuperKen::gotHitWithAlaskaThunder()
{
    DEBUG_PRINT("GOT HIT\n");
    switch (this->currentState) {
    case SuperKenAction::MOVE_OTHERSIDE:
        DEBUG_PRINT("ALREADY MOVING\n");
        break;
    case SuperKenAction::FIRING:
    case SuperKenAction::MOVE_AND_SHOT:
        if (this->isAdvantage()) {
            DEBUG_PRINT("I HAVE ADVANTAGE! KEEP\n");
        } else {
            DEBUG_PRINT("ENEMY HAVE ADVANTAGE... ESCAPE!\n");
            if (this->lastEnemyPosition.x > 0
            && this->isLeft(this->position().x) == this->isLeft(this->lastEnemyPosition.x))
            {
                this->cancelActiveAction();
                this->currentState = SuperKenAction::MOVE_OTHERSIDE;
            } else {
                this->currentState = SuperKenAction::MOVE_AND_SHOT;
            }
        }
    default:
        this->currentState = SuperKenAction::MOVE_AND_SHOT;
        break;
    }
}

#pragma - CornerShot

void SuperKen::stepCornerShot()
{
    RWVec position, p1, p2;
    RWRect box = this->robotBoundingBox();
    RWSize dimentions = this->arenaDimensions();
    float oppositeX = 0.0f, oppositeY = 0.0f;
    int division;
    position = this->position();
    oppositeX = this->getOppositeX(position.x);
    oppositeY = this->getOppositeY(position.y);
    switch (this->currentState)
    {        
        case SuperKenAction::MOVE_VERTICAL:
            DEBUG_PRINT("MOVE VERTICAL\n");
            this->goToPoint(RWVec(position.x, oppositeY));
            this->currentState = SuperKenAction::SCANNING;
            break;
        case SuperKenAction::MOVE_OTHERSIDE:
            DEBUG_PRINT("MOVE OTHERSIDE\n");
            this->lastEnemyPosition = RWVec(-1, -1);
            this->goToPoint(RWVec(oppositeX, position.y));
            this->currentState = SuperKenAction::SCANNING;
            break;
        case SuperKenAction::FIRING:
            DEBUG_PRINT("FIRE\n");
            if (this->currentTimestamp() - this->timeSinceLastEnemyHit > 2.5f)
            {
                this->cancelActiveAction();
                this->currentState = SuperKenAction::SCANNING;
            }
            else
            {
                int rnd = rand() % 5 - 2;
                rnd = 0;
                RWVec target = RWVec(this->lastEnemyPosition.x, this->lastEnemyPosition.y + 50 * rnd);
                this->shootPoint(target);
            }
            break;
            
        case SuperKenAction::SCANNING:
            DEBUG_PRINT("SCANING\n");
            division = rand() % 4 + 3;
            p1 = RWVec(oppositeX, box.size.height);
            p2 = RWVec(oppositeX, dimentions.height - box.size.height);
            for (int n = 0; n <= division - 1; n++) {
                RWVec target = KMath::divideInternally(p1, p2, n, division - 1 - n);
                this->shootPoint(target);
                if (this->currentState != SuperKenAction::SCANNING) {
                    break;
                }
            }
            if (this->currentState == SuperKenAction::SCANNING && rand() % 2 == 0) {
                this->shootPoint(RWVec(position.x, oppositeY));
            }
            break;
        default:
            this->currentState = SuperKenAction::SCANNING;
            break;
    }
}

void SuperKen::bulletHitEnemyWithCornerShot(RWVec enemyPosition)
{
    float diffX = fabs(enemyPosition.x - this->position().x);

    switch (this->currentState) {
    case SuperKenAction::MOVE_OTHERSIDE:
    case SuperKenAction::MOVE_VERTICAL:
        DEBUG_PRINT("KEEP ON MOVING\n");
        break;
    case SuperKenAction::SCANNING:
    default:
        this->cancelActiveAction();
    case SuperKenAction::FIRING:
        if (diffX < 200 && comboDamage >= 1) {
            this->currentState = SuperKenAction::MOVE_OTHERSIDE;
        } else {
            this->currentState = SuperKenAction::FIRING;
        }
        break;
    }
}

void SuperKen::scannedRobotAtPositionWithCornerShot(RWVec position)
{
    DEBUG_PRINT("SCAN\n");
    switch (this->currentState) {
    case SuperKenAction::MOVE_OTHERSIDE:
    case SuperKenAction::MOVE_VERTICAL:
        DEBUG_PRINT("KEEP MOVING\n");
        break;
    case SuperKenAction::SCANNING:
    case SuperKenAction::FIRING:
    default:
        if (comboDamage >= 1) {
            DEBUG_PRINT("ESCAPE\n");
            this->shootPoint(position);
            this->currentState = SuperKenAction::MOVE_OTHERSIDE;
            break;
        }
    }
}

void SuperKen::gotHitWithCornerShot()
{
    DEBUG_PRINT("GOT HIT\n");
    switch (this->currentState) {
    case SuperKenAction::MOVE_VERTICAL:
    case SuperKenAction::MOVE_OTHERSIDE:
        DEBUG_PRINT("KEEP MOVING\n");
        break;
    case SuperKenAction::FIRING:
        if (this->isAdvantage()) {
            DEBUG_PRINT("I HAVE ADVANTAGE! CONTINUE TO FIRE\n");
        } else {
            DEBUG_PRINT("ENEMY HAVE ADVANTAGE... ESCAPE!\n");
            this->cancelActiveAction();
            this->currentState = SuperKenAction::MOVE_VERTICAL;
        }
    case SuperKenAction::SCANNING:
        DEBUG_PRINT("STOP SCANNING, AND ESCAPE!\n");
        if (this->lastEnemyPosition.x > 0
            && this->isLeft(this->position().x) == this->isLeft(this->lastEnemyPosition.x))
        {
            this->cancelActiveAction();
            this->currentState = SuperKenAction::MOVE_OTHERSIDE;
        } else if (comboDamage >= 3) {
            this->cancelActiveAction();
            this->currentState = SuperKenAction::MOVE_VERTICAL;
        }

    default:
        break;
    }
}

#pragma - other method

bool SuperKen::isAdvantage()
{
    return this->hitPoints() > this->enemyHitPoint;
}

bool SuperKen::isUp(float y)
{
    auto dimentions = this->arenaDimensions();
    return y > dimentions.height / 2;
}

bool SuperKen::isLeft(float x)
{
    auto dimentions = this->arenaDimensions();
    return x < dimentions.width / 2;
}

float SuperKen::getOppositeX(float x)
{
    auto dimentions = this->arenaDimensions();
    auto box = this->robotBoundingBox();
    return (this->isLeft(x)) ? dimentions.width - box.size.width / 2 : box.size.width / 2;
   
}

float SuperKen::getOppositeY(float y)
{
    auto dimentions = this->arenaDimensions();
    auto box = this->robotBoundingBox();
    return (this->isUp(y)) ? box.size.height / 2 : dimentions.height - box.size.height / 2;
}

void SuperKen::goToPoint(RWVec point)
{
    DEBUG_PRINT("GO TO POINT (x = %f, y = %f) \n", point.x, point.y);
    float d = KMath::distance(point, this->position());
    if (d < 1) {
        return;
    }

    float angle = this->angleBetweenHeadingDirectionAndWorldPosition(point);
    if (angle < -90) {
        this->turnRobotRight(180 - fabs(angle));
    } else if (angle > 90) {
        this->turnRobotLeft(180 - angle);
    } else if (angle < -1) {
        this->turnRobotLeft(-angle);
    } else if (angle > 1) {
        this->turnRobotRight(angle);
    }
    if (this->currentState == SuperKenAction::FIRING) {
        return;
    }
    
    if (fabs(angle) > 90) {
        moveBack(d);
    } else {
        moveAhead(d);
    }
    
}

void SuperKen::shootPoint(RWVec point)
{
    DEBUG_PRINT("SHOOT TO POINT (x = %f, y = %f) \n", point.x, point.y);
    float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(point);
        if (angle < -1) {
        this->turnGunLeft(-angle);
    } else if (angle > 1) {
        this->turnGunRight(angle);
    }
    if (this->currentState == SuperKenAction::MOVE_VERTICAL
    || this->currentState == SuperKenAction::MOVE_OTHERSIDE)
    {
        return;
    }

    this->shoot();
}

