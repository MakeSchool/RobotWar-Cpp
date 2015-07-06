//
//  RobotCpp.h
//  RobotWar
//
//  Created by Daniel Haaser on 7/5/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__RobotCpp__
#define __RobotWar__RobotCpp__

#include "CppInterface.h"

#ifdef __OBJC__
@class RobotWrapper;
#else
typedef struct objc_object RobotWrapper;
#endif

class RobotCpp : public RobotCallbackDelegate
{
public:
    void setObjCRobot(RobotWrapper* pRobot);
    
protected:
    void turnGunLeft(int degrees);
    
    void turnGunRight(int degrees);
    
    void turnRobotLeft(int degrees);
    
    void turnRobotRight(int degrees);
    
    void moveAhead(int distance);
    
    void moveBack(int distance);
    
    void shoot();
    
    void cancelActiveAction();
    
    int hitPoints();
    
    RWRect robotBoundingBox();
    
    RWVec position();
    
    RWVec headingDirection();
    
    float angleBetweenHeadingDirectionAndWorldPosition(RWVec position);
    
    RWVec gunHeadingDirection();
    
    float angleBetweenGunHeadingDirectionAndWorldPosition(RWVec position);
    
    float currentTimestamp();
    
    RWSize arenaDimensions();
    
private:
    RobotWrapper* robot;
};

#endif /* defined(__RobotWar__RobotCpp__) */
