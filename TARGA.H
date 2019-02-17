#ifndef __TARGA__
#define __TARGA__

struct  TargaHeader
{
        char    TextSize;
        char    MapType;
        char    DataType;
        int     MapOrg;
        int     MapLength;
        char    CMapBits;
        int     XOffset;
        int     YOffset;
        int     Width;
        int     Height;
        char    DataBits;
        char    ImType;
};

#ifndef __RGB__
#define __RGB__
struct  RGB
{
        char    Red;
        char    Green;
        char    Blue;
};
#endif

class   TargaFile               // basic class for writing TGA image files
{
public:
        TargaFile ( char *, int, int, char * = "" );
        ~TargaFile ();

        void    PutPixel ( RGB );
private:
        TargaHeader     Hdr;
        RGB           * Buffer;
        int             BufSize;
        int             pos;
        int             file;

        void            Flush ();
};

#include "targa.cpp"

#endif
