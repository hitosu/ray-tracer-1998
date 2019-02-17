#include "vector.h"
#include "tracer.h"
#include "render.h"
#include "targa.h"
#include "draw.h"
#include "geometry.h"
#include "noise.h"
#include "wood.h"
#include "colors.tbl"

#define SceneName "Wood Texture Test"
#define OutPutFile "wood.TGA"
#define SceneWidth 640
#define SceneHeight 480

void main()
{
        cout << "\nRay Tracer v1.0 (C)1998 Vasily Kazantsev.";
        cout << "\nInit Scene... ";
        Box *b = new Box(Vector(-1, -1, -2), Vector(2, 0, 0), Vector(0, 2, 0), Vector(0, 0, 4));
        PointLight *Light1, *Light2;

        Scene = new Environment();

        b->DefMaterial.Ka = 0.3;
        b->DefMaterial.Kd = 0.7;
        b->DefMaterial.Ks = 0.5;
        b->DefMaterial.Kr = 0.0;
        b->DefMaterial.Kt = 0.0;
        b->DefMaterial.p = 30;
        b->DefMaterial.Color = Yellow;
        b->DefMaterial.Med = Glass;
        b->Add(new Wood(35, 6, 5));

        Light1 = new PointLight(Vector(10, 5, -10), 17);
        Light2 = new PointLight(Vector(-10, -5, -10), 17);

        Scene->Add(b);
        Scene->Add(Light1);
        Scene->Add(Light2);

        Background = DimGray;

        InitNoise();
        SetCamera(Vector(-4, 8, -4), Vector(2, -5, 2), Vector(0, 1, 0));
        cout << "OK!";
        cout << "\nScene Name: " << SceneName;
        cout << "\nOutput file: " << OutPutFile << " [" << SceneWidth << "x" << SceneHeight << "].";
        DistributedRenderScene(1.5, 1.0, SceneWidth, SceneHeight, 4, 4, OutPutFile);
}