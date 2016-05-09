//
//  SimpleRobotCpp.h
//  RobotWar
//
//  Created by Daniel Haaser on 7/6/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#ifndef __EtsukoMiyazato__
#define __EtsukoMiyazato__

#include "RobotCpp.h"

namespace EtsukoMiyazatoAction {
    
    enum EtsukoMiyazatoAction
    {
        DEFAULT,
        TURN_AROUND,
        FIRING,
        SEARCHING,
        MOVE_STRAIGHT,
    };
}

class EtsukoMiyazato : public RobotCpp
{
public:
    EtsukoMiyazato();
    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void scannedRobotAtPosition(RWVec position) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
private:
    int actionIndex = 0;
    int searchingIndex = 0;
    int firstActionIndex = 0;
    int firstActionCount = 0;
    
    bool isDebug = false;
    bool isTurnGunRight = false;
    bool isFirstAction = false;
    bool isInitialize = false;
    
    RWVec lastKnownPosition;
    float lastKnownPositionTimestamp;
    RobotWallHitSide::RobotWallHitSide lastWallSide;
    float lastHitWallAngle;
    
    EtsukoMiyazatoAction::EtsukoMiyazatoAction currentState;
    EtsukoMiyazatoAction::EtsukoMiyazatoAction previousState;
    
    void performNextDefaultAction();
    void performNextTurnAroundAction();
    void performNextFiringAction();
    void performNextSearchingAction();
    void performNextMoveStraightAction();
    
    void setCurrentState(EtsukoMiyazatoAction::EtsukoMiyazatoAction newState);
};

#endif /* defined(EtsukoMiyazato) */
