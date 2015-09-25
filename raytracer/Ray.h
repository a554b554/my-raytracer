//
//  Ray.h
//  raytracer
//
//  Created by DarkTango on 9/21/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#ifndef __raytracer__Ray__
#define __raytracer__Ray__

#include <stdio.h>
#include <opencv2/core/core.hpp>
using namespace cv;
class Ray{
public:
    Ray();
    Ray(Vec3d _origin, Vec3d _direction);
    Vec3d origin;
    Vec3d direction;
    void setDirection(Vec3d _dir);
    void normalize();
    Vec3d getPoint(double t) const;
    int refcount;
};
#endif /* defined(__raytracer__Ray__) */
