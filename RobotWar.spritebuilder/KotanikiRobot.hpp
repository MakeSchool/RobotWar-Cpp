//
//  KotanikiRobot.hpp
//  RobotWar
//
//  Created by Yuki Kotani on 2016/05/06.
//  Copyright © 2016年 MakeGamesWithUs. All rights reserved.
//

#ifndef KotanikiRobot_hpp
#define KotanikiRobot_hpp

#include "RobotCpp.h"

namespace KotanikiRobotAction {
    
    enum KotanikiRobotAction
    {
        FIRST_MOVE,
        SECOND_MOVE,
        DEFAULT,
        TURN_AROUND,
        FIRING,
        SEARCHING
    };
}

class KotanikiRobot : public RobotCpp
{
public:
    KotanikiRobot();
    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    //void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    
private:
    KotanikiRobotAction::KotanikiRobotAction currentState;
    RWVec lastKnownPosition;
    float lastKnownPositionTimestamp;
 
    void setCurrentState(KotanikiRobotAction::KotanikiRobotAction newState);
    
};
#endif /* KotanikiRobot_hpp */
