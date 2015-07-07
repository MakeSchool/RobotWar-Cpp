//
//  AkiRobotCpp.cpp
//  RobotWar
//
//  Created by Akihito OKUHATA on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include "AkiRobotCpp.h"
#include <tgmath.h>
#include <iostream>
#include <random>

using namespace std;

AkiRobotCpp::AkiRobotCpp() :
    randomGenerator(std::default_random_engine((unsigned int) time(0)))
{
    this->currentState = FliegerhammerAction::DEFAULT;
    this->actionIndex = 0;
    this->enemyHp = 20;
    this->ownHp = 20;
    this->emergencyActing = false;
}

void AkiRobotCpp::run()
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

        if (this->currentState == FliegerhammerAction::RESET_SEARCHING) {
            this->currentState = FliegerhammerAction::SEARCHING;
        }
        if (this->currentState == FliegerhammerAction::RESET_FIRING) {
            this->currentState = FliegerhammerAction::FIRING;
        }
    }
}

// {{{ state machines

void AkiRobotCpp::performNextDefaultAction()
{
    this->currentState = FliegerhammerAction::TURN_AROUND;
}

void AkiRobotCpp::performNextFiringAction()
{
    while (this->currentState == FliegerhammerAction::FIRING) {
        this->barrage();
    }
}

void AkiRobotCpp::performNextSearchingAction()
{
    cout << "Searching..." << endl;

    FliegerhammerArea::FliegerhammerArea area = this->getAreaFromPosition(this->position());
    for (int i = 0; i < 3 && this->currentState == FliegerhammerAction::SEARCHING; ++i) {
        area = this->getNextCorner(area);
        RWVec corner = this->getCornerCoordinateByArea(area);
        float angle = this->angleBetweenHeadingDirectionAndWorldPosition(corner);
        if (angle >= 0.0f) {
            this->turnRobotRight(abs(angle));
        } else {
            this->turnRobotLeft(abs(angle));
        }
        this->shoot();
    }

    RWVec prevCorner = this->getCornerCoordinateByArea(this->getNextCorner(this->getAreaFromPosition(this->position())));
    float angle = this->angleBetweenHeadingDirectionAndWorldPosition(prevCorner);
    if (angle >= 0.0f) {
        this->turnRobotRight(abs(angle));
    } else {
        this->turnRobotLeft(abs(angle));
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

void AkiRobotCpp::performNextTurnAroundAction()
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

FliegerhammerArea::FliegerhammerArea AkiRobotCpp::getNextCorner(FliegerhammerArea::FliegerhammerArea area, bool counterClockwise)
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

void AkiRobotCpp::moveToTriangle(FliegerhammerArea::FliegerhammerArea currentArea, int numOfCorner)
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

RWVec AkiRobotCpp::getCornerCoordinateByArea(FliegerhammerArea::FliegerhammerArea area)
{
    RWSize dimension = this->arenaDimensions();
    RWVec targetPosition;
    float diff = 30.0f;

    switch (area) {
        case FliegerhammerArea::TopLeft:
            return RWVec(diff, dimension.height - diff);

        case FliegerhammerArea::TopRight:
            return RWVec(dimension.width - diff, dimension.height - diff);

        case FliegerhammerArea::BottomRight:
            return RWVec(dimension.width - diff, diff);

        case FliegerhammerArea::BottomLeft:
            return RWVec(diff, diff);
    }

    // fail safe
    return RWVec(0.0f, 0.0f);
}

void AkiRobotCpp::moveToCorner(FliegerhammerArea::FliegerhammerArea area)
{
    RWVec direction = this->getCornerCoordinateByArea(area);
    float angle = this->angleBetweenHeadingDirectionAndWorldPosition(direction);
    RWVec position = this->position();
    float distance = sqrt(pow(direction.x - position.x, 2) + pow(direction.y - position.y, 2));

    if (angle >= 0.0f) {
        this->turnRobotRight(abs(angle));
    } else {
        this->turnRobotLeft(abs(angle));
    }

    this->moveAhead(distance);
}


FliegerhammerArea::FliegerhammerArea AkiRobotCpp::getAreaFromPosition(RWVec position)
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

void AkiRobotCpp::barrage()
{
    if (this->enemyPositionsInFiring.empty()) {
        // 初回 -> enemyPositionInsearch の両側 3, 6, 9度に撃つ
        RWVec pos = this->enemyPositionInSearch;
        float angle = this->angleBetweenHeadingDirectionAndWorldPosition(pos);
        if (angle >= 0.0f) {
            this->turnRobotRight(abs(angle));
        } else {
            this->turnRobotLeft(abs(angle));
        }
        if (this->currentState == FliegerhammerAction::FIRING) this->shoot();
        if (this->currentState == FliegerhammerAction::FIRING) this->turnRobotRight(3);
        if (this->currentState == FliegerhammerAction::FIRING) this->shoot();
        if (this->currentState == FliegerhammerAction::FIRING) this->turnRobotLeft(6);
        if (this->currentState == FliegerhammerAction::FIRING) this->shoot();
        if (this->currentState == FliegerhammerAction::FIRING) this->turnRobotRight(9);
        if (this->currentState == FliegerhammerAction::FIRING) this->shoot();
        if (this->currentState == FliegerhammerAction::FIRING) this->turnRobotLeft(12);
        if (this->currentState == FliegerhammerAction::FIRING) this->shoot();
        if (this->currentState == FliegerhammerAction::FIRING) this->turnRobotRight(15);
        if (this->currentState == FliegerhammerAction::FIRING) this->shoot();
        if (this->currentState == FliegerhammerAction::FIRING) this->turnRobotLeft(24);
        if (this->enemyPositionsInFiring.empty()) {
            RWVec prevCorner = this->getCornerCoordinateByArea(this->getNextCorner(this->getAreaFromPosition(this->position()), true));
            angle = this->angleBetweenHeadingDirectionAndWorldPosition(prevCorner);
            if (angle >= 0.0f) {
                this->turnRobotLeft(abs(angle));
            } else {
                this->turnRobotRight(abs(angle));
            }
            this->currentState = FliegerhammerAction::SEARCHING;
        }
    }
    else {
        // 2回目以降
        cout << "(" << enemyPositionInSearch.x << ", " << enemyPositionInSearch.y << ") -> ";
        for (auto pos = enemyPositionsInFiring.begin(), last = enemyPositionsInFiring.end(); pos != last; ++pos) {
            cout << "(" << pos->x << ", " << pos->y << "), ";
        }
        cout << endl;

        if (this->currentTimestamp() - this->bulletHitTimestampList[this->bulletHitTimestampList.size() - 1] > 5.0f) {
            this->enemyPositionInSearch = RWVec(0, 0);
            this->enemyPositionsInFiring.clear();
            this->bulletHitTimestampInSearch = 0.0f;
            this->bulletHitTimestampList.clear();
            this->currentState = FliegerhammerAction::SEARCHING;
            this->cancelActiveAction();
            return;
        }

        float t;
        RWVec p;
        if (this->enemyPositionsInFiring.size() == 1) {
            t = this->bulletHitTimestampInSearch;
            p = this->enemyPositionInSearch;
        } else {
            t = this->bulletHitTimestampList[this->bulletHitTimestampList.size() - 1];
            p = this->enemyPositionsInFiring[this->enemyPositionsInFiring.size() - 1];
        }

        float angle = this->angleBetweenHeadingDirectionAndWorldPosition(p);

        if (angle >= 0.0f) {
            //right
            if (this->currentState == FliegerhammerAction::FIRING) this->turnRobotRight(abs(angle));
            if (this->currentState == FliegerhammerAction::FIRING) this->shoot();
            if (this->currentState == FliegerhammerAction::FIRING) this->turnRobotRight(2);
            if (this->currentState == FliegerhammerAction::FIRING) this->shoot();
            if (this->currentState == FliegerhammerAction::FIRING) this->turnRobotLeft(4);
            if (this->currentState == FliegerhammerAction::FIRING) this->shoot();
            if (this->currentState == FliegerhammerAction::FIRING) this->turnRobotRight(2);
        } else {
            //left
            if (this->currentState == FliegerhammerAction::FIRING) this->turnRobotLeft(abs(angle));
            if (this->currentState == FliegerhammerAction::FIRING) this->shoot();
            if (this->currentState == FliegerhammerAction::FIRING) this->turnRobotLeft(2);
            if (this->currentState == FliegerhammerAction::FIRING) this->shoot();
            if (this->currentState == FliegerhammerAction::FIRING) this->turnRobotRight(4);
            if (this->currentState == FliegerhammerAction::FIRING) this->shoot();
            if (this->currentState == FliegerhammerAction::FIRING) this->turnRobotLeft(2);
        }
    }
}


// }}}



// {{{ callbacks

void AkiRobotCpp::gotHit()
{
    cout << "gotHit!!" << endl;

    this->ownHp--;
    this->gotHitTimestampList.push_back(this->currentTimestamp());

    // 3秒の間に2発当たるまでは気にしない
    if (this->gotHitTimestampList.size() < 2 || this->gotHitTimestampList[this->gotHitTimestampList.size()-1] -  this->gotHitTimestampList[this->gotHitTimestampList.size()-2] > 3.0f) {
        return;
    }

    if (emergencyActing) {
        return;
    }

    this->gotHitTimestampList.clear();
    this->emergencyActing = true;

    if (!this->enemyPositionsInFiring.empty() && this->bulletHitTimestampInSearch == 0.0f) {
        this->enemyPositionInSearch = this->enemyPositionsInFiring[this->enemyPositionsInFiring.size() - 1];
        this->bulletHitTimestampInSearch = this->bulletHitTimestampList[this->bulletHitTimestampList.size() - 1];
    }

    if (this->currentState == FliegerhammerAction::FIRING) {
        if (this->enemyHp > this->ownHp) {
            this->cancelActiveAction();
            this->moveToCorner(this->getNextCorner(this->getAreaFromPosition(this->position())));
        }
    }

    if (this->currentState == FliegerhammerAction::SEARCHING) {
        this->cancelActiveAction();
        this->moveToCorner(this->getNextCorner(this->getAreaFromPosition(this->position())));
    }

    if (this->bulletHitTimestampInSearch == 0.0f) {
        this->currentState = FliegerhammerAction::RESET_SEARCHING;
    } else {
        this->currentState = FliegerhammerAction::RESET_FIRING;
    }

    this->emergencyActing = false;
    return;
}

void AkiRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
}

void AkiRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    cout << "bulletHit!! (" << enemyPosition.x << ", " << enemyPosition.y << ")" << endl;

    this->enemyHp--;

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

void AkiRobotCpp::scannedRobotAtPosition(RWVec position)
{
    cout << "scanned!! (" << position.x << ", " << position.y << ")" << endl;

    if (this->currentState != FliegerhammerAction::FIRING) {
        this->cancelActiveAction();
        this->bulletHitTimestampInSearch = this->currentTimestamp();
        this->enemyPositionInSearch = position;
        this->bulletHitTimestampList.clear();
        this->enemyPositionsInFiring.clear();
        this->bulletHitTimestampList.push_back(this->currentTimestamp());
        this->enemyPositionsInFiring.push_back(position);

        if (!this->emergencyActing && this->enemyHp > this->ownHp) {
            this->moveToCorner(this->getNextCorner(this->getAreaFromPosition(this->position())));
        }

        this->currentState = FliegerhammerAction::FIRING;
        return;
    }
    this->bulletHitTimestampList.push_back(this->currentTimestamp());
    this->enemyPositionsInFiring.push_back(position);
}

// }}}
