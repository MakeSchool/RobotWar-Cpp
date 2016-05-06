//
//  Tokutya.hpp
//  RobotWar
//
//  Created by Masahiro Ito on 2016/05/06.
//  Copyright © 2016年 MakeGamesWithUs. All rights reserved.
//

#ifndef Tokutya_hpp
#define Tokutya_hpp

#include "RobotCpp.h"

namespace TokutyaAction {
    
    enum TokutyaAction
    {
        DEFAULT,
        TURN_AROUND,
        FIRING,
        SEARCHING
    };
}

class Tokutya : public RobotCpp
{
public:
    Tokutya();
    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    
private:
    TokutyaAction::TokutyaAction currentState;
    RWVec lastKnownPosition;
    float lastKnownPositionTimestamp;
    int actionIndex;
    
    void performNextFiringAction();
    void performNextSearchingAction();
    void performNextDefaultAction();
    
    void setCurrentState(TokutyaAction::TokutyaAction newState);
};


#endif /* Tokutya_hpp */
