//
//  Camera.cpp
//  raytracer
//
//  Created by DarkTango on 9/20/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#include "Camera.h"
Camera::Camera(Vec3d _eye, Vec3d _forward, Vec3d _up, double _fov):eye(_eye),forward(_forward),up(_up),fov(_fov){
    right = forward.cross(up);
    fovScale = tan(fov*0.5*M_PI/180.)*2;
    //forward = forward/norm(forward);
//    right = right/norm(right);
//    up = up/norm(up);
}

Ray Camera::generateRay(double x, double y){
    Vec3d r = right*(x-0.5)*fovScale;
    Vec3d u = up*(y-0.5)*fovScale;
    return Ray(eye, forward+r+u);
}

