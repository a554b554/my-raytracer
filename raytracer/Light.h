//
//  Light.h
//  raytracer
//
//  Created by DarkTango on 9/21/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#ifndef __raytracer__Light__
#define __raytracer__Light__

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include "RenderingContext.h"
using namespace cv;
struct LightSample{
    static LightSample zero();
    LightSample();
    LightSample(Vec3d _dir, Vec3d _irr);
    Vec3d direction;
    Vec3d irradiance;
};

class Scene;
class Light{
public:
    Light();
    Light(Vec3d _dir, Vec3d _irr);
    Vec3d direction;
    Vec3d irradiance;
    virtual LightSample sample(Scene& scene, Vec3d position) = 0;
};


class DirectionalLight : public Light{
public:
    DirectionalLight(Vec3d _dir, Vec3d _irr);
    LightSample sample(Scene& scene, Vec3d position);
};


class PointLight : public Light{
public:
    Vec3d position;
    PointLight(Vec3d _dir, Vec3d _irr, Vec3d position);
    PointLight(Vec3d _irr, Vec3d position);
    LightSample sample(Scene& scene, Vec3d position);
};


class SpotLight: public Light{
public:
    SpotLight(Vec3d _dir, Vec3d _irr, Vec3d _position, double _theta, double _phi, double _falloff);
    Vec3d position;
    double theta;
    double phi;
    double falloff;
    double costheta;
    double cosphi;
    double base;
    LightSample sample(Scene& scene, Vec3d position);
};


#endif /* defined(__raytracer__Light__) */
