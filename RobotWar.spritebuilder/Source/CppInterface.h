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

/*!
 @methodgroup Event Handlers
 */

/*
 All of the following event handlers are called with a high priority. If an event handler calls 'cancelActiveAction'
 the currently running action of a robot will be stopped immediately. Any commands to the robot after this cancellation
 will also be performed immediately. Code running within the event handlers, such as movements, etc. will block further events from being called, until that action has been completed. Therefore you should avoid performing long actions directly in event handlers and instead set a state variable for your robot and run the behaviour from within the run method.
 */

class RobotCallbackDelegate
{
public:
    
    /*!
     @method
     
     This method is called ONCE when the match begins. Most robots will implement an endless loop in the run method
     and add different movement patterns to this loop.
     */
    virtual void run() {}
    
    /*!
     @method
     
     This method is called when another robot has been detected. Another robot is detected if it
     is within 150 points of your robot.
     
     @param robot
     Shallow copy of the robot that has been scanned. You can only access the class name
     and the owner of the robot.
     @param position
     World position of the robot that has been scanned
     */
    virtual void scannedRobotAtPosition(RWVec position) {}
    
    /*!
     @method
     
     This method is called when this robot got hit by a bullet.
     */
    virtual void gotHit() {}
    
    /*!
     @method
     
     This method is called when this robot hits one of the area walls.
     Robots should attempt moving away from a wall when receiving this event.
     
     @param hitDirection
     Describes which face of the robot has hit the wall (front, left, rear, right)
     
     @param hitAngle
     Provides the exact angle of the collision between robot and the wall.
     Angle is between -179 and +179
     An angle > 0 means that the robot hit the wall with the left side,
     An angle < 0 means that the robot hit the wall with the right side.
     */
    virtual void hitWallWithSideAndAngle(RobotWallHitSide::RobotWallHitSide side, float hitAngle) {}
    
    /*!
     @method
     
     Informs this robot that a bullet is has fired has hit an enemy.
     
     @param bullet
     Reference to the bullet that hit the enemy.
     
     */
    virtual void bulletHitEnemy(RWVec enemyPosition) {}
};

#endif
