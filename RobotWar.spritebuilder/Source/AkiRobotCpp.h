//
//  AkiRobotCpp.h
//  RobotWar
//
//  Created by Akihito OKUHATA on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__Fliegerhammer__
#define __RobotWar__Fliegerhammer__

#include "RobotCpp.h"
#include <random>

namespace FliegerhammerAction {
    enum FliegerhammerAction {
        DEFAULT,
        TURN_AROUND,
        SEARCHING,
        RESET_SEARCHING,
        FIRING,
        RESET_FIRING
    };
};

namespace FliegerhammerArea {
    enum FliegerhammerArea {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
    };
};


class AkiRobotCpp : public RobotCpp
{
public:
    AkiRobotCpp();

    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;

private:
    FliegerhammerAction::FliegerhammerAction currentState;
    RWVec lastKnownPosition;
    float lastKnownPositionTimestamp;
    int actionIndex;

    void performNextFiringAction();
    void performNextDefaultAction();
    void performNextTurnAroundAction();
    void performNextSearchingAction();

    RWVec enemyPositionInSearch;
    float bulletHitTimestampInSearch;
    std::vector<RWVec> enemyPositionsInFiring;
    std::vector<float> bulletHitTimestampList;
    std::vector<float> gotHitTimestampList;

    FliegerhammerArea::FliegerhammerArea getAreaFromPosition(RWVec position);

    FliegerhammerArea::FliegerhammerArea getNextCorner(FliegerhammerArea::FliegerhammerArea area, bool counterClockwise = false);
    void moveToTriangle(FliegerhammerArea::FliegerhammerArea currentArea, int numOfCornder);
    void moveToCorner(FliegerhammerArea::FliegerhammerArea area);
    RWVec getCornerCoordinateByArea(FliegerhammerArea::FliegerhammerArea area);
    void barrage();

    std::default_random_engine randomGenerator;

    int enemyHp;
    int ownHp;

    bool emergencyActing;

};


#endif /* defined(__RobotWar__Fliegerhammer__) */
