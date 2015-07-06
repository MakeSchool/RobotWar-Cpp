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

class FoeReaper4000RobotCpp : public RobotCpp
{
public:
    FoeReaper4000RobotCpp();
    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
 //   void bulletHitEnemy(RWVec enemyPosition) override;
 //   void scannedRobotAtPosition(RWVec position) override;
private:
    int generateRandomNumber(int begin, int end);
    void optimizeMove(int degree, int distance);
    void randomWalk(int beginDegree = 0, int endDegree = 360, int beginDistance = 0, int EndDistance = 100);
    void showLog();
    
};

#endif /* defined(__RobotWar__FoeReaper4000__) */
