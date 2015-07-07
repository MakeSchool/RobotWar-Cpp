//
//  CookieRobotCpp.h
//  RobotWar
//
//  Created by Tingting Gu on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__CookieRobotCpp__
#define __RobotWar__CookieRobotCpp__

#include "RobotCpp.h"

namespace CookieRobotCppAction {
    
    enum CookieRobotCppAction
    {
        INIT,
        FIRING,
        SEARCHING,
        ESCAPING,
        TURN_AROUND,
    };
}

class CookieRobotCpp : public RobotCpp
{
public:
    CookieRobotCpp();
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    
private:
    CookieRobotCppAction::CookieRobotCppAction currentState;
    RWVec lastKnownPosition;
    float lastKnownPositionTimestamp;
    int actionIndex;
    int enemysHitPoints;
    
    void gameInit();
    void gunFiring();
    void searching();
    void escaping();
    void performNextDefaultAction();
    void performFiringBack();
    
    void setCurrentState(CookieRobotCppAction::CookieRobotCppAction newState);
};

#endif /* defined(__RobotWar__CookieRobotCpp__) */
