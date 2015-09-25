//
//  Ray.cpp
//  raytracer
//
//  Created by DarkTango on 9/21/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#include "Ray.h"
Ray::Ray(Vec3d _origin, Vec3d _direction):origin(_origin),direction(_direction){
    refcount = 0;
    normalize();
}

void Ray::setDirection(Vec3d _dir){
    direction = _dir;
    normalize();
}

void Ray::normalize(){
    double factor = norm(direction);
    direction/=factor;
}

Vec3d Ray::getPoint(double t)const{
    return this->origin+this->direction*t;
}
