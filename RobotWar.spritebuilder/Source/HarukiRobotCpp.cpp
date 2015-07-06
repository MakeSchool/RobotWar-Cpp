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

            case HarukiRobotCppAction::GO_UP:
                this->moveAhead(50);
                break;
            case HarukiRobotCppAction::GO_DOWN:
                this->moveBack(50);
                break;
            case HarukiRobotCppAction::PAUSE:
                // this->cancelActiveAction();
                break;
            case HarukiRobotCppAction::FIRING:
                if (this->currentTimestamp() - this->lastKnownPositionTimestamp > 1.0f) {
                    this->currentState = HarukiRobotCppAction::GO_UP;
                } else {
                    float angle = this->angleBetweenGunHeadingDirectionAndWorldPosition(this->lastKnownPosition);

                    if (angle >= 0) {
                        this->turnGunRight(fabsf(angle));
                    } else {
                        this->turnGunLeft(fabsf(angle));
                    }

                    this->shoot();
                }
                break;
        }
    }
}

void HarukiRobotCpp::scannedRobotAtPosition(RWVec position) {
    if (this->currentState != HarukiRobotCppAction::FIRING) {
        this->cancelActiveAction();
    }

    this->lastKnownPosition = position;
    this->lastKnownPositionTimestamp = this->currentTimestamp();
    this->currentState = HarukiRobotCppAction::FIRING;
}

void HarukiRobotCpp::gotHit() {
}

void HarukiRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) {
    this->cancelActiveAction();

    if (this->currentState == HarukiRobotCppAction::MOVE_TO_CORNER) {
        this->turnRobotLeft(90);
        this->turnGunRight(90);
        this->currentState = HarukiRobotCppAction::GO_UP;
        printStates();
    } else if (this->currentState == HarukiRobotCppAction::GO_UP) {
        this->currentState = HarukiRobotCppAction::GO_DOWN;
    } else if (this->currentState == HarukiRobotCppAction::GO_DOWN) {
        this->currentState = HarukiRobotCppAction::GO_UP;
    }
}

void HarukiRobotCpp::bulletHitEnemy(RWVec enemyPosition) {
}

void HarukiRobotCpp::printStates() {
    DEBUG_PRINT("Position: " << this->position().x << ", " << this->position().y);
    DEBUG_PRINT("Gun head direction: " << this->gunHeadingDirection().x << ", " << this->gunHeadingDirection().y);
    DEBUG_PRINT("Gun angle: " << this->angleBetweenGunHeadingDirectionAndWorldPosition(this->headingDirection()));
}