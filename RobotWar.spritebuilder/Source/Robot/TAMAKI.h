//
//  TAMAKI.h
//  RobotWar
//
//  Created by ryutamaki on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__TAMAKI__
#define __RobotWar__TAMAKI__

#include "RobotCpp.h"
#include <vector>

namespace TAMAKIAction {
    enum TAMAKIAction
    {
        PREPARE_FOR_SNIPE,
        SEARCH_USING_SNIPE,
        SEARCH_USING_SNIPE_CLOSE_TO_LAST_ENEMY_POSITION,
        SNIPE,
        ESCAPE,
    };
}

class TAMAKI : public RobotCpp
{
public:
    TAMAKI();

    void run() override;
    void scannedRobotAtPosition(RWVec position) override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;

private:
    TAMAKIAction::TAMAKIAction state;
    std::vector<RWVec> enemyPositions;
    int enemyHitpoint;
    time_t lastHitTime;
    int escapeCounter;

    void prepareForSnipe();
    void searchUsingSnipe();
    void searchUsingSnipeCloseToLastEnemyPosition();
    void snipe();
    void escape();

    void setState(TAMAKIAction::TAMAKIAction state);


    void printRWVec(RWVec vec);
    void printRWSize(RWSize size);

    int radianToDegree(float rad);
    int degreeBetweenRWVecs(RWVec vec1, RWVec vec2);
};

#endif /* defined(__RobotWar__TAMAKI__) */
