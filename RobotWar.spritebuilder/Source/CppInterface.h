//
//  CppInterface.h
//  RobotWar
//
//  Created by Daniel Haaser on 7/5/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#ifndef RobotWar_CppInterface_h
#define RobotWar_CppInterface_h

struct RWVec
{
    float x;
    float y;
    
    RWVec()
    : x(0.0f), y(0.0f)
    {}
    
    RWVec(float xx, float yy)
    : x(xx), y(yy)
    {}
};

struct RWSize
{
    float width;
    float height;
    
    RWSize()
    : width(0.0f), height(0.0f)
    {}
    
    RWSize(float pWidth, float pHeight)
    : width(pWidth), height(pHeight)
    {}
};

struct RWRect
{
    RWVec origin;
    RWSize size;
    
    RWRect()
    : origin(RWVec()), size(RWSize())
    {}
    
    RWRect(RWVec pOrigin, RWSize pSize)
    : origin(pOrigin), size(pSize)
    {}
};


// This is a hack to work around Objective C++
// As it appears to not support scoped enums
namespace RobotWallHitSide {

enum RobotWallHitSide
{
    NONE = 0,
    FRONT = 1,
    LEFT = 2,
    REAR = 3,
    RIGHT = 4
};
    
}

class RobotCallbackDelegate
{
public:
    virtual void run() {}
    virtual void scannedRobotAtPosition(RWVec position) {}
    virtual void gotHit() {}
    virtual void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) {}
    virtual void bulletHitEnemy(RWVec enemyPosition) {}
};

#endif
