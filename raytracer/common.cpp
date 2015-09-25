//
//  common.cpp
//  raytracer
//
//  Created by DarkTango on 9/20/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#include "common.h"
Vec3d modulateColor(const Vec3d& c1, const Vec3d& c2){
    return Vec3d(c1[0]*c2[0], c1[1]*c2[1], c1[2]*c2[2]);
}