//
//  Object.h
//  raytracer
//
//  Created by DarkTango on 9/20/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#ifndef __raytracer__Object__
#define __raytracer__Object__

#include <stdio.h>
#include "common.h"
#include "RenderingContext.h"


using namespace cv;

class Material;
////////////////Object/////////////////////

class Object;
struct intersectResult{
    static intersectResult notHit();
    bool isHit;
    Object* geometry;
    Vec3d position;
    double distance;
    Vec3d normal;
};


class Object{
public:
    Object();
    Object(Vec3d location);
    Vec3d location;
    Material* material;
    virtual intersectResult intersect(const Ray& ray);
    virtual ~Object()=0;
};



class Sphere : public Object{
public:
    double radius;
    Sphere();
    Sphere(Vec3d loc, double r);
    intersectResult intersect(const Ray& ray);
};


class Plane: public Object{
public:
    Plane(Vec3d _normal, double _d);
    Vec3d normal;
    double d;
    intersectResult intersect(const Ray& ray);
    
};

class UnionObject : public Object{
public:
    UnionObject(vector<Object*> _objs);
    intersectResult intersect(const Ray& ray);
private:
    vector<Object*> objs;
};


class Triangle : public Object{
public:
    Triangle(Vec3d _v1, Vec3d _v2, Vec3d _v3);
    Vec3d normal;
    double d;
    Vec3d v1;
    Vec3d v2;
    Vec3d v3;
    bool isInTriangle(const Vec3d& v);
    intersectResult intersect(const Ray& ray);
};

//no use
class BiTriangle: public Triangle{
public:
    BiTriangle(Vec3d _v1, Vec3d _v2, Vec3d _v3);
    intersectResult intersect(const Ray& ray);
};


/////////////Scene///////////////////////
class Light;
class Camera;
class Scene{
public:
    Scene(Camera* camera);
    intersectResult intersect(Ray& ray);
    void addObject(Object* obj);
    void addLight(Light* lt);
    vector<Object*> objectlist;
    Camera* cmr;
    vector<Light*> lights;
};


//////////////Material////////////////////
class Material{
public:
    Material();
    Material(Vec3d dif, Vec3d spec, double shin, double refl);
    Vec3d diffuse;
    Vec3d specular;
    double shininess;
    double reflectiveness;
    virtual Vec3d shade(const Ray& ray, const Vec3d position, const Vec3d normal, Scene& scene) = 0;
};

//only for test
class checkerBoardMaterial : public Material{
public:
    checkerBoardMaterial(Vec3d dif, Vec3d spec, double shin, double refl, double scl);
    checkerBoardMaterial(double scl);
    checkerBoardMaterial(double scl, double refl);
    Vec3d shade(const Ray& ray, const Vec3d position, const Vec3d normal, Scene& scene);
    double scale;
};

class ColorfulCheckerBoardMaterial : public checkerBoardMaterial{
public:
    ColorfulCheckerBoardMaterial(double scl, Vec3d color);
    Vec3d color;
    Vec3d shade(const Ray& ray, const Vec3d position, const Vec3d normal, Scene& scene);
};


class Phong : public Material{
public:
    Phong(Vec3d dif, Vec3d spec, double shin, double refl);
    Vec3d shade(const Ray& ray, const Vec3d position, const Vec3d normal, Scene& scene);
};

class BasicMaterial : public Material{
public:
    BasicMaterial();
    BasicMaterial(Vec3d dif, Vec3d spec, double shin, double refl);
    Vec3d shade(const Ray& ray, const Vec3d position, const Vec3d normal, Scene& scene);
};

class ColorfulBasicMaterial : public BasicMaterial{
public:
    ColorfulBasicMaterial(Vec3d color);
    Vec3d color;
    Vec3d shade(const Ray& ray, const Vec3d position, const Vec3d normal, Scene& scene);
};

class Lambertian : public Material{
public:
    Lambertian(Vec3d color);
    Vec3d shade(const Ray& ray, const Vec3d position, const Vec3d normal, Scene& scene);
    Vec3d color;
};

void rayTrace(Scene& scene, Size canvasSize);
#endif /* defined(__raytracer__Object__) */
