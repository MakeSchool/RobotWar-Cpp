//
//  TourneyManagementScene.m
//  RobotWar
//
//  Created by Daniel Haaser on 7/5/14.
//  Copyright (c) 2014 MakeGamesWithUs. All rights reserved.
//

#import "TournamentScene.h"
#import "Robot.h"
#import "MainScene.h"
#import "TournamentWonScene.h"
#import "TournamentConfiguration.h"

static NSMutableDictionary* schedule;

@implementation TournamentScene
{
    CCLabelTTF* roundLabel;
    CCLabelTTF* robotOneLabel;
    CCLabelTTF* robotTwoLabel;
    CCLabelTTF* robotOneStats;
    CCLabelTTF* robotTwoStats;
    CCLabelTTF* countdownLabel;
    
    int countdown;
    BOOL tournamentOver;
}

#pragma mark -
#pragma mark Init and Scheduling

- (id)init
{
    if (self = [super init])
    {
        static dispatch_once_t once;
        dispatch_once(&once, ^ {
            
            // Check if there's a tournament saved to disk to resume
            schedule = [[NSUserDefaults standardUserDefaults] objectForKey:@"TournamentState"];
            
            if (!schedule)
            {
                // No tournament on disk, so make a new one
                NSArray* allRobots = ClassGetSubclasses([Robot class]);
                
                // Swift classes seem to prepend with BundleName (format seems to be BundleName.ClassName now)
                // So remove the BundleName. part so that we can load the Swift classes later
                NSArray* robotClassStrings = [self convertClassArrayToClassNameArrayRemovingBundleID:allRobots];
                
                schedule = [NSMutableDictionary dictionaryWithDictionary:[self createTournamentScheduleWithBots:robotClassStrings]];
                tournamentOver = NO;
            }
        });
    }
    
    return self;
}

NSArray *ClassGetSubclasses(Class parentClass)
{
    int numClasses = objc_getClassList(NULL, 0);
    Class *classes = NULL;
    
    classes = (__unsafe_unretained Class *) malloc(sizeof(Class) * numClasses);
    numClasses = objc_getClassList(classes, numClasses);
    
    NSMutableArray *result = [NSMutableArray array];
    
    for (NSInteger i = 0; i < numClasses; i++)
    {
        Class superClass = classes[i];
        do
        {
            superClass = class_getSuperclass(superClass);
        } while(superClass && superClass != parentClass);
        
        if (superClass == nil)
        {
            continue;
        }
        
        [result addObject:classes[i]];
    }
    
    free(classes);
    
    return result;
}

- (NSArray*) convertClassArrayToClassNameArrayRemovingBundleID:(NSArray*) classArray
{
    NSString* appName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleName"];
    
    NSString* bundleNameWithDot = [NSString stringWithFormat:@"%@.", appName];
    
    NSMutableArray* allRobotsFixed = [NSMutableArray arrayWithCapacity:classArray.count];
    
    for (Class class in classArray)
    {
        const char* className = class_getName(class);
        NSString* classString = [NSString stringWithUTF8String:className];
        
        NSRange replaceRange = [classString rangeOfString:bundleNameWithDot];
        
        if (replaceRange.location != NSNotFound)
        {
            NSString* fixedRobotClassString = [classString stringByReplacingCharactersInRange:replaceRange withString:@""];
            [allRobotsFixed addObject:fixedRobotClassString];
        }
        else
        {
            [allRobotsFixed addObject:classString];
        }
    }
    
    return allRobotsFixed;
}


- (NSDictionary*)createTournamentScheduleWithBots:(NSArray*)robotNames
{
    NSMutableArray* matches = [NSMutableArray arrayWithCapacity:(robotNames.count / 2) * (robotNames.count - 1)];
    NSMutableDictionary* records = [NSMutableDictionary dictionaryWithCapacity:robotNames.count];
    
    int matchNumber = 0;
    
    for (int i = 0; i < robotNames.count; ++i)
    {
        NSString* robotOneClassString = robotNames[i];
        
        for (int j = i + 1; j < robotNames.count; ++j)
        {
            NSString* robotTwoClassString = robotNames[j];
            
            NSDictionary* match = @{@"Match": @(matchNumber),
                                    @"RobotOne": robotOneClassString,
                                    @"RobotTwo": robotTwoClassString,
                                    @"Winner": @""};
            
            [matches addObject:match];
            
            ++matchNumber;
        }
        
        // Add record (win, loss draw) entry for each bot
        NSDictionary* record = @{@"Wins": @(0), @"Losses": @(0), @"Draws": @(0)};
        [records setObject:record forKey:robotOneClassString];
    }
    
    return @{@"Matches": matches, @"CurrentMatch": @(-1), @"Records": records};
}

#pragma mark -
#pragma mark Tournament Stuff

- (void)cleanup
{
    [self unschedule:@selector(updateCountdown)];
}

- (void)onEnterTransitionDidFinish
{
    [super onEnterTransitionDidFinish];
    
    [self incrementMatchNumber];
    
    [self updateLabels];
    
    countdown = COUNTDOWN;
    countdownLabel.string = [NSString stringWithFormat:@"%d", countdown];
    [self schedule:@selector(updateCountdown) interval:1.0f];
}

- (void)incrementMatchNumber
{
    int nextMatchNumber = [[schedule objectForKey:@"CurrentMatch"] intValue] + 1;
    NSArray* matches = [schedule objectForKey:@"Matches"];
    
    if (nextMatchNumber >= matches.count)
    {
        tournamentOver = YES;
        [self unschedule:@selector(updateCountdown)];
        [self loadTournamentWonScene];
    }
    else
    {
        [schedule setObject:@(nextMatchNumber) forKey:@"CurrentMatch"];
    }
    
    [self saveTournamentStateToDisk];
}

- (void)updateCountdown
{
    --countdown;
    countdownLabel.string = [NSString stringWithFormat:@"%d", countdown];
    
    if (countdown <= 0 && !tournamentOver)
    {
        [self unschedule:@selector(updateCountdown)];
        [self loadNextMatch];
    }
}

- (void)updateLabels
{
    int matchNumber = [[schedule objectForKey:@"CurrentMatch"] intValue];
    NSArray* matches = [schedule objectForKey:@"Matches"];
    NSDictionary* match = matches[matchNumber];
    
    NSString* robotOneClass = [match objectForKey:@"RobotOne"];
    NSString* robotTwoClass = [match objectForKey:@"RobotTwo"];
    
    robotOneLabel.string = robotOneClass;
    robotTwoLabel.string = robotTwoClass;
    
    NSDictionary* records = [schedule objectForKey:@"Records"];
    NSDictionary* robotOneRecord = [records objectForKey:robotOneClass];
    NSDictionary* robotTwoRecord = [records objectForKey:robotTwoClass];
    
    robotOneStats.string = [NSString stringWithFormat:@"%d - %d", [[robotOneRecord objectForKey:@"Wins"] intValue], [[robotOneRecord objectForKey:@"Losses"] intValue]];
    robotTwoStats.string = [NSString stringWithFormat:@"%d - %d", [[robotTwoRecord objectForKey:@"Wins"] intValue], [[robotTwoRecord objectForKey:@"Losses"] intValue]];
    
    roundLabel.string = [NSString stringWithFormat:@"Round %d", matchNumber];
}

- (void)loadNextMatch
{
    int matchNumber = [[schedule objectForKey:@"CurrentMatch"] intValue];
    NSArray* matches = [schedule objectForKey:@"Matches"];
    NSDictionary* match = matches[matchNumber];
    
    MainScene* nextMatch = (MainScene*) [CCBReader load:@"MainScene"];
    
    // Randomize Positions
    int randPosition = arc4random() % 2;
    
    if (randPosition == 0)
    {
        [nextMatch initWithRobotClassOne: [match objectForKey:@"RobotOne"] andRobotClassTwo:[match objectForKey:@"RobotTwo"]];
    }
    else
    {
        [nextMatch initWithRobotClassOne: [match objectForKey:@"RobotTwo"] andRobotClassTwo:[match objectForKey:@"RobotOne"]];
    }
    
    CCScene* nextMatchScene = [CCScene node];
    [nextMatchScene addChild:nextMatch];
    [[CCDirector sharedDirector] replaceScene:nextMatchScene];
}

- (void)updateWithResults:(NSDictionary*)results
{
    // Update results dictionary
    NSString* winningRobotClass = [results objectForKey:@"Winner"];
    NSString* losingRobotClass = [results objectForKey:@"Loser"];
    
    NSMutableDictionary* records = [NSMutableDictionary dictionaryWithDictionary:[schedule objectForKey:@"Records"]];
    NSDictionary* winningRobotRecord = [records objectForKey:winningRobotClass];
    NSDictionary* losingRobotRecord = [records objectForKey:losingRobotClass];
    
    NSMutableDictionary* winningRobotRecordCopy = [NSMutableDictionary dictionaryWithDictionary:winningRobotRecord];
    int wins = [[winningRobotRecordCopy objectForKey:@"Wins"] intValue];
    wins++;
    [winningRobotRecordCopy setObject:@(wins) forKey:@"Wins"];
    
    NSMutableDictionary* losingRobotRecordCopy = [NSMutableDictionary dictionaryWithDictionary:losingRobotRecord];
    int losses = [[losingRobotRecordCopy objectForKey:@"Losses"] intValue];
    losses++;
    [losingRobotRecordCopy setObject:@(losses) forKey:@"Losses"];
    
    [records setObject:[NSDictionary dictionaryWithDictionary:winningRobotRecordCopy] forKey:winningRobotClass];
    [records setObject:[NSDictionary dictionaryWithDictionary:losingRobotRecordCopy] forKey:losingRobotClass];
    
    [schedule setObject:[NSDictionary dictionaryWithDictionary:records] forKey:@"Records"];
    
    // Update match winner in matches array
    NSMutableArray* matches = [NSMutableArray arrayWithArray:[schedule objectForKey:@"Matches"]];
    int matchNumber = [[schedule objectForKey:@"CurrentMatch"] intValue];
    NSMutableDictionary* match = [NSMutableDictionary dictionaryWithDictionary:matches[matchNumber]];
    [match setObject:winningRobotClass forKey:@"Winner"];
    matches[matchNumber] = match;
    
    [schedule setObject:[NSArray arrayWithArray:matches] forKey:@"Matches"];
}

- (void)saveTournamentStateToDisk
{
    [[NSUserDefaults standardUserDefaults] setObject:schedule forKey:@"TournamentState"];
}

#pragma mark - 
#pragma mark Tournament Finish Stuff


- (void)loadTournamentWonScene
{
    TournamentWonScene* tournamentWonScene = (TournamentWonScene*) [CCBReader load:@"TournamentWonScene"];
    
    tournamentWonScene.winningRobot = [self getWinningRobot];
    
    CCScene* newScene = [CCScene node];
    [newScene addChild:tournamentWonScene];
    
    [[CCDirector sharedDirector] presentScene:newScene];
}

- (NSString*)getWinningRobot
{
    NSDictionary* records = [schedule objectForKey:@"Records"];
    
    __block int mostWins = 0;
    __block NSString* winningestBot;
    
    [records enumerateKeysAndObjectsUsingBlock:^(NSString* robotClass, NSDictionary* record, BOOL *stop)
     {
         int thisBotWins = [[record objectForKey:@"Wins"] intValue];
         
         if (thisBotWins > mostWins)
         {
             mostWins = thisBotWins;
             winningestBot = [robotClass copy];
         }
         
         // Print ranks to console
         NSLog(@"(%d W, %d L) - %@", [[record objectForKey:@"Wins"] intValue], [[record objectForKey:@"Losses"] intValue], robotClass);
         
     }];
    
    
    // TODO: Check for tie conditions, look at which bot beat the other to break tie
    
    return winningestBot;
}

@end
