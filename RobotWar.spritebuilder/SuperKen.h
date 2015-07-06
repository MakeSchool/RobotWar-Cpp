//
//  SuperKen.h
//  RobotWar
//
//  Created by Ken Watanabe on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__SuperKen__
#define __RobotWar__SuperKen__

#include "RobotCpp.h"

namespace SuperKenAction {
    
    enum SuperKenAction
    {
        SCANNING,
        FIRING,
        ESCAPE
    };
}

class SuperKen : public RobotCpp
{
public:
    SuperKen();
    
    void run() override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    
private:
    SuperKenAction::SuperKenAction currentState;
    float timeSinceLastEnemyHit;
};


#endif /* defined(__RobotWar__SuperKen__) */
