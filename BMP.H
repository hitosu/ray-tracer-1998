#ifndef __BMP__
#define __BMP__

#ifndef __RGB__
#define __RGB__
struct  RGB
{
        char    Red;
        char    Green;
        char    Blue;
};
#endif

class   Image
{
public:
        int     Width, Height;

        virtual ~Image () {};

        virtual RGB     GetPixel ( int, int ) = 0;
};

class   BMPImage : public Image
{
public:
        RGB     * Palette;
        char    * Data;

        BMPImage ( char * );
        ~BMPImage ();

        virtual RGB     GetPixel ( int, int );
};

#include "bmp.cpp"

#endif