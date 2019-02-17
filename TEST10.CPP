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

#define SceneName "Bitmap Relief Texture Test"
#define OutPutFile "relief.TGA"
#define SceneWidth 640
#define SceneHeight 480

void main()
{
        cout << "\nRay Tracer v1.0 (C)1998 Vasily Kazantsev.";
        cout << "\nInit Scene... ";
        Rect *b = new Rect(Vector(-1, 1, 1.5), Vector(2, 0, 0), Vector(0, -2, 0));
        PointLight *Light1 = new PointLight(Vector(7, 10, -10), 20);
        BMPImage *img = new BMPImage("pic.bmp");
        BumpMap *bmap = new BumpMap(img, 1);

        bmap->Scale = Vector(63, -63, 0);
        bmap->Offs = Vector(63, 63, 0);

        Scene = new Environment();

        b->Mapping = new PlaneMap(Vector(0, 0, 1), Vector(1, 0, 0));
        b->Add(bmap);
        b->DefMaterial.Ka = 0.3;
        b->DefMaterial.Kd = 0.8;
        b->DefMaterial.Ks = 0.3;
        b->DefMaterial.Kr = 0.0;
        b->DefMaterial.Kt = 0.0;
        b->DefMaterial.p = 5;
        b->DefMaterial.Med = Glass;
        b->DefMaterial.Color = Blue;

        Scene->Add(b);
        Scene->Add(Light1);

        Background = SkyBlue;
        SetCamera(Vector(0), Vector(0, 0, 1), Vector(0, 1, 0));
        cout << "OK!";
        cout << "\nScene Name: " << SceneName;
        cout << "\nOutput file: " << OutPutFile << " [" << SceneWidth << "x" << SceneHeight << "].";
        DistributedRenderScene(1.5, 1.0, SceneWidth, SceneHeight, 4, 4, OutPutFile);
}
