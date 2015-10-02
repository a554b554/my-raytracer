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

double tolerance = 0.0001;

intersectResult Scene::intersect(Ray &ray){
    intersectResult minResult = intersectResult::notHit();
    double minDist = INFINITY;
    for (int i = 0; i < this->objectlist.size(); i++) {
        intersectResult tmp = objectlist[i]->intersect(ray);
        if (!tmp.isHit || tmp.distance < tolerance) {
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

void renderCheckerboard(){
    Vec3d eye(0,5,15);
    Vec3d forward(0,0,-1);
    Vec3d up(0,1,0);
    int sz =1000;
    Size canv(sz,sz);
    Camera cmr(eye, forward, up, 90);
    Object* ball1 = new Sphere(Vec3d(-10,10,-10),5);
    Object* ball2 = new Sphere(Vec3d(10,10,-10),5);
    Object* plane = new Plane(Vec3d(0,1,0),0);
    ball1->material = new Phong(Vec3d(0,1,0), Vec3d(1,1,1),8, 0.3);
    ball2->material = new Phong(Vec3d(1,0,1), Vec3d(1,1,1),18, 0.3);
    plane->material = new checkerBoardMaterial(0.1, 0.2);
    //    ball1->material = new BasicMaterial(Vec3d(1,0,0), Vec3d(1,1,1),16,Vec3d(0,0,0));
    //    ball2->material = new BasicMaterial(Vec3d(1,0,0), Vec3d(1,1,1),16,Vec3d(0,0,0));
    Scene s(&cmr);
    s.addLight(new DirectionalLight(Vec3d(1,1,1), Vec3d(1,1,1)));
    s.addObject(ball1);
    s.addObject(ball2);
    s.addObject(plane);
//    adaptiveSuperRayTrace(s, canv);
//    rayTrace(s, canv);
    waitKey(0);
}

void renderLight(){
    double eyex = 10;
    double eyey = 15;
//    double aa = 0.1;
    while(1){
        char c = waitKey(12);
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
        double sz = 400;
        Size canv(sz,sz);
        Camera cmr(eye, forward, up, 90);
        
        Object* plane1 = new Plane(Vec3d(0,1,0),0);
        Object* plane2 = new Plane(Vec3d(0,0,1),-50);
        Object* plane3 = new Plane(Vec3d(1,0,0),-20);
        Object* sphere = new Sphere(Vec3d(0,10,-10),10);
        Light* plight = new PointLight(Vec3d(700,700,700),Vec3d(20,40,20));
        Light* dirlight = new DirectionalLight(Vec3d(-1.75,-2,-1.5), Vec3d(1,1,1));
        
        Light* Rspotligt = new SpotLight(Vec3d(0,-1,-1), Vec3d(0,0,3000), Vec3d(0,30,10),20,30,1);
        Light* Bspotligt = new SpotLight(Vec3d(0,-1,-1), Vec3d(0,3000,0), Vec3d(6,30,20),20,30,1);
        Light* Gspotligt = new SpotLight(Vec3d(0,-1,-1), Vec3d(3000,0,0), Vec3d(-6,30,20),20,30,1);
//        aa = aa+0.1;
        Scene s(&cmr);
        s.addLight(dirlight);
        s.addLight(plight);
//        s.addLight(Rspotligt);
//        s.addLight(Bspotligt);
//        s.addLight(Gspotligt);
        s.addObject(plane1);
        s.addObject(plane2);
        s.addObject(plane3);
        s.addObject(sphere);
        plane1->material = new BasicMaterial();
        plane2->material = new BasicMaterial();
        plane3->material = new BasicMaterial();
        sphere->material = new Phong(Vec3d(1,0,1), Vec3d(1,1,1),20, 0);
//        sphere->material = new ColorfulBasicMaterial(Vec3d(1,1,1));
//        rayTrace(s, canv);
    }
    
}

void renderManyLights(){
    Vec3d eye(0,10,10);
    Vec3d forward(0,0,-1);
    Vec3d up(0,1,0);
    double sz = 400;
    Size canv(sz,sz);
    Camera cmr(eye, forward, up, 90);
    
    Object* plane1 = new Plane(Vec3d(0,1,0),0);
    Object* plane2 = new Plane(Vec3d(0,0,1),-50);
    Object* plane3 = new Plane(Vec3d(1,0,0),-20);
    Vec3d o(0,10,-10);
    
    Object* sphere = new Sphere(o,Vec3d(-1,1,0),Vec3d(1,1,0),10);
    Vec3d p1(0,6,3),p2(3,6,0),p3(-10,6,-10);
 
    Object* tr = new BiTriangle(p1,p2,p3);
    Object* tr2 = new Triangle(p3,p2,p1);
    tr->material = new BasicMaterial();
    tr2->material = new BasicMaterial();
    Scene s(&cmr);
    Light* filllight = new DirectionalLight(Vec3d(1.5,1,0.5),0.25*Vec3d(1,1,1));
    for (int x = 10; x <= 30; x+=4) {
        for (int z = 20; z <= 40; z+=4) {
            s.addLight(new PointLight(200*Vec3d(1,1,1), Vec3d(x,50,z)));
        }
    }
//    s.addObject(tr);

//    s.addObject(tr2);
    s.addLight(filllight);
    s.addObject(plane1);
    s.addObject(plane2);
    s.addObject(plane3);
    s.addObject(sphere);
    plane1->material = new ColorfulBasicMaterial(Vec3d(0,0,1));
    plane2->material = new ColorfulBasicMaterial(Vec3d(0,1,0));
    plane3->material = new ColorfulBasicMaterial(Vec3d(1,0,0));
    //        sphere->material = new Phong(Vec3d(1,0,1), Vec3d(1,1,1),20, 0);
    Mat tex = imread("../../resources/p.png");
    sphere->material = new TexturedBasicMaterial(tex);
//    sphere->material = new Phong(Vec3d(1,0,1), Vec3d(1,1,1),20, 0);
//    rayTrace(s, canv);
//    adaptiveSuperRayTrace(s, canv);
    waitKey(0);
}


void materialTest(){

    Vec3d eye(0,30,40);
    Vec3d forward(0,0,-1);
    Vec3d up(0,1,0);
    double sz = 400;
    Size canv(sz,sz);
    Camera cmr(eye, forward, up, 90);
    
    
    Object* aa = new Triangle(Vec3d(1,-1,1),Vec3d(0,1,0),Vec3d(1,1,0));
    aa->Rotation(Vec3d(1,1,1));
    
    Object* plane1 = new Plane(Vec3d(0,-1,0),0);
    Object* plane2 = new Plane(Vec3d(0,0,1),-50);
    Object* plane3 = new Plane(Vec3d(1,0,0),-20);
    Vec3d o(0,10,-10);
    plane1->setMaterial(new ColorfulBasicMaterial(Vec3d(0,0,1)));
    plane2->setMaterial(new ColorfulBasicMaterial(Vec3d(0,1,0)));
    plane3->setMaterial(new ColorfulBasicMaterial(Vec3d(1,0,0)));
    
    ObjLoader loader("../../resources/bunny.obj");
    vector<Vec3d> vv;
    vector<Vec3i> ff;
    loader.parse(vv, ff);
    
    Object* bunny = new UnionObject(vv,ff,Vec3d(0,10,0),60);
    bunny->setMaterial(new ColorfulBasicMaterial(Vec3d(1,1,1)));
    Vec3d p1(0,6,3),p2(3,6,0),p3(-10,6,-10);
    
    Object* ball = new Sphere(Vec3d(0,0,0),12);
    ball->setMaterial(new ColorfulBasicMaterial(Vec3d(1,1,1)));
    
    
    Scene s(&cmr);
    s.addObject(bunny);
//    s.addObject(dynamic_cast<UnionObject*>(bunny)->boundingSphere);
//    s.addObject(ball);
    s.addObject(plane1);
    s.addObject(plane2);
    s.addObject(plane3);
    s.addLight(new PointLight(800*Vec3d(1,1,1), Vec3d(10,50,20)));
    s.addLight(new PointLight(800*Vec3d(1,1,1), Vec3d(15,50,25)));
    
//    adaptiveSuperRayTrace(s, canv);
//    rayTrace(s, canv);
    waitKey(0);

}


void transformation(){
    Vec3d eye(0,30,40);
    Vec3d forward(0,0,-1);
    Vec3d up(0,1,0);
    double sz = 400;
    Size canv(sz,sz);
    Camera cmr(eye, forward, up, 90);
    
    
    Object* aa = new Triangle(Vec3d(1,-1,1),Vec3d(0,1,0),Vec3d(1,1,0));
    aa->Rotation(Vec3d(1,1,1));
    
    Object* plane1 = new Plane(Vec3d(0,-1,0),0);
    Object* plane2 = new Plane(Vec3d(0,0,1),-50);
    Object* plane3 = new Plane(Vec3d(1,0,0),-20);
    Vec3d o(0,10,-10);
    plane1->setMaterial(new ColorfulBasicMaterial(Vec3d(0,0,1)));
    plane2->setMaterial(new ColorfulBasicMaterial(Vec3d(0,1,0)));
    plane3->setMaterial(new ColorfulBasicMaterial(Vec3d(1,0,0)));
    
    ObjLoader loader("../../resources/bunny.obj");
    vector<Vec3d> vv;
    vector<Vec3i> ff;
    loader.parse(vv, ff);
    
    Object* bunny = new UnionObject(vv,ff,Vec3d(0,10,0),70);
    bunny->setMaterial(new ColorfulBasicMaterial(Vec3d(1,1,1)));
    Vec3d p1(0,6,3),p2(3,6,0),p3(-10,6,-10);
    
    Object* ball = new Sphere(Vec3d(0,0,0),12);
    ball->setMaterial(new ColorfulBasicMaterial(Vec3d(1,1,1)));
    
    
    Object* trian = new BiTriangle(Vec3d(10,1,1),Vec3d(1,10,1),Vec3d(10,30,1),Vec3d(0,0,0));
    trian->material = ball->material;
    
    
    Scene s(&cmr);
    s.addObject(bunny);
    //    s.addObject(dynamic_cast<UnionObject*>(bunny)->boundingSphere);
    //    s.addObject(ball);
    s.addObject(plane1);
    s.addObject(plane2);
    s.addObject(plane3);
//    s.addObject(trian);
    s.addLight(new PointLight(800*Vec3d(1,1,1), Vec3d(10,50,20)));
    s.addLight(new PointLight(800*Vec3d(1,1,1), Vec3d(15,50,25)));
    
    //    adaptiveSuperRayTrace(s, canv);
    Vec3d angle(0,0,0);
    Mat out;
//    for (int i = 0; i < 20; i++) {
//        bunny->Rotation(Vec3d(0,M_PI/18.,0));
//        rayTrace(s, canv, out);
//       
//        imwrite("../../result/rotation/rot"+to_string(i)+".png", out);
//        waitKey(1);
//    }
    
//    for (int i = 0; i < 20; i++) {
//        bunny->Translation(Vec3d(0,1,0));
//        rayTrace(s, canv, out);
//        
//        imwrite("../../result/translation/trans"+to_string(i)+".png", out);
//        waitKey(1);
//    }
    
    
    for (int i = 0; i < 15; i++) {
        bunny->Scalation(1.1);
        rayTrace(s, canv, out);
        imwrite("../../result/escalation/escl"+to_string(i)+".png", out);
        waitKey(1);
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
        Vec3d color = res.geometry->material->shade(ray, res.position, res.normal, scene, res.textureCor);
        if (res.distance < dist) {
            dist = res.distance;
            final = color;
        }
        
    }
    
    return final;
}


void rayTrace(Scene& scene, Size canvasSize, Mat& img){
    img.create(canvasSize,CV_64FC3);
    img.setTo(Scalar::all(0));
    
    for (int i = 0; i < canvasSize.height; i++) {
        double sy = 1-(double)i/canvasSize.height;
        for (int j = 0; j < canvasSize.width; j++) {
            double sx = (double)j/canvasSize.width;
            Ray r = scene.cmr->generateRay(sx, sy);
            img.at<Vec3d>(i,j) = shading(scene, r);
            
        }
        cout<<"i: "<<i<<endl;
    }
   

    
    //debug
    Point p;
    namedWindow("img");

    imshow("img", img);
    img *= 255;
    Mat result;
    img.convertTo(result, CV_8UC3);
//    imwrite("../../result/bunny2.png", img);
//    imshow("result ", result);
//    waitKey(0);
}

void superRayTrace(Scene& scene, Size canvasSize, int sampleSize, Mat& img){
    img.create(canvasSize,CV_64FC3);
    img.setTo(Scalar::all(0));
    double stepx = 1./canvasSize.width/sampleSize;
    double stepy = 1./canvasSize.height/sampleSize;
    for (int i = 0; i < canvasSize.height; i++) {
        double sy = 1-(double)i/canvasSize.height;
        for (int j = 0; j < canvasSize.width; j++) {
            Vec3d final(0,0,0);
            double sx = (double)j/canvasSize.width;
            for (int ci=0; ci<sampleSize; ci++) {
                for (int cj = 0; cj<sampleSize; cj++) {
                    double cx = sx + cj*stepx;
                    double cy = sy + ci*stepy;
                    Ray r = scene.cmr->generateRay(cx, cy);
                    final += shading(scene, r);
                }
            }
            
            final /= sampleSize*sampleSize;
            
            img.at<Vec3d>(i,j) = final;
            //            cout<<"i: "<<i<<" j: "<<j<<endl;
        }
    }
    
    
    
    //debug
    Point p;
    namedWindow("img");
    
//    imshow("img", img);
    img *= 255;
    Mat result;
    img.convertTo(img, CV_8UC3);
//    imwrite("../../result/antialiasing.png", img);
    imshow("result ", img);
    waitKey(1);
}

void adaptiveSuperRayTrace(Scene& scene, Size canvasSize, Mat& img){
    img.create(canvasSize,CV_64FC3);
    img.setTo(Scalar::all(0));
    
    double stepx = 1./canvasSize.width;
    double stepy = 1./canvasSize.height;
    
    for (int i = 0; i < canvasSize.height; i++) {
        double sy = 1-(double)i/canvasSize.height;
        for (int j = 0; j < canvasSize.width; j++) {
            double sx = (double)j/canvasSize.width;
            
            Vec3d final(0,0,0);
            double sxs[5];
            double sys[5];
            
            sxs[0] = sx;
            sys[0] = sy;
            sxs[1] = sx+stepx;
            sys[1] = sy;
            sxs[2] = sx;
            sys[2] = sy+stepy;
            sxs[3] = sx+stepx;
            sys[3] = sy+stepy;
            sxs[4] = sx+stepx/2;
            sys[4] = sy+stepy/2;
            
            for (int k = 0; k < 5; k++) {
                Ray r = scene.cmr->generateRay(sxs[k], sys[k]);
                final += shading(scene, r);
            }
            
            final /= 5;
            img.at<Vec3d>(i,j) = final;
            
        }
        cout<<"i: "<<i<<endl;
    }
    
    
    
    //debug
    Point p;
    namedWindow("img");
    
    imshow("img", img);
    img *= 255;
    Mat result;
    img.convertTo(result, CV_8UC3);
    imwrite("../../result/bunny2.png", img);
}


