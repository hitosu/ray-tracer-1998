#include "Colors.tbl"

void Wood ::Apply(Vector &p, SurfaceData &t)
{
        double x = p.x * Scale.x + Offs.x;
        double y = p.y * Scale.y + Offs.y;
        double s = pow(SineWave(RingSpacing * sqrt(x * x + y * y) + TurbScale * Turbulence(p, 3)), Squeeze);

        t.Color = (1 - s) * LightWood + s * DarkWood;
        t.Ks *= 0.3 * s + 0.7;
}
