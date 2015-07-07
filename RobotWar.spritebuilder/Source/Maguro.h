//
//  Maguro.h
//  RobotWar
//
//  Created by Haruki Nakano on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__Maguro__
#define __RobotWar__Maguro__

#include "RobotCpp.h"

namespace MaguroAction {

enum MaguroAction { INIT, ESCAPE, FIRING, AIMING };
}

class Maguro : public RobotCpp {
public:
    Maguro();

    void run() override;
    void scannedRobotAtPosition(RWVec position) override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;

private:
    bool is1P;
    bool moveUp;
    MaguroAction::MaguroAction currentState;
    RWVec lastKnownPosition;
    float lastHitTimestamp;
    float lastKnownPositionTimestamp;
    int enemysHitPoints;
    int shotCount;

    void shotTo(float angle);
};
#endif /* defined(__RobotWar__Maguro__) */
