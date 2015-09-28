//
//  RenderingContext.h
//  raytracer
//
//  Created by DarkTango on 9/20/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#ifndef __raytracer__RenderingContext__
#define __raytracer__RenderingContext__

#include <stdio.h>
#include "Camera.h"
#include "common.h"
#include "Object.h"
#include "ObjLoader.h"

class Scene;

void rayTrace(Scene& scene, Size canvasSize, Mat& img);
Vec3d shading(Scene& scene, Ray& ray);
void renderCheckerboard();
void renderMaterial();
void renderDepth();
void renderNormal();
void renderLight();
void renderManyLights();
void materialTest();
void superRayTrace(Scene& scene, Size canvasSize, int sampleSize, Mat& img);
void adaptiveSuperRayTrace(Scene& scene, Size canvasSize, Mat& img);

void transformation();
#endif /* defined(__raytracer__RenderingContext__) */
