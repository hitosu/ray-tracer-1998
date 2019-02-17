#include "Tracer.h"
#include "Bmp.h"

void ColorMap ::Apply(Vector &p, SurfaceData &t)
{
        double x = Mod(Offs.x + Scale.x * t.MapCoord.x, Img->Width);
        double y = Mod(Offs.y + Scale.y * t.MapCoord.y, Img->Height);
        int ix = (int)x;
        int iy = (int)y;
        int jx = ix + 1;
        int jy = iy + 1;

        x -= ix;
        y -= iy;

        if (jx >= Img->Width) // wrap around
                jx = 0;

        if (jy >= Img->Height)
                jy = 0;
        // interpolate between corners
        RGB c00 = Img->GetPixel(ix, iy);
        RGB c01 = Img->GetPixel(ix, jy);
        RGB c10 = Img->GetPixel(jx, iy);
        RGB c11 = Img->GetPixel(jx, jy);

        t.Color.x = ((1 - x) * (1 - y) * c00.Red + (1 - x) * y * c01.Red + x * (1 - y) * c10.Red + x * y * c11.Red) / 255;
        t.Color.y = ((1 - x) * (1 - y) * c00.Green + (1 - x) * y * c01.Green + x * (1 - y) * c10.Green + x * y * c11.Green) / 255;
        t.Color.z = ((1 - x) * (1 - y) * c00.Blue + (1 - x) * y * c01.Blue + x * (1 - y) * c10.Blue + x * y * c11.Blue) / 255;
}

void BumpMap ::Apply(Vector &p, SurfaceData &t)
{
        double x = Mod(Offs.x + Scale.x * p.x, Img->Width);
        double y = Mod(Offs.y + Scale.y * p.y, Img->Height);
        int ix = (int)x;
        int iy = (int)y;
        int jx = ix + 1;
        int jy = iy + 1;

        x -= ix;
        y -= iy;

        if (jx >= Img->Width) // wrap around
                jx = 0;

        if (jy >= Img->Height)
                jy = 0;
        // interpolate between corners
        RGB c00 = Img->GetPixel(ix, iy);
        RGB c01 = Img->GetPixel(ix, jy);
        RGB c10 = Img->GetPixel(jx, iy);
        double i00 = (0.229 * c00.Red + 0.587 * c00.Green + 0.114 * c00.Blue) / 255;
        double i01 = (0.229 * c01.Red + 0.587 * c01.Green + 0.114 * c01.Blue) / 255;
        double i10 = (0.229 * c10.Red + 0.587 * c10.Green + 0.114 * c10.Blue) / 255;
        double du = (i10 - i00) * Amount;
        double dv = (i01 - i00) * Amount;
        Vector tu, tv;

        if (object->Mapping != NULL)
                object->Mapping->FindTangent(p, tu, tv);

        t.n += du * (t.n ^ tv) - dv * (t.n ^ tu);
        t.n = Normalize(t.n);
}
