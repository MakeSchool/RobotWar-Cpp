//
//  MyRobot.h
//  RobotWar
//
//  Created by Haruki Nakano on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__MyRobot__
#define __RobotWar__MyRobot__

#include "RobotCpp.h"

namespace HarukiRobotCppAction {

enum HarukiRobotCppAction { INIT, MOVE_TO_CORNER, PAUSE, GO_UP, GO_DOWN, FIRING };
}

class HarukiRobotCpp : public RobotCpp {
public:
    HarukiRobotCpp();

    void run() override;
    void scannedRobotAtPosition(RWVec position) override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void printStates();

private:
    HarukiRobotCppAction::HarukiRobotCppAction currentState;
    RWVec lastPosition;
    RWVec lastKnownPosition;
    float lastKnownPositionTimestamp;
};

#endif /* defined(__RobotWar__MyRobot__) */
