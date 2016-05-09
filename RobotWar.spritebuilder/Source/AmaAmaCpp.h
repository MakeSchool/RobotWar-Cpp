#ifndef __RobotWar__AmaAmaCpp__
#define __RobotWar__AmaAmaCpp__

#include "RobotCpp.h"

namespace AmaAmaCppAction {
    
    enum AmaAmaCppAction
    {
        DEFAULT,
        TURN_AROUND,    //not use
        FIRING,         //Find Enemy and Shot
        SEARCHING,
        START           //Start Only
    };
}

class AmaAmaCpp : public RobotCpp
{
public:
    AmaAmaCpp();
    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    
private:
    AmaAmaCppAction::AmaAmaCppAction currentState;
    RWVec lastKnownPosition;
    float lastKnownPositionTimestamp;
    int actionIndex;
    
    void performNextFiringAction();
    void performNextSearchingAction();
    void performNextDefaultAction();
    void performNextStartAction();
    
    void setCurrentState(AmaAmaCppAction::AmaAmaCppAction newState);
    
    void turnGun(float angle);
    void turnRobot(float angle);
    void Aim();
};

#endif /* defined(__RobotWar__AmaAmaCpp__) */
