//
//  TurretRobotCpp.h
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__TurretRobotCpp__
#define __RobotWar__TurretRobotCpp__

#include "RobotCpp.h"

namespace TurretRobotCppAction {
    
    enum TurretRobotCppAction
    {
        SCANNING,
        FIRING
    };
}

class TurretRobotCpp : public RobotCpp
{
public:
    TurretRobotCpp();
    
    void run() override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    
private:
    TurretRobotCppAction::TurretRobotCppAction currentState;
    float timeSinceLastEnemyHit;
};

#endif /* defined(__RobotWar__TurretRobotCpp__) */
