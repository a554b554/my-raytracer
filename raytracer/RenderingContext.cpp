//
//  RenderingContext.cpp
//  raytracer
//
//  Created by DarkTango on 9/20/15.
//  Copyright (c) 2015 DarkTango. All rights reserved.
//

#include "RenderingContext.h"
#include <iostream>

Scene::Scene(Camera* camera):cmr(camera){
    
}

void Scene::addObject(Object *obj){
    objectlist.push_back(obj);
}

void Scene::addLight(Light *lt){
    lights.push_back(lt);
}

intersectResult Scene::intersect(Ray &ray){
    intersectResult minResult = intersectResult::notHit();
    double minDist = INFINITY;
    for (int i = 0; i < this->objectlist.size(); i++) {
        intersectResult tmp = objectlist[i]->intersect(ray);
        if (!tmp.isHit) {
            continue;
        }
        if (tmp.distance<minDist) {
            minResult = tmp;
            minDist = tmp.distance;
        }
    }
    return minResult;
}

void renderDepth(){
    Vec3d eye(0,10,10);
    Vec3d forward(0,0,-1);
    Vec3d up(0,1,0);
    Camera cmr(eye, forward, up, 90);
    Object* ball = new Sphere(Vec3d(0,10,-10),10);
    
    Size canvas(256,256);
    Mat img(canvas,CV_64FC1);
    img.setTo(0);
    for (int i = 0; i < img.rows; i++) {
        double sy = 1 - (double)i/img.rows;
        for (int j = 0; j < img.cols; j++) {
            double sx = (double)j/img.cols;
            Ray r = cmr.generateRay(sx, sy);
            intersectResult res = ball->intersect(r);
            if (!res.isHit) {
                continue;
            }
            img.at<double>(i,j) = 1-res.distance/20;
        }
    }
    img*=255;
    img.convertTo(img, CV_8UC1);
//    imwrite("../../result/depth_"+to_string(getTickCount())+".jpg", img);
    imshow("result ", img);
    waitKey(0);
}

void renderNormal(){

    Vec3d eye(0,10,10);
    Vec3d forward(0,0,-1);
    Vec3d right(0,1,0);
     Camera cmr(eye, forward, right, 90);
    Object* ball = new Sphere(Vec3d(0,10,-10),10);
    
    Size canvas(256,256);
    Mat img(canvas,CV_64FC3);
    img.setTo(0);
    for (int i = 0; i < img.rows; i++) {
        double sy = 1 - (double)i/img.rows;
        for (int j = 0; j < img.cols; j++) {
            double sx = (double)j/img.cols;
            Ray r = cmr.generateRay(sx, sy);
            intersectResult res = ball->intersect(r);
            if (!res.isHit) {
                continue;
            }
            img.at<Vec3d>(i,j)[0] = (res.normal[0]+1)*127;
            img.at<Vec3d>(i,j)[1] = (res.normal[1]+1)*127;
            img.at<Vec3d>(i,j)[2] = (res.normal[2]+1)*127;
//            cout<<img.at<Vec3d>(i,j)<<endl;
//            cout<<res.normal<<endl;
        }
    }
    img.convertTo(img, CV_8UC3);
//    imwrite("../../result/normal.jpg", img);
    imshow("result ", img);
    waitKey(0);

}

//void renderMaterial(){
//    
//    Vec3d eye(0,5,15);
//    Vec3d forward(0,0,-1);
//    Vec3d up(0,1,0);
//    Size canv(400,400);
//    Camera cmr(eye, forward, up, 90);
//    Object* ball1 = new Sphere(Vec3d(-13,9,-15),10);
//    Object* ball2 = new Sphere(Vec3d(10,10,-15),12);
//    ball1->material = new Phong(Vec3d(0,1,0), Vec3d(1,1,1),8, 0.3);
//    ball2->material = new Phong(Vec3d(1,0,1), Vec3d(1,1,1),18,0.3);
////    ball1->material = new BasicMaterial(Vec3d(1,0,0), Vec3d(1,1,1),16,Vec3d(0,0,0));
////    ball2->material = new BasicMaterial(Vec3d(1,0,0), Vec3d(1,1,1),16,Vec3d(0,0,0));
//    Scene s(cmr);
//    s.light = Light(Vec3d(-1,1,1), Vec3d(1,1,1));
//    s.addObject(ball1);
//    s.addObject(ball2);
//    rayTrace(s, canv);
//    
//    
//}

//void renderCheckerboard(){
//    Vec3d eye(0,5,15);
//    Vec3d forward(0,0,-1);
//    Vec3d up(0,1,0);
//    Size canv(1600,1600);
//    Camera cmr(eye, forward, up, 90);
//    Object* ball1 = new Sphere(Vec3d(-10,10,-10),5);
//    Object* ball2 = new Sphere(Vec3d(10,10,-10),5);
//    Object* plane = new Plane(Vec3d(0,1,0),0);
//    ball1->material = new Phong(Vec3d(0,1,0), Vec3d(1,1,1),8, 0.3);
//    ball2->material = new Phong(Vec3d(1,0,1), Vec3d(1,1,1),18, 0.3);
//    plane->material = new checkerBoardMaterial(0.1, 0.2);
//    //    ball1->material = new BasicMaterial(Vec3d(1,0,0), Vec3d(1,1,1),16,Vec3d(0,0,0));
//    //    ball2->material = new BasicMaterial(Vec3d(1,0,0), Vec3d(1,1,1),16,Vec3d(0,0,0));
//    Scene s(cmr);
//    s.light = Light(Vec3d(1,1,1), Vec3d(1,1,1));
//    s.addObject(ball1);
//    s.addObject(ball2);
//    s.addObject(plane);
//    rayTrace(s, canv);
//}

void renderLight(){
    double eyex = 10;
    double eyey = 10;
    while(1){
        char c = waitKey(1);
        if (c=='a') {
            eyex+=1;
        }
        else if (c=='d'){
            eyex-=1;
        }
        else if (c=='q'){
            return;
        }
        Vec3d eye(0,eyex,eyey);
        Vec3d forward(0,0,-1);
        Vec3d up(0,1,0);
        double sz = 1600;
        Size canv(sz,sz);
        Camera cmr(eye, forward, up, 90);
        
        Object* plane1 = new Plane(Vec3d(0,1,0),0);
        Object* plane2 = new Plane(Vec3d(0,0,1),-50);
        Object* plane3 = new Plane(Vec3d(1,0,0),-20);
        Object* sphere = new Sphere(Vec3d(0,10,-10),10);
        Light* plight = new PointLight(Vec3d(2000,2000,2000),Vec3d(20,40,20));
        Light* dirlight = new DirectionalLight(Vec3d(-1.75,-2,-1.5), Vec3d(1,1,1));
        Scene s(&cmr);
//        s.addLight(dirlight);
        s.addLight(plight);
        s.addObject(plane1);
        s.addObject(plane2);
        s.addObject(plane3);
        s.addObject(sphere);
        plane1->material = new BasicMaterial();
        plane2->material = new BasicMaterial();
        plane3->material = new BasicMaterial();
//        sphere->material = new Phong(Vec3d(1,0,1), Vec3d(1,1,1),20, 0);
        sphere->material = new ColorfulBasicMaterial(Vec3d(1,0,0));
        rayTrace(s, canv);
    }
    
}



void onMouse(int event,int x, int y, int flag,void*p){
    if (event==CV_EVENT_LBUTTONDOWN) {
        //cout<<"x :"<<x<<" y: "<<y<<endl;
        Point* _p = reinterpret_cast<Point*>(p);
        _p->x = x;
        _p->y = y;
    }
}

Vec3d shading(Scene& scene, Ray& ray){
    double dist = INFINITY;
    Vec3d final;
    for (int cc = 0; cc < scene.objectlist.size(); cc++) {
        intersectResult res = scene.objectlist[cc]->intersect(ray);
        if (!res.isHit) {
            if (dist==INFINITY) {
                final = backgroundcolor;
            }
            continue;
        }
        Vec3d color = res.geometry->material->shade(ray, res.position, res.normal, scene);
        if (res.distance < dist) {
            dist = res.distance;
            final = color;
        }
        
    }
    
    return final;
}


void rayTrace( Scene& scene, Size canvasSize){
    Mat img(canvasSize,CV_64FC3);
    img.setTo(Scalar::all(0));
    
    for (int i = 0; i < canvasSize.height; i++) {
        double sy = 1-(double)i/canvasSize.height;
        for (int j = 0; j < canvasSize.width; j++) {
            double sx = (double)j/canvasSize.width;
            Ray r = scene.cmr->generateRay(sx, sy);
            img.at<Vec3d>(i,j) = shading(scene, r);
        }
    }
   

    
    //debug
    Point p;
    namedWindow("img");

    imshow("img", img);
    img *= 255;
    Mat result;
    img.convertTo(result, CV_8UC3);
    imwrite("../../result/pointlight.png", img);
//    imshow("result ", result);
//    waitKey(0);
}






