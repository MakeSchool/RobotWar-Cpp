//
//  Fliegerhammer.cpp
//  RobotWar
//
//  Created by Akihito OKUHATA on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include "Fliegerhammer.h"
#include <tgmath.h>
#include <iostream>
#include <random>

using namespace std;


Fliegerhammer::Fliegerhammer() :
    randomGenerator(std::default_random_engine((unsigned int) time(0)))
{
    this->currentState = FliegerhammerAction::DEFAULT;
    this->actionIndex = 0;
    
    
}

void Fliegerhammer::run()
{
    this->actionIndex = 0;

    while (true) {
        while (this->currentState == FliegerhammerAction::TURN_AROUND) {
            this->performNextTurnAroundAction();
        }
        while (this->currentState == FliegerhammerAction::FIRING) {
            this->performNextFiringAction();
        }

        while (this->currentState == FliegerhammerAction::SEARCHING) {
            this->performNextSearchingAction();
        }

        while (this->currentState == FliegerhammerAction::DEFAULT) {
            this->performNextDefaultAction();
        }
    }
}

// {{{ state machines

void Fliegerhammer::performNextDefaultAction()
{
    this->currentState = FliegerhammerAction::TURN_AROUND;
}

void Fliegerhammer::performNextFiringAction()
{
    while (this->currentState == FliegerhammerAction::FIRING) {
        this->barrage();
    }
}

void Fliegerhammer::performNextSearchingAction()
{
    cout << "Searching..." << endl;
    // search by radar
    if (this->currentState == FliegerhammerAction::SEARCHING) {
        this->turnRobotLeft(100);
        this->turnRobotLeft(90);
    }

    // search by gun
    int searchingDegree = 5;
    int searchingCount = 105 / searchingDegree;
    while (this->currentState == FliegerhammerAction::SEARCHING && searchingCount--) {
        this->shoot();
        this->turnRobotRight(5);
    }

    if (this->currentState == FliegerhammerAction::SEARCHING) {
        this->currentState = FliegerhammerAction::TURN_AROUND;
    }
}

void Fliegerhammer::performNextTurnAroundAction()
{
    cout << "Turning around..." << endl;
    RWVec pos = this->position();
    FliegerhammerArea::FliegerhammerArea area = this->getAreaFromPosition(pos);

    std::uniform_int_distribution<> dist(1, 2);
    int numOfCorner = dist(this->randomGenerator);
    this->moveToTriangle(area, numOfCorner);

    this->currentState = FliegerhammerAction::SEARCHING;
}

// }}}

// {{{

FliegerhammerArea::FliegerhammerArea Fliegerhammer::getNextCorner(FliegerhammerArea::FliegerhammerArea area, bool counterClockwise)
{
    switch (area) {
        case FliegerhammerArea::TopLeft:
            return counterClockwise ? FliegerhammerArea::BottomLeft : FliegerhammerArea::TopRight;

        case FliegerhammerArea::TopRight:
            return counterClockwise ? FliegerhammerArea::TopLeft : FliegerhammerArea::BottomRight;

        case FliegerhammerArea::BottomRight:
            return counterClockwise ? FliegerhammerArea::TopRight : FliegerhammerArea::BottomLeft;

        case FliegerhammerArea::BottomLeft:
            return counterClockwise ? FliegerhammerArea::BottomRight: FliegerhammerArea::TopLeft;
    }
}

void Fliegerhammer::moveToTriangle(FliegerhammerArea::FliegerhammerArea currentArea, int numOfCorner)
{
    FliegerhammerArea::FliegerhammerArea direction1, direction2, direction3;
    direction1 = this->getNextCorner(currentArea);
    direction2 = this->getNextCorner(direction1);
    direction3 = this->getNextCorner(direction2);

    if (this->currentState == FliegerhammerAction::TURN_AROUND && numOfCorner >= 1) {
        this->moveToCorner(direction1);
    }
    if (this->currentState == FliegerhammerAction::TURN_AROUND && numOfCorner >= 2) {
        this->moveToCorner(direction2);
    }
}

RWVec Fliegerhammer::getCornerCoordinateByArea(FliegerhammerArea::FliegerhammerArea area)
{
    RWSize dimension = this->arenaDimensions();
    RWVec targetPosition;

    switch (area) {
        case FliegerhammerArea::TopLeft:
            return RWVec(0.0f, dimension.height);

        case FliegerhammerArea::TopRight:
            return RWVec(dimension.width, dimension.height);

        case FliegerhammerArea::BottomRight:
            return RWVec(dimension.width, 0.0f);

        case FliegerhammerArea::BottomLeft:
            return RWVec(0.0f, 0.0f);
    }

    // fail safe
    return RWVec(0.0f, 0.0f);
}

void Fliegerhammer::moveToCorner(FliegerhammerArea::FliegerhammerArea area)
{
    RWVec direction = this->getCornerCoordinateByArea(area);
    float angle = this->angleBetweenHeadingDirectionAndWorldPosition(direction);
    RWVec position = this->position();
    float distance = sqrt(pow(direction.x - position.x, 2) + pow(direction.y - position.y, 2));

    if (angle > 0.0f) {
        this->turnRobotRight(angle);
    } else {
        this->turnRobotLeft(-angle);
    }

    this->moveAhead(distance);
}


FliegerhammerArea::FliegerhammerArea Fliegerhammer::getAreaFromPosition(RWVec position)
{
    RWSize dimension = this->arenaDimensions();
    float half_x = dimension.width  / 2.0f;
    float half_y = dimension.height / 2.0f;

    if (position.x < half_x && position.y < half_y) {
        return FliegerhammerArea::BottomLeft;
    }
    if (position.x < half_x && position.y > half_y) {
        return FliegerhammerArea::TopLeft;
    }
    if (position.x > half_x && position.y < half_y) {
        return FliegerhammerArea::BottomRight;
    }
    if (position.x > half_x && position.y > half_y) {
        return FliegerhammerArea::TopRight;
    }

    // fail safe
    return FliegerhammerArea::BottomLeft;
}

void Fliegerhammer::barrage()
{
    if (this->enemyPositionsInFiring.empty()) {
        // 初回 -> enemyPositionInsearch の両側 30度, 15度に撃つ
        RWVec pos = this->enemyPositionInSearch;
        float angle = this->angleBetweenHeadingDirectionAndWorldPosition(pos);
        if (angle > 0.0f) {
            this->turnRobotRight(angle);
        } else {
            this->turnRobotLeft(-angle);
        }
        this->shoot();
        this->turnRobotRight(30);
        this->shoot();
        this->turnRobotLeft(60);
        this->shoot();
        this->turnRobotRight(45);
        this->shoot();
        this->turnRobotLeft(30);
        this->shoot();
        this->turnRobotRight(15);
    }
    else {
        // 2回目以降
        cout << "(" << enemyPositionInSearch.x << ", " << enemyPositionInSearch.y << ") -> ";
        for (auto pos = enemyPositionsInFiring.begin(), last = enemyPositionsInFiring.end(); pos != last; ++pos) {
            cout << "(" << pos->x << ", " << pos->y << "), ";
        }
        cout << endl;

        float t1, t2;
        RWVec p1, p2;
        if (this->enemyPositionsInFiring.size() == 1) {
            t1 = this->bulletHitTimestampInSearch;
            t2 = this->bulletHitTimestampList[0];
            p1 = this->enemyPositionInSearch;
            p2 = this->enemyPositionsInFiring[0];
        } else {
            t1 = this->bulletHitTimestampList[this->bulletHitTimestampList.size() - 2];
            t2 = this->bulletHitTimestampList[this->bulletHitTimestampList.size() - 1];
            p1 = this->enemyPositionsInFiring[this->enemyPositionsInFiring.size() - 2];
            p2 = this->enemyPositionsInFiring[this->enemyPositionsInFiring.size() - 1];
        }

        float r1 = this->angleBetweenHeadingDirectionAndWorldPosition(p1);
        float r2 = this->angleBetweenHeadingDirectionAndWorldPosition(p2);
        float r_diff = (r2 - r1) / (t2 - t1);
        if (this->currentTimestamp() - t2 != 0) {
            //r_diff /= (this->currentTimestamp() - t2);
        }
        if (r_diff > 0) {
            //right
            this->turnRobotRight(r_diff);
            this->shoot();
            this->turnRobotRight(5);
            this->shoot();
            this->turnRobotLeft(5);
        } else {
            //left
            this->turnRobotLeft(-r_diff);
            this->shoot();
            this->turnRobotLeft(5);
            this->shoot();
            this->turnRobotRight(5);
        }
    }
}


// }}}



// {{{ callbacks

void Fliegerhammer::gotHit()
{
    cout << "gotHit!!" << endl;
    if (this->currentState == FliegerhammerAction::FIRING) {
        this->bulletHitTimestampInSearch = 0.0f;
        this->enemyPositionInSearch = RWVec(0, 0);
        this->enemyPositionsInFiring.clear();
        this->bulletHitTimestampList.clear();

        this->currentState = FliegerhammerAction::TURN_AROUND;
        this->cancelActiveAction();
        return;
    }

    if (this->currentState == FliegerhammerAction::SEARCHING) {
        this->currentState = FliegerhammerAction::TURN_AROUND;
        this->cancelActiveAction();
        return;
    }

    this->currentState = FliegerhammerAction::TURN_AROUND;
    this->cancelActiveAction();
    return;
}

void Fliegerhammer::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{

}

void Fliegerhammer::bulletHitEnemy(RWVec enemyPosition)
{
    cout << "bulletHit!! (" << enemyPosition.x << ", " << enemyPosition.y << ")" << endl;

    if (this->currentState == FliegerhammerAction::SEARCHING) {
        this->enemyPositionInSearch = enemyPosition;
        this->bulletHitTimestampInSearch = this->currentTimestamp();
        this->currentState = FliegerhammerAction::FIRING;
        this->cancelActiveAction();
        return;
    }

    if (this->currentState == FliegerhammerAction::FIRING) {
        this->enemyPositionsInFiring.push_back(enemyPosition);
        this->bulletHitTimestampList.push_back(this->currentTimestamp());
        return;
    }
}

void Fliegerhammer::scannedRobotAtPosition(RWVec position)
{
    cout << "scanned!! (" << position.x << ", " << position.y << ")" << endl;
    if (this->currentState != FliegerhammerAction::FIRING) {
        this->bulletHitTimestampInSearch = this->currentState;
        this->enemyPositionInSearch = position;

        this->moveToCorner(this->getNextCorner(this->getAreaFromPosition(this->position())));

        this->currentState = FliegerhammerAction::FIRING;
        this->cancelActiveAction();
    }
}

// }}}
