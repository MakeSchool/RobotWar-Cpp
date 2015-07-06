//
//  ShootOnlyRobotCpp.cpp
//  RobotWar
//
//  Created by Akihito OKUHATA on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include "ShootOnlyRobotCpp.h"
#include <iostream>
#include <math.h>


void ShootOnlyRobotCpp::run()
{
    bool turnRight = true;
    while (true)
    {
        this->shoot();
        if (turnRight) {
            this->turnGunRight(20);
        } else {
            this->turnGunLeft(20);
        }
        if (abs((float)this->angleBetweenGunHeadingDirectionAndWorldPosition(this->headingDirection())) > 80.0f) {
            turnRight = !turnRight;
        }
    }
}
