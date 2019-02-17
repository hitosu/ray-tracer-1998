#include "vector.h"
#include "tracer.h"
#include "render.h"
#include "targa.h"
#include "draw.h"
#include "geometry.h"
#include "checker.h"
#include "colors.tbl"

#define SceneName "Spheres on the Checkers Table Test"
#define OutPutFile "chsphere.TGA"
#define SceneWidth 640
#define SceneHeight 480

extern unsigned _stklen = 20480;

void main()
{
        cout << "\nRay Tracer v1.0 (C)1998 Vasily Kazantsev.";
        cout << "\nInit Scene... ";
        Rect *r = new Rect(Vector(0.1), Vector(40, 0, 0), Vector(0, 40, 0));
        Sphere *s1 = new Sphere(Vector(3, 20, 3.5), 2.75);
        Sphere *s2 = new Sphere(Vector(7.9, 7.9, 3.5), 2.75);
        Sphere *s3 = new Sphere(Vector(20, 3, 3.5), 2.75);
        Sphere *s4 = new Sphere(Vector(32, 7.9, 3.5), 2.75);
        Sphere *s5 = new Sphere(Vector(37, 20, 3.5), 2.75);
        Sphere *s6 = new Sphere(Vector(32, 32, 3.5), 2.75);
        Sphere *s7 = new Sphere(Vector(20, 37, 3.5), 2.75);
        Sphere *s8 = new Sphere(Vector(7.9, 32, 3.5), 2.75);

        PointLight *Light1 = new PointLight(Vector(65, 58, 20), 15);

        Scene = new Environment();

        r->DefMaterial.Ka = 0.3;
        r->DefMaterial.Kd = 0.9;
        r->DefMaterial.Ks = 0.0;
        r->DefMaterial.Kr = 0.8;
        r->DefMaterial.Kt = 0.0;
        r->DefMaterial.p = 5;
        r->DefMaterial.Med = Glass;
        r->DefMaterial.Color = White;
        r->Add(new Checker(Red, White));

        Scene->Add(r);
        Scene->Add(Light1);

        s1->DefMaterial.Ka = 0.3;
        s1->DefMaterial.Kd = 0.0;
        s1->DefMaterial.Ks = 0.9;
        s1->DefMaterial.Kr = 0.8;
        s1->DefMaterial.Kt = 0.0;
        s1->DefMaterial.p = 30;
        s1->DefMaterial.Med = Glass;
        s1->DefMaterial.Color = White;

        Scene->Add(s1);

        s2->DefMaterial = s1->DefMaterial;
        s3->DefMaterial = s1->DefMaterial;
        s4->DefMaterial = s1->DefMaterial;
        s5->DefMaterial = s1->DefMaterial;
        s6->DefMaterial = s1->DefMaterial;
        s7->DefMaterial = s1->DefMaterial;
        s8->DefMaterial = s1->DefMaterial;

        Scene->Add(s2);
        Scene->Add(s3);
        Scene->Add(s4);
        Scene->Add(s5);
        Scene->Add(s6);
        Scene->Add(s7);
        Scene->Add(s8);

        Background = Black;
        SetCamera(Vector(65, 58, 20), Vector(-0.45, -0.45, -0.2), Vector(0, 0, 1));
        cout << "OK!";
        cout << "\nScene Name: " << SceneName;
        cout << "\nOutput file: " << OutPutFile << " [" << SceneWidth << "x" << SceneHeight << "].";
        DistributedRenderScene(0.3, 0.2, SceneWidth, SceneHeight, 4, 4, OutPutFile);
}
