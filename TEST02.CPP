#include "vector.h"
#include "tracer.h"
#include "render.h"
#include "targa.h"
#include "draw.h"
#include "geometry.h"
#include "colors.tbl"

#define SceneName "Reflection Test"
#define OutPutFile "reflect.TGA"
#define SceneWidth 640
#define SceneHeight 480

void main()
{
        cout << "\nRay Tracer v1.0 (C)1998 Vasily Kazantsev.";
        cout << "\nInit Scene... ";
        Sphere *s1, *s2, *s3;
        Plane *p;
        PointLight *Light1;

        Scene = new Environment();
        s1 = new Sphere(Vector(0, 1, 5), 1.5);
        s2 = new Sphere(Vector(-3, 0, 4), 1);
        s3 = new Sphere(Vector(3, 0, 4), 1);
        p = new Plane(Vector(0, 1, 0), 1);

        s1->DefMaterial.Ka = 0.2;
        s1->DefMaterial.Kd = 0.5;
        s1->DefMaterial.Ks = 0.6;
        s1->DefMaterial.Kr = 0.0;
        s1->DefMaterial.Kt = 0.0; //0.0
        s1->DefMaterial.p = 30;
        s1->DefMaterial.Color = Yellow;
        s1->DefMaterial.Med = Glass;

        s2->DefMaterial = s1->DefMaterial;
        s2->DefMaterial.Color = Red;

        s3->DefMaterial = s1->DefMaterial;
        s3->DefMaterial.Color = Green;

        p->DefMaterial = s1->DefMaterial;
        p->DefMaterial.Ka = 0.1;
        p->DefMaterial.Ks = 0.4; //0.4
        p->DefMaterial.Kd = 0.5; //0.5
        p->DefMaterial.Kr = 0.4; //0.4
        p->DefMaterial.Color = Blue;

        s1->DefMaterial.Kr = 0.3;

        Light1 = new PointLight(Vector(10, 5, -10), 17);

        Scene->Add(s1);
        Scene->Add(s2);
        Scene->Add(s3);
        Scene->Add(p);
        Scene->Add(Light1);

        Background = DimGray;
        cout << "OK!";
        cout << "\nScene Name: " << SceneName;
        cout << "\nOutput file: " << OutPutFile << " [" << SceneWidth << "x" << SceneHeight << "].";
        SetCamera(Vector(0, 0, 0.5), Vector(0, 0, 1), Vector(0, 1, 0));
        DistributedRenderScene(2.0, 1.5, SceneWidth, SceneHeight, 4, 4, OutPutFile);
}