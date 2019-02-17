
#ifndef         __MAP__
#define         __MAP__

#include        "Tracer.h"
#include        "Bmp.h"

class   ColorMap : public Texture
{
public:
        Image   * Img;

        ColorMap ( Image * i ) : Texture () { Img = i; };
        ~ColorMap () { delete Img; };

        virtual void    Apply ( Vector&, SurfaceData& );
};

class   BumpMap : public Texture
{
public:
        Image   * Img;
        double    Amount;

        BumpMap ( Image * i, double a ) : Texture () { Img = i; Amount = a; };
        ~BumpMap () { delete Img; };

        virtual void    Apply ( Vector&, SurfaceData& );
};
#include "map.cpp"

#endif