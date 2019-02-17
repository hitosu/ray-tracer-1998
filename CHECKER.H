#ifndef __CHECKER__
#define __CHECKER__

#include        "Tracer.h"

class   Checker : public Texture
{
public:
        Vector  Color1, Color2;

        Checker ( Vector& c1, Vector& c2 ) : Texture () { Color1 = c1; Color2 = c2; };

        virtual void    Apply ( Vector&, SurfaceData& );
};

#include "checker.cpp"

#endif