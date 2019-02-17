#include "vector.h"
#include "tracer.h"
#include "render.h"
#include "targa.h"
#include "draw.h"
#include "geometry.h"
#include "noise.h"
#include "colortbl.h"
#include "colors.tbl"

#define SceneName "Marble and Granite Texture Test"
#define OutPutFile "marble.TGA"
#define SceneWidth 640
#define SceneHeight 480

class Marble : public Texture
{
      public:
        double TurbScale;
        int Squeeze;
        ColorTable Tbl;

        Marble(double t = 1, int s = 1) : Texture(), Tbl()
        {
                TurbScale = t;
                Squeeze = s;
        };

        virtual void Apply(Vector &, SurfaceData &);
};

class Granite : public Texture
{
      public:
        ColorTable Tbl;

        Granite() : Texture(), Tbl(){};

        virtual void Apply(Vector &, SurfaceData &);
};

void Marble ::Apply(Vector &p, SurfaceData &t)
{
        double x = p.x * Scale.x + Offs.x;
        double s = pow(SawWave(x + TurbScale * Turbulence(p, 4)), Squeeze);

        t.Color = Tbl.FindColor(s);
}

void Granite ::Apply(Vector &p, SurfaceData &t)
{
        double s = 0.5 * Turbulence(3 * p, 5);

        t.Color = Tbl.FindColor(s);
}

void main()
{
        cout << "\nRay Tracer v1.0 (C)1998 Vasily Kazantsev.";
        cout << "\nInit Scene... ";
        Sphere *s1, *s2, *s3, *s4;
        PointLight *Light1;
        Marble *m1 = new Marble;
        Marble *m2 = new Marble;
        Granite *g1 = new Granite;
        Granite *g2 = new Granite;

        Scene = new Environment();
        s1 = new Sphere(Vector(-2.2, 2.2, 10), 2);
        s2 = new Sphere(Vector(2.2, 2.2, 10), 2);
        s3 = new Sphere(Vector(-2.2, -2.2, 10), 2);
        s4 = new Sphere(Vector(2.2, -2.2, 10), 2);

        m1->Tbl.AddEntry(0.0, 0.8, Vector(0.9), Vector(0.5));
        m1->Tbl.AddEntry(0.8, 1.0, Vector(0.5), Vector(0.2));

        m2->Tbl.AddEntry(0.0, 0.8, Vector(0.8, 0.8, 0.6), Vector(0.8, 0.4, 0.4));
        m2->Tbl.AddEntry(0.8, 1.0, Vector(0.8, 0.4, 0.4), Vector(0.8, 0.2, 0.2));

        g1->Tbl.AddEntry(0.000, 0.178, Vector(0.831, 0.631, 0.569), Vector(0.925, 0.831, 0.714));
        g1->Tbl.AddEntry(0.178, 0.356, Vector(0.925, 0.831, 0.714), Vector(0.871, 0.702, 0.659));
        g1->Tbl.AddEntry(0.356, 0.525, Vector(0.871, 0.702, 0.659), Vector(0.831, 0.631, 0.569));
        g1->Tbl.AddEntry(0.525, 0.729, Vector(0.831, 0.631, 0.569), Vector(0.937, 0.882, 0.820));
        g1->Tbl.AddEntry(0.729, 1.000, Vector(0.937, 0.882, 0.820), Vector(0.831, 0.631, 0.569));

        g2->Tbl.AddEntry(0.000, 0.241, Vector(0.973, 0.973, 0.976), Vector(0.973, 0.973, 0.976));
        g2->Tbl.AddEntry(0.241, 0.284, Vector(0.973, 0.973, 0.976), Vector(0.600, 0.741, 0.608));
        g2->Tbl.AddEntry(0.284, 0.336, Vector(0.600, 0.741, 0.608), Vector(0.820, 0.643, 0.537));
        g2->Tbl.AddEntry(0.336, 0.474, Vector(0.820, 0.643, 0.537), Vector(0.886, 0.780, 0.714));
        g2->Tbl.AddEntry(0.474, 0.810, Vector(0.886, 0.780, 0.714), Vector(0.996, 0.643, 0.537));
        g2->Tbl.AddEntry(0.810, 0.836, Vector(0.996, 0.643, 0.537), Vector(0.973, 0.973, 0.976));
        g2->Tbl.AddEntry(0.836, 1.000, Vector(0.973, 0.973, 0.976), Vector(0.973, 0.973, 0.976));

        s1->DefMaterial.Ka = 0.3;
        s1->DefMaterial.Kd = 0.6;
        s1->DefMaterial.Ks = 0.7;
        s1->DefMaterial.Kr = 0.0;
        s1->DefMaterial.Kt = 0.0;
        s1->DefMaterial.p = 30;
        s1->DefMaterial.Color = Green;
        s1->DefMaterial.Med = Glass;
        s1->Add(m1);

        s2->DefMaterial = s1->DefMaterial;
        s2->Add(m2);

        s3->DefMaterial = s1->DefMaterial;
        s3->Add(g1);

        s4->DefMaterial = s1->DefMaterial;
        s4->Add(g2);

        Light1 = new PointLight(Vector(10, 5, -10), 17);

        Scene->Add(s1);
        Scene->Add(s2);
        Scene->Add(s3);
        Scene->Add(s4);
        Scene->Add(Light1);

        Background = DimGray;

        InitNoise();
        SetCamera(Vector(0), Vector(0, 0, 2), Vector(0, 1, 0));
        cout << "OK!";
        cout << "\nScene Name: " << SceneName;
        cout << "\nOutput file: " << OutPutFile << " [" << SceneWidth << "x" << SceneHeight << "].";
        DistributedRenderScene(1.5, 1.0, SceneWidth, SceneHeight, 4, 4, OutPutFile);
}
