//
//  TAMAKI.cpp
//  RobotWar
//
//  Created by ryutamaki on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include "TAMAKI.h"

#include <iostream>
#include <cmath>
#include <ctime>

#pragma mark - Initializer

TAMAKI::TAMAKI()
{
    RobotCpp();

    this->enemyHitpoint = 20;
    this->enemyPositions = std::vector<RWVec>(0);

    this->setState(TAMAKIAction::PREPARE_FOR_SNIPE);
}

#pragma mark - Delegate methods

void TAMAKI::run()
{
    while (1) {
        switch (this->state) {
            case TAMAKIAction::PREPARE_FOR_SNIPE:
                this->prepareForSnipe();
                break;
            case TAMAKIAction::SEARCH_USING_SNIPE:
                this->searchUsingSnipe();
                break;
            case TAMAKIAction::SNIPE:
                if (this->lastHitTime < std::time(nullptr) - 1.5)
                {
                    this->setState(TAMAKIAction::SEARCH_USING_SNIPE_CLOSE_TO_LAST_ENEMY_POSITION);
                    break;
                }
                this->snipe();
                break;
            case TAMAKIAction::SEARCH_USING_SNIPE_CLOSE_TO_LAST_ENEMY_POSITION:
                this->searchUsingSnipeCloseToLastEnemyPosition();
                break;
            case TAMAKIAction::ESCAPE:
                this->escape();
                break;

            default:
                break;
        }
    }
}

void TAMAKI::scannedRobotAtPosition(RWVec position)
{
    this->enemyPositions.push_back(position);
    this->lastHitTime = std::time(nullptr);
    this->setState(TAMAKIAction::SNIPE);
}

void TAMAKI::gotHit()
{
    this->escapeCounter++;
    if (this->escapeCounter > 3)
    {
        this->escapeCounter = 0;
        this->setState(TAMAKIAction::ESCAPE);
    }
}

void TAMAKI::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
    this->cancelActiveAction();

    switch (side)
    {
        case RobotWallHitSide::FRONT:
            std::cout << "front" << std::endl;
            break;

        case RobotWallHitSide::REAR:
            std::cout << "rear" << std::endl;
            break;

        case RobotWallHitSide::LEFT:
            std::cout << "left" << std::endl;
            break;

        case RobotWallHitSide::RIGHT:
            std::cout << "right" << std::endl;
            break;

        case RobotWallHitSide::NONE:
            break;
    }
}

void TAMAKI::bulletHitEnemy(RWVec enemyPosition)
{
    this->enemyPositions.push_back(enemyPosition);
    this->lastHitTime = std::time(nullptr);
    this->setState(TAMAKIAction::SNIPE);
}

#pragma mark - Private methods

void TAMAKI::prepareForSnipe()
{
    RWSize arenaSize = this->arenaDimensions();

    RWVec toVec;
    int toDegree;
    if (this->position().x < arenaSize.width / 2.0f)
    {
        toVec = RWVec(arenaSize.width - 50.0f, this->position().y);
        toDegree = this->angleBetweenHeadingDirectionAndWorldPosition(toVec);
    }
    else
    {
        toVec = RWVec(50.0f, this->position().y);
        toDegree = this->angleBetweenHeadingDirectionAndWorldPosition(toVec);
    }
    if (toDegree < 0)
    {
        this->turnRobotLeft(abs(toDegree));
    }
    else
    {
        this->turnRobotRight(toDegree);
    }

    float arenaSizeWidthHalved = arenaSize.width / 2.0f;
    float distanceToMoveBack = floor(arenaSizeWidthHalved - fabsf(this->position().x - arenaSizeWidthHalved/* avoid collision */));
    this->moveBack(distanceToMoveBack);

    this->setState(TAMAKIAction::SEARCH_USING_SNIPE);
}

void TAMAKI::searchUsingSnipe()
{
    RWSize arenaSize = this->arenaDimensions();

    RWVec firstShootPosition, endShootPosition;
    if (this->position().x < arenaSize.width / 2.0f)
    {
        firstShootPosition = RWVec(arenaSize.width, arenaSize.height);
        endShootPosition = RWVec(arenaSize.width, 0.0f);
    }
    else
    {
        firstShootPosition = RWVec(0.0f, 0.0f);
        endShootPosition = RWVec(0.0f, arenaSize.height);
    }

    int angleToStart = this->angleBetweenGunHeadingDirectionAndWorldPosition(firstShootPosition);
    int startDegree = this->degreeBetweenRWVecs(this->position(), firstShootPosition);
    int endDegree = this->degreeBetweenRWVecs(this->position(), endShootPosition);

    if (angleToStart < 0)
    {
        this->turnGunLeft(abs(angleToStart));
    }
    else
    {
        this->turnGunRight(angleToStart);
    }
    this->shoot();

    int totalDeltaDegree = abs(startDegree) + abs(endDegree);
    const int deltaDegree = 8;
    for (int deg = 0; deg < totalDeltaDegree; deg += deltaDegree)
    {
        if (this->state != TAMAKIAction::SEARCH_USING_SNIPE)
        {
            continue;
        }
        
        this->turnGunRight(deltaDegree);
        this->shoot();
    }
}

void TAMAKI::searchUsingSnipeCloseToLastEnemyPosition()
{
    const int angleRange = 20;
    const int deltaDegree = 5;

    int angleToStart = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->enemyPositions.back());
    if (angleToStart < 0)
    {
        this->turnGunLeft(abs(angleToStart) + angleRange / 2);
    }
    else
    {
        this->turnGunRight(angleToStart + angleRange / 2);
    }
    this->shoot();

    for (int deg = 0; deg < angleRange; deg += deltaDegree)
    {
        if (this->state != TAMAKIAction::SEARCH_USING_SNIPE_CLOSE_TO_LAST_ENEMY_POSITION)
        {
            continue;
        }

        if (angleToStart < 0)
        {
            this->turnGunRight(deltaDegree);
        }
        else
        {
            this->turnGunLeft(deltaDegree);
        }
        this->shoot();
    }

    if (this->state == TAMAKIAction::SEARCH_USING_SNIPE_CLOSE_TO_LAST_ENEMY_POSITION)
    {
        this->setState(TAMAKIAction::SEARCH_USING_SNIPE);
    }
}

void TAMAKI::snipe()
{
    int degreeToTarget = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->enemyPositions.back());
    if (degreeToTarget < 0)
    {
        this->turnGunLeft(abs(degreeToTarget));
    }
    else
    {
        this->turnGunRight(degreeToTarget);
    }
    this->shoot();
}

void TAMAKI::escape()
{
    RWSize arenaSize = this->arenaDimensions();
    this->moveAhead(arenaSize.width - 70);
    this->setState(TAMAKIAction::PREPARE_FOR_SNIPE);
}

#pragma mark - Accessor

void TAMAKI::setState(TAMAKIAction::TAMAKIAction state)
{
    if (this->state == TAMAKIAction::ESCAPE && state != TAMAKIAction::PREPARE_FOR_SNIPE)
    {
        return;
    }

    this->state = state;
    switch (state) {
        case TAMAKIAction::SNIPE:
            if (this->state != TAMAKIAction::SNIPE)
            {
                this->cancelActiveAction();
            }
            break;

        default:
            break;
    }
    std::cout << this->state << std::endl;
}

#pragma mark - Debugging

void TAMAKI::printRWVec(RWVec vec)
{
    std::cout << "vec: (" << vec.x << ", " << vec.y << ")" << std::endl;
}

void TAMAKI::printRWSize(RWSize size)
{
    std::cout << "size: (" << size.width << ", " << size.height << ")" << std::endl;
}

int TAMAKI::radianToDegree(float rad)
{
//    std::cout << rad * (180/M_PI) << std::endl;
    return rad * (180/M_PI);
}

int TAMAKI::degreeBetweenRWVecs(RWVec vec1, RWVec vec2)
{
    float xDistance = vec2.x - vec1.x;
    float yDistance = vec2.y - vec1.y;
    float distance = powf(powf(xDistance, 2) + powf(yDistance, 2), 0.5f);
    return this->radianToDegree(asin(yDistance / distance));
}