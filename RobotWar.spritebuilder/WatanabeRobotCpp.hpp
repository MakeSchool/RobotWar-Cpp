//
//  WatanabeRobotCpp.hpp
//  RobotWar
//
//  Created by Shintaro Watanabe on 2016/05/06.
//  Copyright © 2016年 MakeGamesWithUs. All rights reserved.
//

#ifndef WatanabeRobotCpp_hpp
#define WatanabeRobotCpp_hpp

#include "RobotCpp.h"
#include "math.h"

namespace WatanabeRobotState {
    enum WatanabeRobotState
    {
        DEFAULT,
        SEARCHING,
        FIRING
    };
}

class WatanabeRobotCpp : public RobotCpp
{
public:
    WatanabeRobotCpp();
    
    void run() override;
    void gotHit() override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    
private:
    enum vectorState
    {
        Left,
        Right
    };
    void defaultAction();
    void searchAction();
    void firingAction();
    
    WatanabeRobotState::WatanabeRobotState currentState;
    vectorState currentVectorState;
    vectorState lastVectorState;
    RWVec lastKnownPosition;
    int bulletCount;
    int hitWallCount;
};

#endif /* WatanabeRobotCpp_hpp */
