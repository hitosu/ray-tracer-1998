#ifndef __NOISE__
#define __NOISE__
#include        <math.h>
#include        "Vector.h"

#define NOISE_DIM       15

void    InitNoise ();
double  Noise ( const Vector& );
Vector  Noise3d ( const Vector& );
double  Turbulence ( const Vector&, int );
Vector  Turbulence3d ( const Vector&, int );

#include "noise.cpp"

#endif
