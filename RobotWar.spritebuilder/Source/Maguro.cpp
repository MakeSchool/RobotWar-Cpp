//
//  Maguro.cpp
//  RobotWar
//
//  Created by Haruki Nakano on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include "Maguro.h"

#include <iostream>
#include <math.h>

Maguro::Maguro()
    : currentState(MaguroAction::INIT),
      lastHitTimestamp(currentTimestamp()),
      lastKnownPositionTimestamp(currentTimestamp()),
      enemysHitPoints(20),
      shotCount(0) {
}

void Maguro::run() {
    while (true) {
        switch (this->currentState) {
            case MaguroAction::INIT:
                this->moveBack(20);
                break;

            case MaguroAction::ESCAPE:
                moveUp ? this->moveAhead(20) : this->moveBack(20);
                break;

            case MaguroAction::FIRING:
                if (this->currentTimestamp() - this->lastKnownPositionTimestamp < 5.0f) {
                    this->shotTo(this->angleBetweenGunHeadingDirectionAndWorldPosition(lastKnownPosition));
                } else {
                    this->currentState = MaguroAction::AIMING;
                }
                break;

            case MaguroAction::AIMING:
                RWSize size = this->arenaDimensions();
                RWVec pos = RWVec(size.width, size.height);
                RWVec pos2 = RWVec(size.width, 0.0f);
                float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(pos) + shotCount * 2.0f;
                shotTo(angle);
                shotCount = angle > angleBetweenGunHeadingDirectionAndWorldPosition(pos2) ? 0 : shotCount + 1;
                break;
        }
    }
}

void Maguro::scannedRobotAtPosition(RWVec position) {
    if (this->currentState == MaguroAction::INIT) {
        return;
    } else if (this->currentState == MaguroAction::ESCAPE && this->hitPoints() < this->enemysHitPoints) {
        return;
    }

    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    this->currentState = MaguroAction::FIRING;
}

void Maguro::gotHit() {
    this->currentState = MaguroAction::ESCAPE;
    this->lastHitTimestamp = this->currentTimestamp();
}

void Maguro::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) {
    this->cancelActiveAction();

    if (this->currentState == MaguroAction::INIT) {
        this->turnRobotLeft(90);
        this->currentState = MaguroAction::AIMING;
    } else if (this->currentState == MaguroAction::ESCAPE) {
        this->moveUp = !this->moveUp;
        this->currentState = MaguroAction::AIMING;
    }
}

void Maguro::bulletHitEnemy(RWVec enemyPosition) {
    this->enemysHitPoints--;
    this->lastKnownPosition = enemyPosition;
    this->lastKnownPositionTimestamp = this->currentTimestamp();

    if (this->currentState != MaguroAction::ESCAPE) {
        this->currentState = MaguroAction::FIRING;
        this->cancelActiveAction();
    }
}

void Maguro::shotTo(float angle) {
    this->shotCount++;
    if (angle >= 0.0f) {
        this->turnGunRight(fabsf(angle));
    } else {
        this->turnGunLeft(fabsf(angle));
    }
    this->shoot();
}