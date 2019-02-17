#include        <dos.h>
#include        "vector.h"

#ifndef __DRAW__
#define __DRAW__

#ifndef __RGB__
#define __RGB__
struct  RGB
{
        char    Red;
        char    Green;
        char    Blue;
};
#endif

void    SetMode ( int );
void    SetPalette ( RGB far * );
void    SetPreviewPalette ();
void    DrawPixel ( int, int, Vector& );
void    BuildImagePalette ( char far *, RGB * );
void    DrawImageFile ( char * );
void    DrawTargaFile ( char * );

#include "draw.cpp"

#endif
