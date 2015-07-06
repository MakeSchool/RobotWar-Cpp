//
//  LiveRobotCpp.h
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__LiveRobotCpp__
#define __RobotWar__LiveRobotCpp__

#include "RobotCpp.h"

namespace LiveRobotCppAction {
    
    enum LiveRobotCppAction
    {
        FIRST_MOVE,
        TURN_AROUND,
        FIRING,
        SEARCHING,
        SCATTER,
        WAITING
    };
}

class LiveRobotCpp : public RobotCpp
{
public:    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    
private:
    LiveRobotCppAction::LiveRobotCppAction currentState;
    RWVec lastKnownPosition;
    float lastKnownPositionTimestamp;
    int actionIndex;
    
    void performNextFiringAction();
    void performNextFirstMove();
    
    void setCurrentState(LiveRobotCppAction::LiveRobotCppAction newState);
};

#endif /* defined(__RobotWar__LiveRobotCpp__) */
