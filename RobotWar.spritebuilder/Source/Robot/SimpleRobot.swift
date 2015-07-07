//
//  SimpleRobotSwift.swift
//  RobotWar
//
//  Created by Dion Larson on 7/2/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

import Foundation

class SimpleRobot: Robot {
  
  override func run() {
    while true {
      moveAhead(80)
      turnRobotRight(20)
      moveAhead(100)
      shoot()
      turnRobotLeft(10)
    }
  }
  
  override func scannedRobot(robot: Robot!, atPosition position: CGPoint) {
    // unimplemented
  }
  
  override func gotHit() {
    shoot()
    turnRobotLeft(45)
    moveAhead(100)
  }
  
  override func hitWall(hitDirection: RobotWallHitDirection, hitAngle: CGFloat) {
    cancelActiveAction()
    
    switch hitDirection {
    case .Front:
      turnRobotRight(180)
      moveAhead(20)
    case .Rear:
      moveAhead(80)
    case .Left:
      turnRobotRight(90)
      moveAhead(20)
    case .Right:
      turnRobotLeft(90)
      moveAhead(20)
    case .None:           // should never be none, but switch must be exhaustive
      break
    }
  }
  
  override func bulletHitEnemy(bullet: Bullet!) {
    // unimplemented
  }
  
}