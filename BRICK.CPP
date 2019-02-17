#include "Vector.h"
#include "Tracer.h"

void Brick ::Apply(Vector &p, SurfaceData &t)
{
        Vector r = (p * Scale + Offs) / BrickSize;
        double bx, by, bz;

        if (Mod(r.y, 1) <= MortarSize.y)
        {
                t.Color = MortarColor;
                return;
        }

        by = Mod(0.5 * r.y, 1);
        if ((bx = Mod(r.x, 1)) <= MortarSize.x && by <= 0.5)
        {
                t.Color = MortarColor;
                return;
        }

        if ((bx += 0.5) >= 1.0)
                bx -= 1;

        if (bx <= MortarSize.x && by > 0.5)
        {
                t.Color = MortarColor;
                return;
        }

        if ((bz = Mod(r.z, 1)) <= MortarSize.z && by > 0.5)
        {
                t.Color = MortarColor;
                return;
        }

        if ((bz += 0.5) >= 1.0)
                bz -= 1;

        if (bz <= MortarSize.z && by <= 0.5)
        {
                t.Color = MortarColor;
                return;
        }

        t.Color = BrickColor;
}
