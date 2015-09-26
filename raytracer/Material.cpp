//
//  Material.cpp
//  raytracer
//
//  Created by DarkTango on 9/20/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#include "Material.h"

Material::Material(Vec3d dif, Vec3d spec, double shin, double refl):diffuse(dif),specular(spec),shininess(shin),reflectiveness(refl){
    
}

Material::Material():diffuse(0,0,0),specular(0,0,0),shininess(0),reflectiveness(0){
    
}

checkerBoardMaterial::checkerBoardMaterial(double scl):scale(scl),Material(0,0,0,0){
    
}

checkerBoardMaterial::checkerBoardMaterial(double scl, double refl):scale(scl),Material(0,0,0,refl){
    
}

checkerBoardMaterial::checkerBoardMaterial(Vec3d dif, Vec3d spec, double shin, double refl, double scl):Material(dif,spec,shin,refl),scale(scl){
    
}



Vec3d checkerBoardMaterial::shade(const Ray& ray, const Vec3d position, const Vec3d normal, Scene& scene){
    Vec3d refdir = ray.direction -  2*ray.direction.dot(normal)*normal;
    Ray refl(position, refdir);
    refl.refcount = ray.refcount+1;
    Vec3d specterm;
    if (refl.refcount>5 || reflectiveness==0) {
        specterm = 0;
    }
    else{
        specterm = shading(scene, refl);
    }
    
    Vec3d term1 = abs((int)(floor(position[0]*this->scale) + floor(position[2]*this->scale))%2)    < 1?Vec3d(0,0,0):(1-reflectiveness)*Vec3d(1,1,1) ;
    return reflectiveness*specterm+term1;
}

ColorfulCheckerBoardMaterial::ColorfulCheckerBoardMaterial(double scl, Vec3d color):checkerBoardMaterial(scl),color(color){
    
}

Vec3d ColorfulCheckerBoardMaterial::shade(const Ray &ray, const Vec3d position, const Vec3d normal, Scene &scene){
    Vec3d refdir = ray.direction -  2*ray.direction.dot(normal)*normal;
    Ray refl(position, refdir);
    refl.refcount = ray.refcount+1;
   
    Vec3d term1 = abs((int)(floor(position[0]*this->scale) + floor(position[2]*this->scale))%2) < 1?color:Vec3d(1,1,1);
    
    Vec3d term2(0,0,0);
    for (int i = 0; i < scene.lights.size(); i++) {
        Light& light = *scene.lights[i];
        LightSample sample = light.sample(scene, position);
        if (sample.irradiance != Vec3d(0,0,0)) {
            double NdotL = normal.dot(-sample.direction);
            
            //angle < 90
            if (NdotL >= 0) {
                term2 += sample.irradiance*NdotL;
            }
        }
    }
    return modulateColor(term1, term2);
}


Phong::Phong(Vec3d dif, Vec3d spec, double shin, double refl):Material(dif,spec,shin,refl){

}

Vec3d Phong::shade(const Ray &ray, const Vec3d position, const Vec3d normal, Scene &scene){
    Vec3d ans(0,0,0);
    for (int i = 0; i < scene.lights.size(); i++) {
        
        Light& light = *(scene.lights[i]);
        Light* is = dynamic_cast<PointLight*>(&light);
        if (is) {
            continue;
        }
        LightSample samp = scene.lights[i]->sample(scene, position);
        if (samp.irradiance == Vec3d(0,0,0)) {
            continue;
        }
        double NdotL = normal.dot(-samp.direction);
        Vec3d H = (-samp.direction) - ray.direction;
        H = H/norm(H);
        
        double NdotH = normal.dot(H);
        Vec3d diffuseTerm = this->diffuse*(max(0., NdotL));
        Vec3d specularTerm = this->specular*(pow(max(NdotH, 0.), this->shininess));
        Vec3d reflecterm;
        Vec3d refdir = ray.direction -  2*ray.direction.dot(normal)*normal;
        Ray refl(position, refdir);
        refl.refcount = ray.refcount+1;
        if (refl.refcount>5 || reflectiveness==0) {
            reflecterm = 0;
        }
        else{
            reflecterm = shading(scene, refl);
        }
        ans += modulateColor(light.irradiance, diffuseTerm+specularTerm)+reflectiveness*reflecterm;
    }
    return ans;
}

BasicMaterial::BasicMaterial(Vec3d dif, Vec3d spec, double shin, double refl):Material(dif,spec,shin,refl){
}

BasicMaterial::BasicMaterial():Material(Vec3d(0,0,0),Vec3d(0,0,0),0,0){
    
}

Vec3d BasicMaterial::shade(const Ray &ray, const Vec3d position, const Vec3d normal, Scene &scene){
    Vec3d color(0,0,0);
    for (int i = 0; i < scene.lights.size(); i++) {
        Light& light = *scene.lights[i];
        LightSample sample = light.sample(scene, position);
        if (sample.irradiance != Vec3d(0,0,0)) {
            double NdotL = normal.dot(-sample.direction);
            
            //angle < 90
            if (NdotL >= 0) {
                color += sample.irradiance*NdotL;
            }
        }
    }
    return color;
}


ColorfulBasicMaterial::ColorfulBasicMaterial(Vec3d color):BasicMaterial(),color(color){
    
}


Vec3d ColorfulBasicMaterial::shade(const Ray &ray, const Vec3d position, const Vec3d normal, Scene &scene){
    Vec3d c = BasicMaterial::shade(ray, position, normal, scene);
    return modulateColor(c, this->color);
}


Lambertian::Lambertian(Vec3d color):color(color),Material(){
    
}

Vec3d Lambertian::shade(const Ray &ray, const Vec3d position, const Vec3d normal, Scene &scene){
    Vec3d ir(0,0,0);
    for (int i = 0; i < scene.lights.size(); i++) {
        Light& light = *scene.lights[i];
        LightSample sample = light.sample(scene, position);
        if (sample.irradiance != Vec3d(0,0,0)) {
            double NdotL = normal.dot(-sample.direction);
            
            //angle < 90
            if (NdotL >= 0) {
                ir += sample.irradiance*INV_PI;
            }
        }
    }
    return modulateColor(this->color, ir);
}




