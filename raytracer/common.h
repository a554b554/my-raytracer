//
//  common.h
//  raytracer
//
//  Created by DarkTango on 9/20/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#ifndef __raytracer__common__
#define __raytracer__common__

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>
#include "Ray.h"
#include "Light.h"

using namespace std;
using namespace cv;

const Vec3d backgroundcolor(0,0,0);

Vec3d modulateColor(const Vec3d& c1, const Vec3d& c2);

#endif /* defined(__raytracer__common__) */
