
#ifndef GahakuRobotCpp_hpp
#define GahakuRobotCpp_hpp

#include "RobotCpp.h"

namespace GahakuRobotCppAction {
    
    
}

class GahakuRobotCpp : public RobotCpp
{
public:
    GahakuRobotCpp();
    
    void run() override;
    void gotHit() override;
    void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) override;
    void bulletHitEnemy(RWVec enemyPosition) override;
    void scannedRobotAtPosition(RWVec position) override;
    
private:
    
    enum State {
        STATE_START,
        STATE_NONE,
        STATE_SCANNING
    };
    
    enum Location{
        LOCATION_START,
        LOCATION_BUTTOM_LEFT,
        LOCATION_TOP_RIGHT,
        LOCATION_CENTER
    };
    
    RWVec lastKnownPosition;
    float lastKnownPositionTimestamp;
    int actionIndex;
    
    float START_BOMBS;
    
    State c_State;
    Location c_Location;
    
private:
    
    void performNextFiringAction();
    void performNextSearchingAction();
    void performNextDefaultAction();
    
    void changeDirectionToPointAhead(RWVec pos);
    void changeDirectionToPointBack(RWVec pos);
    void changeGunDirectionToPoint(RWVec pos);
    float getDistanceToPoint(RWVec pos);
    void moveAheadToPoint(RWVec);
    void moveBackToPoint(RWVec);
    void moveToNearestCorner();
    void moveToTopRightCorner();
    void moveToButtomLeftCorner();
};


#endif /* GahakuRobotCpp_hpp */