//
//  Camera.h
//  raytracer
//
//  Created by DarkTango on 9/20/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#ifndef __raytracer__Camera__
#define __raytracer__Camera__

#include <stdio.h>
#include "common.h"
#include "Ray.h"
class Camera{
public:
    Camera(Vec3d _eye, Vec3d _forward, Vec3d _up, double _fov);
    Ray generateRay(double x, double y);
    bool checkValid();
    double w; //image width in world
    double h; //image height in world
    int rows;
    int cols;
    double fov;
    double fovScale;
private:
    Vec3d forward; //forward direction
    Vec3d right; //right direction
    Vec3d up; //up direction
    Vec3d eye; // eye position
    double f; //focal length

};


#endif /* defined(__raytracer__Camera__) */
