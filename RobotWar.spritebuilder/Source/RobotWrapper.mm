//
//  RobotWrapper.m
//  RobotWar
//
//  Created by Daniel Haaser on 7/5/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#import "RobotWrapper.h"
#import "Bullet.h"
#include "CppInterface.h"
#include "CppIncludes.h"
#include "Configuration.h"

@implementation RobotWrapper
{
    RobotCpp* delegate;
}

#pragma mark -
#pragma mark Lifecycle

- (void)setCppRobotClassForRobot:(BOOL)robotOne
{
    if (robotOne)
    {
        delegate = new ROBOT_ONE_CPP_CLASS();
    }
    else
    {
        delegate = new ROBOT_TWO_CPP_CLASS();
    }
    
    delegate->setObjCRobot(self);
}

- (void)dealloc
{
    delete delegate;
}

#pragma mark -
#pragma mark Callback Methods

- (void)run
{
    if (delegate)
    {
        delegate->run();
    }
}


- (void)scannedRobot:(Robot *)robot atPosition:(CGPoint)position
{
    if (delegate)
    {
        RWVec vPosition(position.x, position.y);
        delegate->scannedRobotAtPosition(vPosition);
    }
}

- (void)gotHit
{
    if (delegate)
    {
        delegate->gotHit();
    }
}

- (void)hitWall:(RobotWallHitDirection)hitDirection hitAngle:(CGFloat)hitAngle
{
    if (delegate)
    {
        RobotWallHitSide::RobotWallHitSide side;
        
        switch (hitDirection)
        {
            case RobotWallHitDirectionNone: side = RobotWallHitSide::NONE; break;
            case RobotWallHitDirectionFront: side = RobotWallHitSide::FRONT; break;
            case RobotWallHitDirectionLeft: side = RobotWallHitSide::LEFT; break;
            case RobotWallHitDirectionRear: side = RobotWallHitSide::REAR; break;
            case RobotWallHitDirectionRight: side = RobotWallHitSide::RIGHT; break;
        }
        
        float hitAngleF = hitAngle;
        
        delegate->hitWallWithSideAndAngle(side, hitAngleF);
    }
}

- (void)bulletHitEnemy:(Bullet *)bullet
{
    if (delegate)
    {
        CGPoint enemyPosition = bullet.position;
        
        delegate->bulletHitEnemy(RWVec(enemyPosition.x, enemyPosition.y));
    }
}

@end
