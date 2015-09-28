//
//  ObjLoader.h
//  raytracer
//
//  Created by DarkTango on 9/26/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#ifndef __raytracer__ObjLoader__
#define __raytracer__ObjLoader__

#include <stdio.h>
#include "common.h"
using namespace std;
class ObjLoader{
public:
    ObjLoader(string f);
    string filename;
    void parse(vector<Vec3d>& vertices, vector<Vec3i>& faces);
};




#endif /* defined(__raytracer__ObjLoader__) */
