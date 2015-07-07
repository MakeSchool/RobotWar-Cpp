//
//  NewAdvancedRobotCpp.h
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__NewAdvancedRobotCpp__
#define __RobotWar__NewAdvancedRobotCpp__

#include "RobotCpp.h"

namespace NewAdvancedRobotCppAction {
    
    enum NewAdvancedRobotCppAction
    {
        DEFAULT,
        TURN_AROUND,
        FIRING,
        SEARCHING
    };
}

class NewAdvancedRobotCpp : public RobotCpp
{
public:
    NewAdvancedRobotCpp();
    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    
private:
    NewAdvancedRobotCppAction::NewAdvancedRobotCppAction currentState;
    RWVec lastKnownPosition;
    float lastKnownPositionTimestamp;
    int actionIndex;
    
    void performNextFiringAction();
    void performNextSearchingAction();
    void performNextDefaultAction();
    
    void setCurrentState(NewAdvancedRobotCppAction::NewAdvancedRobotCppAction newState);
};

#endif /* defined(__RobotWar__NewAdvancedRobotCpp__) */
