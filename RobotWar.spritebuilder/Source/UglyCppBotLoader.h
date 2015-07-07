//
//  UglyCppBotLoader.h
//  RobotWar
//
//  Created by Daniel Haaser on 7/7/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

#import <Foundation/Foundation.h>

class RobotCpp;

@interface UglyCppBotLoader : NSObject

+ (UglyCppBotLoader*)sharedInstance;

- (RobotCpp*)constructBotWithName:(NSString*)name;

@end
