#ifndef __RIPPLES__
#define __RIPPLES__

#include        "Tracer.h"

class   Ripples : public Texture
{
public:
        Vector  Center;
        double  WaveLength;
        double  Phase;
        double  Amount;

        Ripples ( Vector& c, double a, double l, double p = 0 ) : Texture ()
        {
                Center     = c;
                WaveLength = l;
                Phase      = p;
                Amount     = a;
        };

        virtual void    Apply ( Vector&, SurfaceData& );
};

#include "ripples.cpp"

#endif