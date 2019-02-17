#include "Vector.h"
#include "Tracer.h"

void Checker ::Apply(Vector &p, SurfaceData &t)
{
        Vector r = p * Scale + Offs;
        int ix = (int)(r.x < 0 ? 1 - r.x : r.x);
        int iy = (int)(r.y < 0 ? 1 - r.y : r.y);
        int iz = (int)(r.z < 0 ? 1 - r.z : r.z);

        if ((ix + iy + iz) & 1)
                t.Color = Color2;
        else
                t.Color = Color1;
}
