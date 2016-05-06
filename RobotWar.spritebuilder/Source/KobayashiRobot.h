//
//  KobayashiRobot.h
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__KobayashiRobot__
#define __RobotWar__KobayashiRobot__

#include "RobotCpp.h"

namespace KobayashiRobotAction {
    
    enum KobayashiRobotAction
    {
        DEFAULT,
        TURN_AROUND,
        FIRING,
        SEARCHING
    };
}

class KobayashiRobot : public RobotCpp
{
public:
    KobayashiRobot();
    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    
private:
    KobayashiRobotAction::KobayashiRobotAction currentState;
    RWVec lastKnownPosition;
    float lastKnownPositionTimestamp;
    int actionIndex;
    
    void performNextFiringAction();
    void performNextSearchingAction();
    void performNextDefaultAction();
    
    void setCurrentState(KobayashiRobotAction::KobayashiRobotAction newState);
};

#endif /* defined(__RobotWar__KobayashiRobot__) */
