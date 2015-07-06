//
//  HarukiRobot.cpp
//  RobotWar
//
//  Created by Haruki Nakano on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include "HarukiRobotCpp.h"
#include <math.h>

#if 1
#include <iostream>
#define DEBUG_PRINT(x)               \
    do {                             \
        std::cout << x << std::endl; \
    } while (0)
#else
#define DEBUG_PRINT(x)
#endif

#define INF 99999

HarukiRobotCpp::HarukiRobotCpp() {
    this->currentState = HarukiRobotCppAction::INIT;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    this->gunAngleMoved = 0.0f;
}

void HarukiRobotCpp::run() {
    while (true) {
        switch (this->currentState) {
            case HarukiRobotCppAction::INIT:
                DEBUG_PRINT("Move to corner");
                this->currentState = HarukiRobotCppAction::MOVE_TO_CORNER;
                break;
            case HarukiRobotCppAction::MOVE_TO_CORNER:
                this->moveBack(50);
                break;

            case HarukiRobotCppAction::MOVE:
                if (this->moveUp) {
                    this->moveAhead(50);
                } else {
                    this->moveBack(50);
                }
                if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 16.0f) {
                    this->currentState = HarukiRobotCppAction::HIT_AND_AWAY;
                }
                break;
            case HarukiRobotCppAction::HIT_AND_AWAY:
                if (this->moveUp) {
                    int distance = std::min(this->robotBoundingBox().size.height, this->robotBoundingBox().size.width);
                    DEBUG_PRINT(distance);
                    this->moveAhead(distance);
                    this->shoot();
                    this->shoot();
                    this->shoot();
                } else {
                    int distance = std::min(this->robotBoundingBox().size.height, this->robotBoundingBox().size.width);
                    DEBUG_PRINT(distance);
                    this->moveBack(distance);
                    this->shoot();
                    this->shoot();
                    this->shoot();
                }
                break;
            case HarukiRobotCppAction::PAUSE:
                // this->cancelActiveAction();
                break;
            case HarukiRobotCppAction::FIRING:
                if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f) {
                    // restore the position of the gun.
                    float angle = this->gunAngleMoved;
                    if (angle >= 0.0f) {
                        this->turnGunLeft(fabsf(angle));
                    } else {
                        this->turnGunRight(fabsf(angle));
                    }
                    this->gunAngleMoved = 0.0;
                    this->currentState = HarukiRobotCppAction::MOVE;
                } else {
                    float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);

                    this->gunAngleMoved += angle;
                    if (angle >= 0.0f) {
                        this->turnGunRight(fabsf(angle));
                    } else {
                        this->turnGunLeft(fabsf(angle));
                    }

                    this->shoot();
                }
                break;
        }

        lastPosition = this->position();
    }
}

void HarukiRobotCpp::scannedRobotAtPosition(RWVec position) {
    if (this->currentState == HarukiRobotCppAction::MOVE_TO_CORNER) {
        // Do nothing utill we reach the corner.
        return;
    }
    if (this->currentState != HarukiRobotCppAction::FIRING) {
        this->cancelActiveAction();
    }

    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    this->currentState = HarukiRobotCppAction::FIRING;
}

void HarukiRobotCpp::gotHit() {
    if (this->currentState == HarukiRobotCppAction::MOVE_TO_CORNER) {
        // Do nothing utill we reach the corner.
        return;
    } else if (this->currentState != HarukiRobotCppAction::MOVE) {
        this->currentState = HarukiRobotCppAction::MOVE;
    }
}

void HarukiRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) {
    this->cancelActiveAction();

    if (this->currentState == HarukiRobotCppAction::MOVE_TO_CORNER) {
        this->turnRobotLeft(90);
        this->turnGunRight(90);
        this->currentState = HarukiRobotCppAction::MOVE;
        printStates();
    } else if (this->currentState == HarukiRobotCppAction::MOVE ||
               this->currentState == HarukiRobotCppAction::HIT_AND_AWAY) {
        this->moveUp = !this->moveUp;
    }
}

void HarukiRobotCpp::bulletHitEnemy(RWVec enemyPosition) {
}

void HarukiRobotCpp::printStates() {
    DEBUG_PRINT("Position: " << this->position().x << ", " << this->position().y);
    DEBUG_PRINT("Gun head direction: " << this->gunHeadingDirection().x << ", " << this->gunHeadingDirection().y);
    DEBUG_PRINT("Gun angle: " << this->angleBetweenGunHeadingDirectionAndWorldPosition(this->headingDirection()));
}