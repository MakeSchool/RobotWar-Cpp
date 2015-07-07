//
//  Math.h
//  RobotWar
//
//  Created by Ken Watanabe on 2015/07/06.
//  Copyright (c) 2015年 MakeGamesWithUs. All rights reserved.
//

#ifndef RobotWar_Math_h
#define RobotWar_Math_h

#include "CppInterface.h"
#include <math.h>

namespace SuperKenMath {
    class KMath {
    public:
        static float distance(RWVec p1, RWVec p2) {
            return sqrtf(powf(p1.x - p2.x, 2) + powf(p1.y - p2.y, 2));
        }
        static RWVec divideInternally(RWVec p1, RWVec p2, float n, float m)
        {
            float x = (n * p1.x + m * p2.x) / (n + m);
            float y = (n * p1.y + m * p2.y) / (n + m);
            return RWVec(x, y);
        }
    };
}


#endif
