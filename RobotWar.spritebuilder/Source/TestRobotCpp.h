//
//  TestRobotCpp.h
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__TestRobotCpp__
#define __RobotWar__TestRobotCpp__

#include "RobotCpp.h"

class TestRobotCpp : public RobotCpp
{
public:
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    
};

#endif /* defined(__RobotWar__TestRobotCpp__) */
