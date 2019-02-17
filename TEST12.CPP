#include "vector.h"
#include "tracer.h"
#include "render.h"
#include "targa.h"
#include "draw.h"
#include "geometry.h"
#include "checker.h"
#include "colors.tbl"

#define SceneName "Checkers Texture Test"
#define OutPutFile "checker.TGA"
#define SceneWidth 640
#define SceneHeight 480

void main()
{
        cout << "\nRay Tracer v1.0 (C)1998 Vasily Kazantsev.";
        cout << "\nInit Scene... ";
        Plane *p;
        Sphere *s[16];
        PointLight *Light1;
        int i, j, k;

        Scene = new Environment();

        p = new Plane(0, 0, 1, -7.1);

        p->DefMaterial.Ka = 0.8;
        p->DefMaterial.Kd = 0.2;
        p->DefMaterial.Ks = 0.1;
        p->DefMaterial.Kr = 0.0;
        p->DefMaterial.Kt = 0.0;
        p->DefMaterial.p = 30;
        p->DefMaterial.Color = White;
        p->DefMaterial.Med = Glass;

        p->Add(new Checker(White, Black));

        p->Material->Scale = 3;

        Scene->Add(p);

        for (i = k = 0; i < 4; i++)
                for (j = 0; j < 4; j++, k++)
                {
                        s[k] = new Sphere(Vector(-3 + j * 2, 2.15 - i * 1.45, 5), 0.7);

                        if (i > 0)
                                s[k]->DefMaterial.Ka = 0.2;
                        else
                                s[k]->DefMaterial.Ka = j * 0.33;

                        if (i < 1)
                                s[k]->DefMaterial.Kd = 0;
                        else if (i == 1)
                                s[k]->DefMaterial.Kd = j * 0.33;
                        else
                                s[k]->DefMaterial.Kd = 0.4;

                        if (i < 2)
                                s[k]->DefMaterial.Ks = 0;
                        else if (i == 2)
                                s[k]->DefMaterial.Ks = j * 0.33;
                        else
                                s[k]->DefMaterial.Ks = 0.7;

                        if (i < 3)
                        {
                                s[k]->DefMaterial.Med.nRefr = 1.0;
                                s[k]->DefMaterial.Kt = 0.0;
                        }
                        else
                        {
                                s[k]->DefMaterial.Med.nRefr = 1.0 + j * 0.05;
                                s[k]->DefMaterial.Kt = 0.8;
                        }

                        s[k]->DefMaterial.p = 20;
                        s[k]->DefMaterial.Kr = 0;
                        s[k]->DefMaterial.Color = Green;
                        s[k]->DefMaterial.Med.Betta = 0;

                        Scene->Add(s[k]);
                }

        Light1 = new PointLight(Vector(10, 5, -10), 15);

        Scene->Add(Light1);

        Background = SkyBlue;
        SetCamera(Vector(0, 0, -10), Vector(0, 0, 1), Vector(0, 1, 0));
        cout << "OK!";
        cout << "\nScene Name: " << SceneName;
        cout << "\nOutput file: " << OutPutFile << " [" << SceneWidth << "x" << SceneHeight << "].";
        DistributedRenderScene(0.3, 0.2, SceneWidth, SceneHeight, 4, 4, OutPutFile);
}
