//
//  UglyCppBotLoader.m
//  RobotWar
//
//  Created by Daniel Haaser on 7/7/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#import "UglyCppBotLoader.h"
#import "RobotCpp.h"
#include "CppIncludes.h"

@implementation UglyCppBotLoader

static UglyCppBotLoader *_sharedInstance = nil;

+ (UglyCppBotLoader *)sharedInstance
{
    if (!_sharedInstance)
    {
        _sharedInstance = [[self alloc] init];
    }
    
    return _sharedInstance;
}

- (instancetype)init
{
    if (self = [super init])
    {
        
    }
    
    return self;
}

#pragma mark -
#pragma mark Public Methods


- (RobotCpp*)constructBotWithName:(NSString *)name
{
    RobotCpp* returnRobot = 0;
    
    if ([name isEqualToString:@"SimpleRobotCpp"])
    {
        returnRobot = new SimpleRobotCpp();
    }
    else if ([name isEqualToString:@"AdvancedRobotCpp"])
    {
        returnRobot = new AdvancedRobotCpp();
    }
    else if ([name isEqualToString:@"TurretRobotCpp"])
    {
        returnRobot = new TurretRobotCpp();
    }
    else if ([name isEqualToString:@"NewAdvancedRobotCpp"])
    {
        returnRobot = new NewAdvancedRobotCpp();
    }
    else if ([name isEqualToString:@"LiveRobotCpp"])
    {
        returnRobot = new LiveRobotCpp();
    }
    else if ([name isEqualToString:@"AmaAmaCpp"])
    {
        returnRobot = new AmaAmaCpp();
    }
    else if ([name isEqualToString:@"AsadaRobotCpp"])
    {
        returnRobot = new AsadaRobotCpp();
    }
    else if ([name isEqualToString:@"Tokutya"])
    {
        returnRobot = new Tokutya();
    }
    else if ([name isEqualToString:@"EtsukoMiyazato"])
    {
        returnRobot = new EtsukoMiyazato();
    }
    else if ([name isEqualToString:@"WatanabeRobotCpp"])
    {
        returnRobot = new WatanabeRobotCpp();
    }
    else if ([name isEqualToString:@"EijiRobotCpp"])
    {
        returnRobot = new EijiRobotCpp();
    }
    else if ([name isEqualToString:@"NoriyukiRobot"])
    {
        returnRobot = new NoriyukiRobot();
    }
    
    return returnRobot;
}

@end
