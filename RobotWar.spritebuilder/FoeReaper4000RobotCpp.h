//
//  FoeReaper4000.h
//  RobotWar
//
//  Created by Liang Fan on 7/7/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__FoeReaper4000__
#define __RobotWar__FoeReaper4000__

#include "RobotCpp.h"

namespace FoeReaper4000RobotConstant {
    static const int ROBOT_WITHD = 50;
}

class FoeReaper4000RobotCpp : public RobotCpp
{
public:
    FoeReaper4000RobotCpp();
    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
private:
    int generateRandomNumber(int begin, int end);
    float generateRandomNumber(float begin, float end);
    void optimizeMove(int degree, int distance);
    void randomWalk(int beginDegree = 0, int endDegree = 360, int beginDistance = 100, int EndDistance = 200);
    void showLog();
    void optimizeGunPosition();
    RWVec lastEnemyPos;
    void shootToPos(RWVec pos);
    
};

#endif /* defined(__RobotWar__FoeReaper4000__) */
