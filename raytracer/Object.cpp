//
//  Object.cpp
//  raytracer
//
//  Created by DarkTango on 9/20/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#include "Object.h"
#include <iostream>

intersectResult intersectResult::notHit(){
    intersectResult r;
    r.isHit = false;
    return r;
}

Object::~Object(){
    
}

Object::Object(){
    
}

Object::Object(Vec3d location):location(location){
    
}

void Object::Rotation(Vec3d rot){
    
}

void Object::Translation(Vec3d t){
    
}

void Object::Scalation(double s){
    
}

Sphere::Sphere(Vec3d loc, double r):Object(loc),radius(r){
    
}

Sphere::Sphere(Vec3d loc, Vec3d sp, Vec3d se, double r):Object(loc),sp(sp),se(se),radius(r){
    sp/=norm(sp);
    se/=norm(se);
}

void Object::setMaterial(Material *m){
    this->material = m;
}

intersectResult Object::intersect(const Ray& ray){
    return intersectResult::notHit();
}

const double tolerance = 0.001;

intersectResult Sphere::intersect(const Ray &ray){
    Vec3d r = ray.origin - this->location;
    double a = r.dot(r) - this->radius*this->radius;
    double Ddot = ray.direction.dot(r);
    intersectResult result;
    result.isinside = false;

    if (a < 0) {
        result.isinside = true;
        
    }
    
    //test if hit
    if (Ddot <= 0) {
        double dis = Ddot*Ddot - a;
        if (dis >= 0) {
            
            result.isHit = true;
            result.geometry = this;
            result.distance = -Ddot - sqrt(dis);
            
            result.position = ray.getPoint(result.distance);
            result.normal = result.position-this->location;
            result.normal /= norm(result.normal);
            if (result.isinside) {
                result.distance = -Ddot + sqrt(dis);
                result.normal = - result.normal;
            }
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

Plane::Plane():Object(0){
    
}

Plane::Plane(Vec3d _normal, double _d):normal(_normal),d(_d),Object(0){
    normal /= norm(normal);
    this->location = this->normal*this->d;
}


intersectResult Plane::intersect(const Ray& ray){
    double a = ray.direction.dot(this->normal);
    Vec3d _normal = this->normal;
    intersectResult res;
    res.isinside = false;
    if (a >= 0) {
        a = -a;
        _normal = -this->normal;
        res.isinside = true;
    }
    double b = _normal.dot(ray.origin-this->location);
    if (b <= 0) {
        return intersectResult::notHit();
    }
    
    res.isHit = true;
    res.geometry = this;
    res.distance = -b/a;
    res.position = ray.getPoint(res.distance);
    res.normal = _normal;
    return res;
}

Slab::Slab():Object(0){
    
}

Slab::Slab(Plane p1, double thick):p1(p1),Object(0){
    p2.normal = -p1.normal;
    p2.d = p1.d + thick;
}

intersectResult Slab::intersect(const Ray &ray){
    intersectResult r1 = p1.intersect(ray);
    intersectResult r2 = p2.intersect(ray);
    if (r1.isHit && !r2.isHit) {
        r1.isinside = true;
        return r1;
    }
    
    if (!r1.isHit && r2.isHit) {
        r2.isinside = true;
        return r2;
    }
    
    if (!r1.isHit&&!r2.isHit) {
        return intersectResult::notHit();
    }
    
    
    if (r1.distance < r2.distance) {
        r1.isinside = false;
        return r1;
    }
    else{
        r2.isinside = false;
        return r2;
    }
    
    
}

Cube::Cube(Slab* s1, Slab* s2, Slab* s3):Object(0){
    slabs[0] = s1;
    slabs[1] = s2;
    slabs[2] = s3;
}

intersectResult Cube::intersect(const Ray &ray){
    intersectResult minRes = intersectResult::notHit();
    double minDist = INFINITY;
    for (int i = 0; i < 3; i++) {
        intersectResult tmp = slabs[i]->intersect(ray);
        if (!tmp.isHit) {
            continue;
        }
        if (tmp.distance < minDist) {
            minRes = tmp;
            minDist = tmp.distance;
        }
    }
    return minRes;
}

UnionObject::UnionObject(vector<Object*> _objs):objs(_objs),Object(0){
    
}

UnionObject::UnionObject(vector<Vec3d>& vertices, vector<Vec3i>& faces):Object(0){

    for (int i = 0; i < faces.size(); i++) {
        Object* face = new Triangle(vertices[faces[i][0]],vertices[faces[i][1]],vertices[faces[i][2]],location);
        objs.push_back(face);
    }
}

UnionObject::UnionObject(vector<Vec3d>& vertices, vector<Vec3i>& faces, Vec3d offset, double scale):Object(offset){
    
  
    
    
    double r = 0;
    for (int i = 0; i < vertices.size(); i++) {
        double dis = norm(vertices[i]);
        if (dis > r) {
            r = dis;
        }
    }
    boundingSphere = new Sphere(location, r*scale);
    boundingSphere->material = new ColorfulBasicMaterial(Vec3d(0,0,1));
    
    for (int i = 0; i < faces.size(); i++) {
        Vec3d v1 = scale*vertices[faces[i][0]] + location;
        Vec3d v2 = scale*vertices[faces[i][1]] + location;
        Vec3d v3 = scale*vertices[faces[i][2]] + location;
        
        Object* face = new BiTriangle(v1,v2,v3);
        objs.push_back(face);
    }
}


void UnionObject::setMaterial(Material *m){
    for (int i = 0; i < objs.size(); i++) {
        objs[i]->material = m;
    }
}

void UnionObject::Rotation(Vec3d rot){
    for (int i = 0; i < objs.size(); i++) {
        objs[i]->Rotation(rot);
    }
}

void UnionObject::Translation(Vec3d t){
    
}

void UnionObject::Scalation(double s){
    
}

intersectResult UnionObject::intersect(const Ray &ray){
    
    if (!boundingSphere->intersect(ray).isHit) {
        return intersectResult::notHit();
    }
    
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

Triangle::Triangle(Vec3d _v1, Vec3d _v2, Vec3d _v3, Vec3d loc):v1(_v1),v2(_v2),v3(_v3),Object(loc){
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

void Triangle::Rotation(Vec3d rot){
    
    v1 -= location;
    v2 -= location;
    v3 -= location;
    
    //rotx
    Matx33d rotx;
    rotx<<1,0,0,
           0,cos(rot[0]),-sin(rot[0]),
           0,sin(rot[0]),cos(rot[0]);
    v1 = rotx * v1;
    v2 = rotx * v2;
    v3 = rotx * v3;
    
    //roty
    Matx33d roty;
    roty << cos(rot[1]), 0, sin(rot[1]),
            0, 1, 0,
            -sin(rot[1]), 0, cos(rot[1]);
    v1 = roty * v1;
    v2 = roty * v2;
    v3 = roty * v3;
    
    
    //rotz
    Matx33d rotz;
    rotz<< cos(rot[2]), -sin(rot[2]), 0,
            sin(rot[2]), cos(rot[2]), 0,
            0, 0, 1;
    v1 = rotz * v1;
    v2 = rotz * v2;
    v3 = rotz * v3;
    
    v1 += location;
    v2 += location;
    v3 += location;
    
}

void Triangle::Translation(Vec3d t){
    
}

void Triangle::Scalation(double s){
    
}


BiTriangle::BiTriangle(Vec3d _v1, Vec3d _v2, Vec3d _v3):Triangle(_v1,_v2,_v3){
    
}

BiTriangle::BiTriangle(Vec3d _v1, Vec3d _v2, Vec3d _v3, Vec3d loc):Triangle(_v1,_v2,_v3,loc){
    
}


intersectResult BiTriangle::intersect(const Ray& ray){
    double a = ray.direction.dot(this->normal);
    Vec3d n = this->normal;
    intersectResult res;
    res.isinside = false;
    if (a > 0) {
        //this->normal = -this->normal;
        n = -this->normal;
        res.isinside = true;
    }
    double b = n.dot(ray.origin-this->v1);
    
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
