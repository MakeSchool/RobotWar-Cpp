//
//  RobotTARS.h
//  RobotWar
//
//  Created by Mitsushige Komiya on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__RobotTARS__
#define __RobotWar__RobotTARS__

#include "RobotCpp.h"

namespace RobotTARSAction {
    
    enum RobotTARSAction
    {
        INITIAL,
        OFFENSE_FIRING,
        OFFENSE_SEARCHING,
        OFFENSE_TURN_AROUND,
        ASSAULT_FIRING,
        DEBUGGING,
    };
}

class RobotTARS : public RobotCpp
{
public:
    RobotTARS();
    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    
private:
    RobotTARSAction::RobotTARSAction currentState;
    int damagedPlayer, damagedEnemy;
    RWVec lastKnownPosition;
    float lastKnownPositionTimestamp, lastGotHitTimestamp;
    bool isTargetedByEnemy;
    
    // running methods
    void runInitial();
    void runOffenseFiring();
    void runOffenseSearching();
    void runAssaultFiring();
    void runDebugging();
    
    // action helper methods
    void turnGunToAhead();
    void turnRobot(float angle);
    void turnGun(float angle);
    bool isWinningLife();
    void changeState(RobotTARSAction::RobotTARSAction nextState);
    
    // math helper methods
    float getVectorLenght(RWVec vec);
    float getDotProduct(RWVec vec1, RWVec vec2);
    float convertRadianToDigree(float radian);
};

#endif /* defined(__RobotWar__RobotTARS__) */
