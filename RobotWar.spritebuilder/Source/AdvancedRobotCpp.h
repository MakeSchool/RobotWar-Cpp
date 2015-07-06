//
//  AdvancedRobotCpp.h
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__AdvancedRobotCpp__
#define __RobotWar__AdvancedRobotCpp__

#include "RobotCpp.h"

namespace AdvancedRobotCppAction {
    
    enum AdvancedRobotCppAction
    {
        DEFAULT,
        TURN_AROUND,
        FIRING,
        SEARCHING
    };
}

class AdvancedRobotCpp : public RobotCpp
{
public:
    AdvancedRobotCpp();
    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    
private:
    AdvancedRobotCppAction::AdvancedRobotCppAction currentState;
    RWVec lastKnownPosition;
    float lastKnownPositionTimestamp;
};


#endif /* defined(__RobotWar__AdvancedRobotCpp__) */
