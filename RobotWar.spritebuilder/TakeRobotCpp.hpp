//
//  TakeRobotCpp.hpp
//  RobotWar
//
//  Created by Takehiro Yoshihama on 2016/05/06.
//  Copyright © 2016年 MakeGamesWithUs. All rights reserved.
//

#ifndef TakeRobotCpp_hpp
#define TakeRobotCpp_hpp

#include "RobotCpp.h"

class TakeRobotCpp : public RobotCpp
{
public:
    TakeRobotCpp();
    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void scannedRobotAtPosition(RWVec enemyPosition) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    
private:
    RWVec lpos;
    RWVec epos;
};


#endif /* TakeRobotCpp_hpp */
