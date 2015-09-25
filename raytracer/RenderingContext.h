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

class Scene;
Vec3d shading(Scene& scene, Ray& ray);
void renderCheckerboard();
void renderMaterial();
void renderDepth();
void renderNormal();
void renderLight();
#endif /* defined(__raytracer__RenderingContext__) */
