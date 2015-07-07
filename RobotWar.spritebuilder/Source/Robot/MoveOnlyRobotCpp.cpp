//
//  MoveOnlyRobotCpp.cpp
//  RobotWar
//
//  Created by Akihito OKUHATA on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#include "MoveOnlyRobotCpp.h"

void MoveOnlyRobotCpp::run()
{
    bool first = true;
    while (true)
    {
        if (first) {
            this->moveAhead(150);
            first = false;
        }
        this->moveAhead(80);
        this->turnRobotRight(40);
    }
}
