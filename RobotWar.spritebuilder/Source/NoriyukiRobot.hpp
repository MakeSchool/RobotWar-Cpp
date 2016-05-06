//
//  NoriyukiRobot.hpp
//  RobotWar
//
//  Created by Noriyuki Matsuoka on 2016/05/06.
//  Copyright © 2016年 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__NoriyukiRobot__
#define __RobotWar__NoriyukiRobot__

#include "RobotCpp.h"

namespace NoriyukiRobotAction {
    
    enum NoriyukiRobotAction
    {
        MOVETOBOTTOM,
        MOVETOTOP,
        SCANNING,
        FIRING
    };
}

class NoriyukiRobot : public RobotCpp
{
public:
    NoriyukiRobot();
    
    void run() override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    NoriyukiRobotAction::NoriyukiRobotAction whichBetterBottomOrTop();
    void moveDirectly(RWVec position);
    
private:
    NoriyukiRobotAction::NoriyukiRobotAction currentState;
    float timeSinceLastEnemyHit;
};

#endif /* defined(__RobotWar__NoriyukiRobot__) */
