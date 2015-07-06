//
//  EnemyParticle.h
//  RobotWar
//
//  Created by Ken Watanabe on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#ifndef __RobotWar__EnemyParticle__
#define __RobotWar__EnemyParticle__

#include "CppInterface.h"

class EnemyParticle
{
public:
    RWVec position;
    float direction;
    static float rotationSpeed;
    static float moveSpeed;
};


#endif /* defined(__RobotWar__EnemyParticle__) */
