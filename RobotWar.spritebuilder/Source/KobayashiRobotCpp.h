//
//  KobayashiRobotCpp.h
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__KobayashiRobotCpp__
#define __RobotWar__KobayashiRobotCpp__

#include "RobotCpp.h"


namespace KobayashiRobotCppAction {
    
    enum KobayashiRobotCppAction
    {
        SCANNING,
        FIRING
    };
}


class KobayashiRobotCpp : public RobotCpp
{
public:
    KobayashiRobotCppAction::KobayashiRobotCppAction currentAction;
    float timeSinceLastEnemyHit;
    
    KobayashiRobotCpp();
    void run() override;
    void scaningAction();
    void firingAction();
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    RWVec getCenterPoint();
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    
};

#endif /* defined(__RobotWar__KobayashiRobotCpp__) */