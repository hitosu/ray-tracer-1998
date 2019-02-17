#include "vector.h"
#include "tracer.h"
#include "render.h"
#include "targa.h"
#include "draw.h"
#include "geometry.h"
#include "colors.tbl"

#define SceneName "Just A Ball"
#define OutPutFile "BALL.TGA"
//#define SceneWidth           80
//#define SceneHeight          60

void main()
{
        int r, g, b, l, bgr, bgg, bgb;
        int SceneWidth, SceneHeight;
        cout << "\nRay Tracer v1.0 (C)1998 Vasily Kazantsev.";
        cout << "\nBall Color [R,G,B in %]: ";
        cin >> r >> g >> b;
        cout << "Light Power: ";
        cin >> l;
        cout << "Background Color [R,G,B in %]: ";
        cin >> bgr >> bgg >> bgb;
        cout << "Image Size [X,Y]: ";
        cin >> SceneWidth >> SceneHeight;

        cout << "\nInit Scene... ";
        Sphere *ball;
        PointLight *Light1;

        Scene = new Environment();
        ball = new Sphere(Vector(0, 0, 5), 3.5);

        ball->DefMaterial.Ka = 0.25; //0.2
        ball->DefMaterial.Kd = 0.5;  //0.5
        ball->DefMaterial.Ks = 0.6;  //0.6
        ball->DefMaterial.Kr = 0.0;  //0.0
        ball->DefMaterial.Kt = 0.0;  //0.0
        ball->DefMaterial.p = 30;    //30
        ball->DefMaterial.Color = Vector((float)r / 100, (float)g / 100, (float)b / 100);
        Medium BallMedium = Glass;
        ball->DefMaterial.Med = BallMedium;
        //17
        Light1 = new PointLight(Vector(-15, 10, -15), l);

        Scene->Add(ball);
        Scene->Add(Light1);

        Background = Vector((float)bgr / 100, (float)bgg / 100, (float)bgb / 100);
        ;
        cout << "OK!";
        cout << "\nScene Name: " << SceneName;
        cout << "\nOutput file: " << OutPutFile << " [" << SceneWidth << "x" << SceneHeight << "].";
        SetCamera(Vector(0, 0, 0.5), Vector(0, 0, 1), Vector(0, 1, 0));
        DistributedRenderScene(2.0, 1.5, SceneWidth, SceneHeight, 3, 3, OutPutFile);
}