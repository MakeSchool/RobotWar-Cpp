
#include "GahakuRobotCpp.h"
#include "stdio.h"
#include <math.h>

GahakuRobotCpp::GahakuRobotCpp()
{
    this->START_BOMBS = 60;
    this->c_State = STATE_START;
    this->lastKnownPositionTimestamp = currentTimestamp();
}

void GahakuRobotCpp::changeDirectionToPointAhead(RWVec pos){
    float angle = angleBetweenHeadingDirectionAndWorldPosition(pos);
    
    if(angle > 0) turnRobotRight((int) angle);
    else turnRobotLeft((int) - angle);
}

void GahakuRobotCpp::changeDirectionToPointBack(RWVec pos){
    float angle = angleBetweenHeadingDirectionAndWorldPosition(pos);
    
    if(angle > 0) turnRobotLeft(180 - (int) angle);
    else turnRobotRight(180 + (int)angle);
}

void GahakuRobotCpp::changeGunDirectionToPoint(RWVec pos){
    float angle = angleBetweenGunHeadingDirectionAndWorldPosition(pos);
    
    if(angle > 0) turnGunRight((int) angle);
    else turnGunLeft((int) - angle);
}

float GahakuRobotCpp::getDistanceToPoint(RWVec pos){
    float distance = sqrt((position().x - pos.x) * (position().x - pos.x) + (position().y - pos.y) * (position().y - pos.y));
    
    return distance;
}

void GahakuRobotCpp::moveAheadToPoint(RWVec pos){
    
    changeDirectionToPointAhead(pos);
    moveAhead(getDistanceToPoint(pos));
}

void GahakuRobotCpp::moveBackToPoint(RWVec pos){
    
    changeDirectionToPointBack(pos);
    moveBack(getDistanceToPoint(pos));
}

void GahakuRobotCpp::moveToButtomLeftCorner(){
    moveBackToPoint(RWVec(0,0));
    moveAheadToPoint(RWVec(0,80));
    moveBackToPoint(RWVec(15,20));
    changeGunDirectionToPoint(RWVec(300,300));
    this->c_Location = LOCATION_BUTTOM_LEFT;
}

void GahakuRobotCpp::moveToTopRightCorner(){
    moveBackToPoint(RWVec(568,320));
    moveAheadToPoint(RWVec(568,240));
    moveBackToPoint(RWVec(553,300));
    changeGunDirectionToPoint(RWVec(this->arenaDimensions().width - 300, this->arenaDimensions().height - 300));
    this->c_Location = LOCATION_TOP_RIGHT;
}

void GahakuRobotCpp::moveToNearestCorner(){
    
    if(this->position().x < this->arenaDimensions().width/2){
        this->moveToButtomLeftCorner();
    }else{
        this->moveToTopRightCorner();
    }
}


void GahakuRobotCpp::run()
{
    
    while (true) {
        
        if(this->c_State == STATE_START){
            
            moveToNearestCorner();
            
            this->c_State = STATE_NONE;
        }
        
        if(this->c_State == STATE_NONE){
            this->shoot();
        }
        
        if (this->c_State == STATE_SCANNING) {
            this->shoot();
        }
        
        if (this->lastKnownPositionTimestamp + 3 < currentTimestamp()) {
            this->c_State = STATE_NONE;
            if(this->position().x < this->arenaDimensions().width/2){
                changeGunDirectionToPoint(RWVec(300,300));
            }else{
                changeGunDirectionToPoint(RWVec(this->arenaDimensions().width - 300, this->arenaDimensions().height - 300));
            }
        }
        // move to the center before bombdrop starts
        if ( (this->START_BOMBS - 1) < currentTimestamp() && currentTimestamp() < (this->START_BOMBS + 1) ) {
            moveAheadToPoint(RWVec(arenaDimensions().width/2, arenaDimensions().height/2));
            changeDirectionToPointAhead(RWVec(arenaDimensions().width/2, arenaDimensions().height));
        }
    }
    
}

void GahakuRobotCpp::gotHit()
{
    if (this->lastKnownPositionTimestamp + 3 < currentTimestamp()){
        this->cancelActiveAction();
        moveToButtomLeftCorner();
    }
}

void GahakuRobotCpp::hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle)
{
}

void GahakuRobotCpp::bulletHitEnemy(RWVec enemyPosition)
{
    this->lastKnownPositionTimestamp = currentTimestamp();
}

void GahakuRobotCpp::scannedRobotAtPosition(RWVec position)
{
    this->cancelActiveAction();
    this->changeGunDirectionToPoint(position);
    if (this->c_State != STATE_SCANNING)
    {
        this->c_State = STATE_SCANNING;
    }
}