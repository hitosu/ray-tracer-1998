#include "vector.h"
#include "tracer.h"
#include "render.h"
#include "targa.h"
#include "draw.h"
#include "geometry.h"
#include "bmp.h"
#include "map.h"
#include "planemap.h"
#include "colors.tbl"

#define SceneName "Spherical Texture Test"
#define OutPutFile "sphere.TGA"
#define SceneWidth 640
#define SceneHeight 480

class CubicMap : public Map
{
        Vector Size;

      public:
        CubicMap(const Vector &s) { Size = s; }

        virtual Vector Apply(Vector &);
        virtual void FindTangent(Vector &, Vector &, Vector &);
};

class SphericalMap : public Map
{
        Vector Center;

      public:
        SphericalMap(const Vector &c) { Center = c; }

        virtual Vector Apply(Vector &);
        virtual void FindTangent(Vector &, Vector &, Vector &);
};

////////////////////////////////////////////////////////////

Vector CubicMap ::Apply(Vector &p)
{
        Vector v;

        if (fabs(p.x) >= fabs(p.y))
                if (fabs(p.x) >= fabs(p.z))
                {
                        v.x = p.y / p.x; // x = max p
                        v.y = p.z / p.x;
                }
                else
                {
                        v.x = p.x / p.z; // z = max p
                        v.y = p.y / p.z;
                }
        else if (fabs(p.y) >= fabs(p.z))
        {
                v.x = p.x / p.y; // y = max p
                v.y = p.z / p.y;
        }
        else
        {
                v.x = p.x / p.z; // z = max p
                v.y = p.y / p.z;
        }

        v.x += 1;
        v.y += 1;
        v.z = 0;

        return v * 0.5 * Size;
}

void CubicMap ::FindTangent(Vector &p, Vector &tu, Vector &tv)
{
        if (fabs(p.x) >= fabs(p.y))
                if (fabs(p.x) >= fabs(p.z))
                {
                        tu = Vector(0, 1, 0);
                        tv = Vector(0, 0, 1);
                }
                else
                {
                        tu = Vector(1, 0, 0);
                        tv = Vector(0, 1, 0);
                }
        else if (fabs(p.y) >= fabs(p.z))
        {
                tu = Vector(1, 0, 0);
                tv = Vector(0, 0, 1);
        }
        else
        {
                tu = Vector(1, 0, 0);
                tv = Vector(0, 1, 0);
        }
}

Vector SphericalMap ::Apply(Vector &p)
{
        Vector v = Normalize(p - Center);
        double phi = acos(v.y) / M_PI;
        double theta = v.x / sqrt(1 - v.y * v.y);

        if (fabs(theta) > 1)
                return Vector(0, phi, 0);

        theta = acos(theta) / (2 * M_PI);

        if (v.z > 0)
                return Vector(theta, phi, 0);
        else
                return Vector(1 - theta, phi, 0);
}

void SphericalMap ::FindTangent(Vector &p, Vector &tu, Vector &tv)
{
        Vector v = p - Center;

        tv = Normalize(Vector(-v.x, -v.y, 0));
        tu = Normalize(v ^ tv);
}

void main()
{
        cout << "\nRay Tracer v1.0 (C)1998 Vasily Kazantsev.";
        cout << "\nInit Scene... ";
        Sphere *b = new Sphere(Vector(0), 3);
        PointLight *Light1 = new PointLight(Vector(7, 10, -10), 20);
        BMPImage *img = new BMPImage("pic.bmp");
        ColorMap *bmap = new ColorMap(img);

        Scene = new Environment();

        b->Mapping = new CubicMap(Vector(img->Width, img->Height, 0));
        b->Add(bmap);
        b->DefMaterial.Ka = 0.4;
        b->DefMaterial.Kd = 0.5;
        b->DefMaterial.Ks = 0.2;
        b->DefMaterial.Kr = 0.0;
        b->DefMaterial.Kt = 0.0;
        b->DefMaterial.p = 5;
        b->DefMaterial.Med = Glass;
        b->DefMaterial.Color = Blue;

        Scene->Add(b);
        Scene->Add(Light1);

        Background = SkyBlue;
        SetCamera(Vector(-1, 2, -5), Vector(0.2, -0.4, 1), Vector(0, 1, 0));
        cout << "OK!";
        cout << "\nScene Name: " << SceneName;
        cout << "\nOutput file: " << OutPutFile << " [" << SceneWidth << "x" << SceneHeight << "].";
        DistributedRenderScene(1.5, 1.0, SceneWidth, SceneHeight, 4, 4, OutPutFile);
}
