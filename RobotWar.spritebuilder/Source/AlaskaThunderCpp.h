//
//  AlaskaThunder.h
//  RobotWar
//
//  Created by Shota Oiyama on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__AlaskaThunder__
#define __RobotWar__AlaskaThunder__

#include <stdio.h>
#include "RobotCpp.h"

namespace AlaskaThunderCppAction {
    
    enum AlaskaThunderCppAction
    {
        DEFAULT,
        BLAST,
        FIRING
    };
}

class AlaskaThunderCpp : public RobotCpp
{
public:
    AlaskaThunderCpp();
    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    void lookRight();
    
private:
    AlaskaThunderCppAction::AlaskaThunderCppAction currentState;
    RWVec lastKnownPosition;
    float lastKnownPositionTimestamp;
    float timeSinceLastEnemyHit;
    float gunAngleMoved;

};

#endif /* defined(__RobotWar__AlaskaThunder__) */