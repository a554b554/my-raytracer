//
//  Light.cpp
//  raytracer
//
//  Created by DarkTango on 9/21/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#include "Light.h"
#include <iostream>
LightSample::LightSample():direction(0,0,0),irradiance(0,0,0){
    
}

LightSample::LightSample(Vec3d _dir, Vec3d _irr):direction(_dir),irradiance(_irr){
    
}

LightSample LightSample::zero(){
    return LightSample(Vec3d(0,0,0),Vec3d(0,0,0));
}

Light::Light(Vec3d _dir, Vec3d _irr):direction(_dir),irradiance(_irr){
    direction /= norm(direction);
}

Light::Light():direction(0,0,0),irradiance(0,0,0){
    
}

DirectionalLight::DirectionalLight(Vec3d _dir, Vec3d _irr):Light(_dir,_irr){
    
}

LightSample DirectionalLight::sample(Scene &scene, Vec3d position){
    Ray shadowRay = Ray(position, -this->direction);
    intersectResult shadowres = scene.intersect(shadowRay);
    if (shadowres.isHit) {
        return LightSample::zero();
    }
    return LightSample(this->direction, this->irradiance);
}


PointLight::PointLight(Vec3d _dir, Vec3d _irr, Vec3d position):Light(_dir,_irr),position(position){
    
}

PointLight::PointLight(Vec3d _irr, Vec3d position):Light(Vec3d(0,0,0),_irr),position(position){
    
}

LightSample PointLight::sample(Scene &scene, Vec3d position){
    Vec3d delta = this->position - position;
    double r = norm(delta);
    double r2 = r*r;
    Vec3d L = delta/r;
    
    //shadow test
    Ray shadowray(position, L);
    intersectResult res = scene.intersect(shadowray);
    if (res.isHit && res.distance <= r) {
        //cout<<res.distance<<endl;
        return LightSample::zero();
    }
    double attenuation = 1.0/r2;
    
    return LightSample(-L, this->irradiance*attenuation);
    
}

SpotLight::SpotLight(Vec3d _dir, Vec3d _irr, Vec3d _position, double _theta, double _phi, double _falloff):Light(_dir,_irr),position(_position),theta(_theta),phi(_phi),falloff(_falloff){
    costheta = cos(theta*M_PI/360);
    cosphi = cos(phi*M_PI/360);
    base = 1./(costheta-cosphi);
}

LightSample SpotLight::sample(Scene &scene, Vec3d position){
    Vec3d delta = this->position - position;
    double r = norm(delta);
    double r2 = r*r;
    Vec3d L = delta/r;
    
    
    double spot;
    double SdotL = (-direction).dot(L);
    if (SdotL >= this->costheta) {
        spot = 1;
    }
    else if(SdotL < this->cosphi){
        spot = 0;
    }
    
}















