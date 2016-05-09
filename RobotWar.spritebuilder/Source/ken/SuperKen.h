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
        MOVE_OTHERSIDE,
        MOVE_VERTICAL,
        MOVE_AND_SHOT
    };
}
namespace SuperKenStrategy {
    enum SuperKenStrategy
    {
        ALASKA_THUNDER,
        CORNER_SHOT
    };
}


class SuperKen : public RobotCpp
{
public:
    SuperKen();
    
    void run() override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    void gotHit() override;
    
private:
    SuperKenAction::SuperKenAction currentState;
    SuperKenStrategy::SuperKenStrategy currentStrategy;
    float timeSinceLastEnemyHit;
    RWVec lastEnemyPosition;
    int enemyHitPoint;
    int comboDamage;
    
    void stepAlaskaThunder();
    void bulletHitEnemyWithAlaskaThunder(RWVec enemyPosition);
    void scannedRobotAtPositionWithAlaskaThunder(RWVec position);
    void gotHitWithAlaskaThunder();
    
    void stepCornerShot();
    void bulletHitEnemyWithCornerShot(RWVec enemyPosition);
    void scannedRobotAtPositionWithCornerShot(RWVec position);
    void gotHitWithCornerShot();

    bool isAdvantage();
    bool isUp(float y);
    bool isLeft(float x);
    float getOppositeX(float x);
    float getOppositeY(float y);
    void shootPoint(RWVec point);
    void goToPoint(RWVec point);
};


#endif /* defined(__RobotWar__SuperKen__) */
