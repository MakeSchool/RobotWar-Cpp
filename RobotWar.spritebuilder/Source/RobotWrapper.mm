//
//  RobotWrapper.m
//  RobotWar
//
//  Created by Daniel Haaser on 7/5/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#import "RobotWrapper.h"
#import "Robot.h"
#include "CppInterface.h"
#include "CppConfiguration.h"

@implementation RobotWrapper
{
    RobotCpp* delegate;
}

#pragma mark -
#pragma mark Lifecycle

- (instancetype)init
{
    self = [super init];
    
    if (self)
    {
        delegate = new SimpleRobotCpp();
        delegate->setObjCRobot(self);
    }
    
    return self;
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
        delegate->bulletHitEnemy();
    }
}

@end
