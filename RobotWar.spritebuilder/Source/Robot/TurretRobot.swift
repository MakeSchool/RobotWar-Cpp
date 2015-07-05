//
//  TurretRobotSwift.swift
//  RobotWar
//
//  Created by Dion Larson on 7/2/15.
//  Copyright (c) 2015 MakeGamesWithUs. All rights reserved.
//

import Foundation

class TurretRobot: Robot {
  
  enum RobotState {                    // enum for keeping track of RobotState
    case Scanning, Firing
  }
  
  var currentRobotState: RobotState = .Scanning
  var lastEnemyHit = CGFloat(0.0)
  let gunToleranceAngle = CGFloat(2.0)
  let firingTimeout = CGFloat(2.5)
  
  override func run() {
    while true {
      switch currentRobotState {
      case .Scanning:
        turnGunRight(90)
      case .Firing:
        if currentTimestamp() - lastEnemyHit > firingTimeout {
          cancelActiveAction()
          currentRobotState = .Scanning
        } else {
          shoot()
        }
      }
    }
  }
  
  override func scannedRobot(robot: Robot!, atPosition position: CGPoint) {
    turnToEnemyPosition(position)
    
    lastEnemyHit = currentTimestamp()
    currentRobotState = .Firing
  }
  
  override func gotHit() {
    // unimplemented
  }
  
  override func hitWall(hitDirection: RobotWallHitDirection, hitAngle: CGFloat) {
    // unimplemented
  }
  
  override func bulletHitEnemy(bullet: Bullet!) {
    let position = bullet.bulletOwner.position()
    turnToEnemyPosition(position)
    
    lastEnemyHit = currentTimestamp()
    currentRobotState = .Firing
  }
  
  func turnToEnemyPosition(position: CGPoint) {
    cancelActiveAction()
    
    // calculate angle between turret and enemey
    var angleBetweenTurretAndEnemy = angleBetweenGunHeadingDirectionAndWorldPosition(position)
    
    // turn if necessary
    if angleBetweenTurretAndEnemy > gunToleranceAngle {
      turnGunRight(Int(abs(angleBetweenTurretAndEnemy)))
    } else if angleBetweenTurretAndEnemy < -gunToleranceAngle {
      turnGunLeft(Int(abs(angleBetweenTurretAndEnemy)))
    }
  }
  
}