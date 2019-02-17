#ifndef __WOOD__
#define __WOOD__

#include        "Tracer.h"
#include        "Noise.h"

class   Wood : public Texture
{
public:
        double  TurbScale;
        double  RingSpacing;
        int     Squeeze;

        Wood ( double r, double t, int s = 1 ) : Texture ()
        {
                TurbScale   = t;
                RingSpacing = r;
                Squeeze     = s;
        };

        virtual void    Apply ( Vector&, SurfaceData& );
};

#include "wood.cpp"

#endif