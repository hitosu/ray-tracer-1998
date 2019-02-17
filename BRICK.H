#ifndef __BRICK__
#define __BRICK__

#include        "Tracer.h"

class   Brick : public Texture
{
public:
        Vector  BrickSize;
        Vector  MortarSize;
        Vector  BrickColor, MortarColor;

        Brick ( Vector& bs, Vector& ms, Vector& bc, Vector& mc ) : Texture ()
        {
                BrickSize  = bs;        MortarSize  = ms / bs;
                BrickColor = bc;        MortarColor = mc;
        };

        virtual void    Apply ( Vector&, SurfaceData& );
};

#include "brick.cpp"

#endif