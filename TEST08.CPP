#include "vector.h"
#include "tracer.h"
#include "render.h"
#include "targa.h"
#include "draw.h"
#include "geometry.h"
#include "noise.h"
#include "colors.tbl"

#define SceneName "Bumpy Texture Test"
#define OutPutFile "bumpy.TGA"
#define SceneWidth 640
#define SceneHeight 480

class BumpyTexture : public Texture
{
      public:
        BumpyTexture() : Texture(){};

        virtual void Apply(Vector &, SurfaceData &);
};

void BumpyTexture ::Apply(Vector &p, SurfaceData &t)
{
        t.n += 2 * (Noise3d(2 * p) - Vector(0.5));
        t.n = Normalize(t.n);
}

void main()
{
        cout << "\nRay Tracer v1.0 (C)1998 Vasily Kazantsev.";
        cout << "\nInit Scene... ";
        Sphere *s1;
        PointLight *Light1, *Light2;

        Scene = new Environment();
        s1 = new Sphere(Vector(0, 0, 0), 4);

        s1->DefMaterial.Ka = 0.3;
        s1->DefMaterial.Kd = 0.2;
        s1->DefMaterial.Ks = 0.7;
        s1->DefMaterial.Kr = 0.0;
        s1->DefMaterial.Kt = 0.0;
        s1->DefMaterial.p = 30;
        s1->DefMaterial.Color = Red;
        s1->DefMaterial.Med = Glass;
        s1->Add(new BumpyTexture);

        Light1 = new PointLight(Vector(-10, 8, -20), 20);
        Light2 = new PointLight(Vector(10, 8, -20), 20);

        Scene->Add(s1);
        Scene->Add(Light1);
        Scene->Add(Light2);

        Background = SkyBlue;

        InitNoise();
        SetCamera(Vector(0, 0, -7), Vector(0, 0, 1), Vector(0, 1, 0));
        cout << "OK!";
        cout << "\nScene Name: " << SceneName;
        cout << "\nOutput file: " << OutPutFile << " [" << SceneWidth << "x" << SceneHeight << "].";
        DistributedRenderScene(1.5, 1.0, SceneWidth, SceneHeight, 4, 4, OutPutFile);
}
