//
//  AsadaRobotCpp.hpp
//  RobotWar
//
//  Created by Shuhei Asada on 2016/05/06.
//  Copyright © 2016年 MakeGamesWithUs. All rights reserved.
//

#ifndef AsadaRobotCpp_hpp
#define AsadaRobotCpp_hpp

#include "RobotCpp.h"

class AsadaRobotCpp : public RobotCpp
{
public:
    
    enum State {
        STATE_BACK,
        STATE_AHEAD,
        STATE_TURNRIGHT90,
        STATE_TURNLEFT90,
        STATE_TURNENEMY,
        STATE_NONE
    };
    
    void init();
    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void scannedRobotAtPosition(RWVec position) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    
private:
    State m_State;
    State m_Next;
    RWVec m_LastEnemyPosition;
    int m_StateFrame;
    
};

#endif /* AsadaRobotCpp_hpp */
