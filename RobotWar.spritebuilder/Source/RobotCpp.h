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
    RobotCpp();
    void setObjCRobot(RobotWrapper* pRobot);
    
protected:
    
    /*!
     @methodgroup Actions
     */
    
    /*!
     @method
     
     Turns the gun of the robot to the left.
     
     @param degrees degrees of rotation
     */
    void turnGunLeft(int degrees);
    
    /*!
     @method
     
     Turns the gun of the robot to the right.
     
     @param degrees degrees of rotation
     */
    void turnGunRight(int degrees);
    
    /*!
     @method
     
     Turns the robot to the left.
     
     @param degrees degrees of rotation
     */
    void turnRobotLeft(int degrees);
    
    /*!
     @method
     
     Turns the robot to the right.
     
     @param degrees degrees of rotation
     */
    void turnRobotRight(int degrees);
    
    /*!
     @method
     
     Moves the robot ahead in the direction it is currently heading.
     
     @param distance distance in points
     */
    void moveAhead(int distance);
    
    /*!
     @method
     
     Moves the robot backwards in the direction opposite to the one it is currently heading.
     
     @param distance distance in points
     */
    void moveBack(int distance);
    
    /*!
     @method
     
     Shoots a bullet in the direction the gun is currently heading.
     Note: unlike all other actions this action cannot be cancelled.
     After shooting the robot will freeze for 0.5 game seconds.
     */
    void shoot();
    
    /*!
     @method
     
     Calling this method will cancel the currently perforemed action (movement, rotation, etc.).
     This is useful within event handlers. You can cancel the currently active action to take immediate control of your Robot in
     order to respond to an event.
     
     Note that the 'shoot' action will not be cancelled by calling this method.
     */
    void cancelActiveAction();
    
    /*!
     @methodgroup Information Retrieval
     */

    /*!
     @method
     
     @return Returns the robot's hit points
     */
    int hitPoints();
    
    /*!
     @method
     
     @return Returns the bounding box of this robot in world coordinates.
     */
    RWRect robotBoundingBox();
    
    /*!
     @method
     
     @return Returns the position of this robot in world coordinates.
     */
    RWVec position();
    
    /*!
     @method
     
     @return Returns the heading direction of the robot in form of a vector.
     */
    RWVec headingDirection();
    
    /*!
     @method
     
     @return Returns the angle between the heading direction of the robot and an arbitrary point.
     Use this information to turn the robot in the direction / opposite direction of a detected object.
     */
    float angleBetweenHeadingDirectionAndWorldPosition(RWVec position);
    
    /*!
     @method
     
     @return Returns the gun heading direction in form of a vector.
     */
    RWVec gunHeadingDirection();
    
    /*!
     @method
     
     @return Returns the angle between the heading direction of the gun of the robot and an arbitrary point.
     Use this information to turn the gun of the robot in the direction / opposite direction of a detected object.
     */
    float angleBetweenGunHeadingDirectionAndWorldPosition(RWVec position);
    
    /*!
     @method
     
     @return Returns the current timestamp. You can use the timestamp to mark information as stale after a certain
     timespan has passed.
     */
    float currentTimestamp();
    
    /*!
     @method
     
     @return Returns the dimensions of the Arena. The bottom left corner of the area has a world position of (0,0).
     Use this information to determine your position within the arena.
     */
    RWSize arenaDimensions();
    
private:
    RobotWrapper* robot;
};

#endif /* defined(__RobotWar__RobotCpp__) */
