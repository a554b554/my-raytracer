//
//  Object.cpp
//  raytracer
//
//  Created by DarkTango on 9/20/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#include "Object.h"


intersectResult intersectResult::notHit(){
    intersectResult r;
    r.isHit = false;
    return r;
}

Object::~Object(){
    
}

Object::Object(Vec3d location):location(location){
    
}

Sphere::Sphere(Vec3d loc, double r):Object(loc),radius(r){
    
}

intersectResult Object::intersect(const Ray& ray){
    return intersectResult::notHit();
}



intersectResult Sphere::intersect(const Ray &ray){
    Vec3d r = ray.origin - this->location;
    double a = r.dot(r) - this->radius*this->radius;
    double Ddot = ray.direction.dot(r);
    
    //test if hit
    if (Ddot <= 0) {
        double dis = Ddot*Ddot - a;
        if (dis >= 0) {
            intersectResult result;
            result.isHit = true;
            result.geometry = this;
            result.distance = -Ddot - sqrt(dis);
            result.position = ray.getPoint(result.distance);
            result.normal = result.position-this->location;
            result.normal /= norm(result.normal);
            return result;
        }
    }
    return intersectResult::notHit();
}

Plane::Plane(Vec3d _normal, double _d):normal(_normal),d(_d),Object(0){
    normal /= norm(normal);
    this->location = this->normal*this->d;
}


intersectResult Plane::intersect(const Ray& ray){
    double a = ray.direction.dot(this->normal);
    if (a >= 0) {
        return intersectResult::notHit();
    }
    double b = this->normal.dot(ray.origin-this->location);
    intersectResult res;
    res.isHit = true;
    res.geometry = this;
    res.distance = -b/a;
    res.position = ray.getPoint(res.distance);
    res.normal = this->normal;
    return res;
}

UnionObject::UnionObject(vector<Object*> _objs):objs(_objs),Object(0){
    
}

intersectResult UnionObject::intersect(const Ray &ray){
    double minDist = INFINITY;
    intersectResult minResult = intersectResult::notHit();
    for (int i = 0; i < objs.size(); i++) {
        intersectResult tmp = objs[i]->intersect(ray);
        if (tmp.isHit && tmp.distance < minDist) {
            minDist = tmp.distance;
            minResult = tmp;
        }
    }
    
    return minResult;
    
}




