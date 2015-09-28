//
//  ObjLoader.cpp
//  raytracer
//
//  Created by DarkTango on 9/26/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#include "ObjLoader.h"

ObjLoader::ObjLoader(string f):filename(f){
    
}

void ObjLoader::parse(vector<Vec3d> &vertices, vector<Vec3i> &faces){
    FILE* fp;
    fp = fopen(filename.c_str(), "r");
    if (fp==NULL) {
        printf("file not exist:%s", filename.c_str());
        exit(1);
    }
    while (!feof(fp)) {
        char com;
        char buf[1024];
        fscanf(fp, "%c", &com);
        if (com=='#') {
            fgets(buf, 1024, fp);
        }
        else if (com=='v'){
            Vec3d v;
            fscanf(fp, " %lf %lf %lf", &v[0],&v[1],&v[2]);
            vertices.push_back(v);
        }
        else if (com=='f'){
            Vec3i f;
            fscanf(fp, " %d %d %d", &f[0], &f[1], &f[2]);
            f = f - Vec3i(1,1,1);
            faces.push_back(f);
        }
    }
}