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

Sphere::Sphere(Vec3d loc, Vec3d sp, Vec3d se, double r):Object(loc),sp(sp),se(se),radius(r){
    sp/=norm(sp);
    se/=norm(se);
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
            
            Point2d cor;
            double phi = acos(max(min(-result.normal.dot(sp),1.),-1.));
            double term = max(min(se.dot(result.normal)/sin(phi),1.),-1.);
            double theta = acos(term)/(2*M_PI);
//            printf("%lf %lf %lf %lf\n",se.dot(result.normal), sin(phi), acos(term), acos(1.));
            if (isnan(phi)) {
                phi=0;
            }
            if(isnan(theta)){
                theta=0;
            }
            cor.x = phi/M_PI;
            if (sp.cross(se).dot(result.normal)>0) {
                cor.y = theta;
            }
            else{
                cor.y = 1 - theta;
            }
            result.textureCor = cor;
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

Triangle::Triangle(Vec3d _v1, Vec3d _v2, Vec3d _v3):v1(_v1),v2(_v2),v3(_v3),Object(Vec3d(0,0,0)){
    Vec3d v1v2 = v2-v1;
    Vec3d v1v3 = v3-v1;
    normal = v1v2.cross(v1v3);
    normal /= norm(normal);
    d = (-v1).dot(normal);
}

bool Triangle::isInTriangle(const Vec3d &v){
    Vec3d v1v2 = v2-v1;
    Vec3d v1v = v-v1;
    Vec3d v1v2xv1v = v1v2.cross(v1v);
    
    Vec3d v2v3 = v3-v2;
    Vec3d v2v = v-v2;
    Vec3d v2v3xv2v = v2v3.cross(v2v);
    if (v1v2xv1v.dot(v2v3xv2v) <= 0) {
        return false;
    }
    
    Vec3d v3v1 = v1-v3;
    Vec3d v3v = v-v3;
    Vec3d v3v1xv3v = v3v1.cross(v3v);
    if (v3v1xv3v.dot(v2v3xv2v) <= 0) {
        return false;
    }
    return true;
    
}

intersectResult Triangle::intersect(const Ray &ray){
    double a = ray.direction.dot(this->normal);
    if (a >= 0) {
        return intersectResult::notHit();
    }
    double b = this->normal.dot(ray.origin-this->v1);
    intersectResult res;
    res.isHit = true;
    res.geometry = this;
    res.distance = -b/a;
    res.position = ray.getPoint(res.distance);
    res.normal = this->normal;
    if (!isInTriangle(res.position)) {
        return intersectResult::notHit();
    }
    return res;
}

BiTriangle::BiTriangle(Vec3d _v1, Vec3d _v2, Vec3d _v3):Triangle(_v1,_v2,_v3){
    
}



intersectResult BiTriangle::intersect(const Ray& ray){
    double a = ray.direction.dot(this->normal);
    Vec3d n = this->normal;
    if (a > 0) {
        //this->normal = -this->normal;
        n = -this->normal;
    }
    double b = n.dot(ray.origin-this->v1);
    intersectResult res;
    res.isHit = true;
    res.geometry = this;
    res.distance = abs(-b/a);
    res.position = ray.getPoint(res.distance);
    res.normal = n;
    if (!isInTriangle(res.position)) {
        return intersectResult::notHit();
    }
    return res;
}
