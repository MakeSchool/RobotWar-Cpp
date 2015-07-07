//
//  MainScene.m
//  PROJECTNAME
//
//  Created by Viktor on 10/10/13.
//  Copyright (c) 2013 Apportable. All rights reserved.
//

#import "MainScene.h"
#import "Robot.h"
#import "Bullet.h"
#import "GameOverScene.h"
#import "Robot_Framework.h"
#import "GameConstants.h"
#import "Helpers.h"
#import "Configuration.h"

#import "RobotWrapper.h"

// weird preprocessor stuff for CPP labels
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)


@implementation MainScene {
  CGFloat timeSinceLastEvent;
  NSMutableArray *_bullets;
  NSMutableArray *_robots;
  CCNodeColor *_gameNode;
    
  CCLabelTTF* _robot1Label;
  CCLabelTTF* _robot2Label;
  CCLabelTTF* _bombCountdownLabel;
  
  CGFloat timeSinceBomb;
}

#pragma mark - Lifecycle / Scene Transitions

- (void)dealloc {
  NSLog(@"Game Over!");
}

- (void)didLoadFromCCB {
  [self updateTimeSinceBomb:0.0f];
  
  _bullets = [NSMutableArray array];
  
  _robots = [NSMutableArray array];
}

- (Class)swiftClassFromString:(NSString *)className {
    NSString *appName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleName"];
    NSString *classStringName = [NSString stringWithFormat:@"_TtC%lu%@%lu%@", (unsigned long)appName.length, appName, (unsigned long)className.length, className];
    return NSClassFromString(classStringName);
}

- (void)initWithRobotClassOne:(NSString *)botClass1 andRobotClassTwo:(NSString *)botClass2  {
  // intantiate two AIs
    
    Robot* robot1;
    Robot* robot2;
    
  if ([botClass1 isEqualToString:@"RobotWrapper"])
  {
      robot1 = (Robot*) [[NSClassFromString(botClass1) alloc] init];
      RobotWrapper* robotOneWrapper = (RobotWrapper*) robot1;
      [robotOneWrapper setCppRobotClassForRobot:YES];
  }
  else
  {
      robot1 = (Robot*) [[[self swiftClassFromString:botClass1] alloc] init];
  }
    
  if ([botClass2 isEqualToString:@"RobotWrapper"])
  {
      robot2 = (Robot*) [[NSClassFromString(botClass2) alloc] init];
      RobotWrapper* robotTwoWrapper = (RobotWrapper*) robot2;
      [robotTwoWrapper setCppRobotClassForRobot:NO];
  }
  else
  {
      robot2 = (Robot*) [[[self swiftClassFromString:botClass2] alloc] init];
  }
    
  _robots = [NSMutableArray arrayWithArray:@[robot1, robot2]];

  //spawn two robots
  robot1.robotNode = [CCBReader load:@"Robot" owner:robot1];
  CCColor* robotOneColor = [CCColor colorWithCcColor3b:ccc3(251, 72, 154)];
  [robot1 _setRobotColor:robotOneColor];
  [robot1 _setFieldOfViewColor:[CCColor colorWithCcColor3b:ccc3(251, 87, 172)]];
  [_robot1Label setColor:robotOneColor];
  
  robot1.robotNode.position = ccp(50, arc4random_uniform(140) + 80);
  [_gameNode addChild:robot1.robotNode];
  robot1.gameBoard = self;
  [robot1 _run];
  robot1.robotClass = botClass1;

  robot2.robotNode = [CCBReader load:@"Robot" owner:robot2];
  CCColor* robotTwoColor = [CCColor colorWithCcColor3b:ccc3(84, 184, 255)];
  [robot2 _setRobotColor:robotTwoColor];
  [_robot2Label setColor:robotTwoColor];
  CGSize screenSize = [[CCDirector sharedDirector] viewSize];
  robot2.robotNode.position = ccp(screenSize.width - 50, arc4random_uniform(140) + 80);
  [_gameNode addChild:robot2.robotNode];
  robot2.gameBoard = self;
  [robot2 _run];
  robot2.robotNode.rotation = 180;
  robot2.robotClass = botClass2;
    
  [self updateScoreLabels];
}

- (void)transitionToGameOverScreen:(NSDictionary *)results {
    
  CCScene *gameOverSceneWrapper = [CCBReader loadAsScene:@"GameOverScene"];
  GameOverScene *gameOverScene = gameOverSceneWrapper.children[0];
    
  Robot* winner = [results objectForKey:@"Winner"];
  Robot* loser = [results objectForKey:@"Loser"];
    
  gameOverScene.winnerClass = winner.robotClass;
  gameOverScene.winnerName = winner.creator;
  gameOverScene.loserClass = loser.robotClass;
  gameOverScene.loserName = loser.creator;
    
  [gameOverScene displayWinMessage];
  CCTransition *transition = [CCTransition transitionCrossFadeWithDuration:0.3f];
  [[CCDirector sharedDirector] replaceScene:gameOverSceneWrapper withTransition:transition];
}

#pragma mark - Update Loop

- (void)update:(CCTime)delta {
  timeSinceLastEvent += delta * GAME_SPEED;
  self.currentTimestamp += delta * GAME_SPEED;
  [self updateTimeSinceBomb:timeSinceBomb + delta * GAME_SPEED];
  
  for (Robot *robot in _robots) {
    if (!CGRectContainsRect(self.boundingBox, robot.robotNode.boundingBox)) {
      
      /**
       Don't permit robots to leave the arena
       */
      while (CGRectGetMaxX([robot.robotNode boundingBox]) > self.contentSizeInPoints.width) {
        robot.robotNode.position = ccp(robot.robotNode.position.x-1, robot.robotNode.position.y);
        [self calculateCollisionAngleWithWallNormalVector:ccp(-1, 0) notifyRobot:robot];
      }
      
      while (CGRectGetMaxY([robot.robotNode boundingBox]) > self.contentSizeInPoints.height) {
        robot.robotNode.position = ccp(robot.robotNode.position.x, robot.robotNode.position.y-1);
        [self calculateCollisionAngleWithWallNormalVector:ccp(0, -1) notifyRobot:robot];
      }
      
      while (CGRectGetMinX([robot.robotNode boundingBox]) < 0) {
        robot.robotNode.position = ccp(robot.robotNode.position.x+1, robot.robotNode.position.y);
        [self calculateCollisionAngleWithWallNormalVector:ccp(+1, 0) notifyRobot:robot];
      }
      
      while (CGRectGetMinY([robot.robotNode boundingBox]) < 0) {
        robot.robotNode.position = ccp(robot.robotNode.position.x, robot.robotNode.position.y+1);
        [self calculateCollisionAngleWithWallNormalVector:ccp(0, +1) notifyRobot:robot];
      }
      
    }
  }
  
  NSMutableArray *cleanupBullets = nil;
  BOOL labelsNeedUpdate = NO;
  
  for (Bullet *bullet in _bullets) {
    
    if (!CGRectContainsRect(self.boundingBox, bullet.boundingBox)) {
      if (!cleanupBullets) {
        cleanupBullets = [NSMutableArray array];
      }
      
      [cleanupBullets addObject:bullet];
      continue;
    }
    
    for (Robot *robot in _robots) {
      if (bullet.bulletOwner == robot) {
        continue;
      } else if (CGRectIntersectsRect(bullet.boundingBox, robot.robotNode.boundingBox)) {
        [robot _gotHit];
        labelsNeedUpdate = YES;
        [bullet.bulletOwner _bulletHitEnemy:bullet];
          
        CCParticleSystem *bulletExplosion = (CCParticleSystem *) [CCBReader load:@"BulletExplosion"];
        bulletExplosion.position = bullet.position;
        [_gameNode addChild:bulletExplosion];
        
        if (!cleanupBullets) {
          cleanupBullets = [NSMutableArray array];
        }
        
        [cleanupBullets addObject:bullet];
      }
    }
  }
  
  if (self.currentTimestamp > START_BOMBS && timeSinceBomb > BETWEEN_BOMBS) {
      [self dropBomb];
      [self updateTimeSinceBomb:0.0f];
  }
    
  if (labelsNeedUpdate)
      [self updateScoreLabels];
  
  for (Bullet *bullet in cleanupBullets) {
    [self cleanupBullet:bullet];
  }
  
  // Robot Detection
  for (Robot *robot in _robots) {
    for (Robot *otherRobot in _robots) {
      if (otherRobot == robot) {
        continue;
      } else if (ccpDistance(robot.robotNode.position, otherRobot.robotNode.position)  < SCAN_DISTANCE) {
        if (timeSinceLastEvent > 0.5f/GAME_SPEED) {
          if (fabsf( (float) [robot angleBetweenGunHeadingDirectionAndWorldPosition:otherRobot.position]) < SCAN_FIELD_OF_VIEW/2) {
            [robot _scannedRobot:[otherRobot copy] atPosition:otherRobot.robotNode.positionInPoints];
            [robot _updateFOVScaned:YES];
          } else {
            [robot _updateFOVScaned:NO];
          }
          if (fabsf( (float) [otherRobot angleBetweenGunHeadingDirectionAndWorldPosition:robot.position]) < SCAN_FIELD_OF_VIEW/2) {
            [otherRobot _scannedRobot:[robot copy] atPosition:robot.robotNode.positionInPoints];
            [otherRobot _updateFOVScaned:YES];
          } else {
            [otherRobot _updateFOVScaned:NO];
          }
          timeSinceLastEvent = 0.f;
        }
      } else {
        [robot _updateFOVScaned:NO];
        [otherRobot _updateFOVScaned:NO];
      }
    }
  }
}

- (void)calculateCollisionAngleWithWallNormalVector:(CGPoint)wallNormalVector notifyRobot:(Robot*)robot {
  if (timeSinceLastEvent > 0.5f/GAME_SPEED) {
    CGFloat collisionAngle;
    RobotWallHitDirection direction;
    calc_collisionAngle_WallHitDirection(wallNormalVector, robot, &collisionAngle, &direction);
    
    [robot _hitWall:direction hitAngle:collisionAngle];
    timeSinceLastEvent = 0.f;
  }
}

- (void)dropBomb {
  CGSize dim = [self dimensions];
  
  int corner = arc4random_uniform(4);
  int angle = arc4random_uniform(90);
  int distance = arc4random_uniform([self dimensions].width/2);
  CGPoint cornerPosition = ccp(0, 0);
  
  switch (corner) {
    case 0: //bottom-left
      break;
    case 1: //top-left
      cornerPosition = ccpAdd(cornerPosition, ccp(0, dim.height));
      angle += 270;
      break;
    case 2: //top-right
      cornerPosition = ccpAdd(cornerPosition, ccp(dim.width, dim.height));
      angle += 180;
      break;
    case 3: //bottom-right
      cornerPosition = ccpAdd(cornerPosition, ccp(dim.width, 0));
      angle += 90;
      break;
  }
  
  CGPoint diffFromCorner = ccpMult(ccpForAngle(CC_DEGREES_TO_RADIANS(angle)), distance);
  CGPoint bombPos = ccpAdd(cornerPosition, diffFromCorner);
  
  CCParticleSystem *explosion = (CCParticleSystem *) [CCBReader load:@"BombExplosion"];
  [_gameNode addChild:explosion];
  explosion.position = bombPos;
  explosion.autoRemoveOnFinish = YES;
  
  for (Robot *robot in _robots) {
    if (ccpDistance(robot.position, bombPos) < 65) {
      [robot _bombHit];
    }
  }
  
  [self updateScoreLabels];
}

#pragma mark - GameBoard Protocol

- (CGSize)dimensions {
  return [[CCDirector sharedDirector] viewSize];
}

- (void)fireBulletFromPosition:(CGPoint)position inDirection:(CGPoint)direction bulletOwner:(id)owner {
  Bullet *bullet = [Bullet nodeWithColor:[CCColor colorWithCcColor3b:ccc3(245, 245, 245)]];
  bullet.contentSize = CGSizeMake(5.f, 5.f);
  CCActionMoveBy *moveBy = [CCActionMoveBy actionWithDuration:0.1f/GAME_SPEED position:ccpMult(direction, 20)];
  CCActionRepeatForever *repeat = [CCActionRepeatForever actionWithAction:moveBy];
  
  bullet.bulletOwner = owner;
  [_bullets addObject:bullet];
  [_gameNode addChild:bullet];
  bullet.position = position;
  [bullet runAction:repeat];
}

- (void)robotDied:(Robot *)robot {
  dispatch_async(dispatch_get_main_queue(), ^{
    CCParticleSystem *explosion = (CCParticleSystem *) [CCBReader load:@"RobotExplosion"];
    [_gameNode addChild:explosion];
    explosion.position = robot.robotNode.positionInPoints;
    
    [robot.robotNode removeFromParent];
    [_robots removeObject:robot];
    
    if (_robots.count == 1) {
        NSDictionary* results = @{@"Winner": _robots[0], @"Loser": robot};
      [self performSelector:@selector(transitionToGameOverScreen:) withObject:results afterDelay:3.0f];
    }
  });
}

- (RobotWallHitDirection)currentWallHitDirectionForRobot:(Robot*)robot {
  static NSInteger toleranceMargin = 5;
  
  CGPoint wallNormalVector = CGPointZero;
  
  if (CGRectGetMaxX([robot.robotNode boundingBox]) >= self.contentSizeInPoints.width - toleranceMargin) {
    wallNormalVector = ccp(-1, 0);
  } else if (CGRectGetMaxY([robot.robotNode boundingBox]) >= self.contentSizeInPoints.height -toleranceMargin) {
    wallNormalVector = ccp(0, -1);
  } else if (CGRectGetMinX([robot.robotNode boundingBox]) <= toleranceMargin) {
    wallNormalVector = ccp(+1, 0);
  } else if (CGRectGetMinY([robot.robotNode boundingBox]) <= toleranceMargin) {
    wallNormalVector = ccp(0, +1);
  }
  
  if (CGPointEqualToPoint(wallNormalVector, CGPointZero)) {
    return RobotWallHitDirectionNone;
  } else {
    CGFloat collisionAngle;
    RobotWallHitDirection wallHitDirection;
    calc_collisionAngle_WallHitDirection(wallNormalVector, robot, &collisionAngle, &wallHitDirection);
    return wallHitDirection;
  }
}

#pragma mark - Util Methods/Functions

- (void)updateScoreLabels {

  if (_robots.count > 1)
  {
      Robot* robot1 = (Robot*) _robots[0];
      Robot* robot2 = (Robot*) _robots[1];
        
      if (robot1)
      {
          _robot1Label.string = [NSString stringWithFormat:@"%@ %ld", robot1.robotClass, (long)[robot1 hitPoints]];
      }
      else
      {
          _robot1Label.string = @"DEAD";
      }
        
      if (robot2)
      {
          _robot2Label.string = [NSString stringWithFormat:@"%@ %ld", robot2.robotClass, (long)[robot2 hitPoints]];
      }
      else
      {
          _robot2Label.string = @"DEAD";
      }
   }
}

- (void)updateTimeSinceBomb:(CGFloat)pTimeSinceBomb {
    timeSinceBomb = pTimeSinceBomb;
    
    float timeUntilBomb = 0.0f;
    
    if (self.currentTimestamp > START_BOMBS)
    {
        timeUntilBomb = BETWEEN_BOMBS - timeSinceBomb;
    }
    else
    {
        timeUntilBomb = START_BOMBS - timeSinceBomb;
    }
    
    if (timeUntilBomb <= 1.0f)
    {
        _bombCountdownLabel.string = @"Warning";
        _bombCountdownLabel.color = [CCColor colorWithCcColor3b:ccc3(255, 0, 255)];
    }
    else
    {
        _bombCountdownLabel.string = [NSString stringWithFormat:@"%.0f", timeUntilBomb];
        _bombCountdownLabel.color = [CCColor colorWithCcColor3b:ccc3(255, 255, 255)];
    }
}

- (void)cleanupBullet:(CCNode *)bullet {
  [bullet removeFromParent];
  [_bullets removeObject:bullet];
}

void calc_collisionAngle_WallHitDirection(CGPoint wallNormalVector, Robot *robot, CGFloat *collisionAngle_p, RobotWallHitDirection *direction_p) {
  // Calculate Collision Angle
  *collisionAngle_p = ccpAngleSigned([robot headingDirection], wallNormalVector);
  *collisionAngle_p = roundf(radToDeg(*collisionAngle_p));
  *direction_p = radAngleToRobotWallHitDirection(*collisionAngle_p);
}

@end
