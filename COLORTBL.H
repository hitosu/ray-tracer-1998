#ifndef __COLOR_TABLE__
#define __COLOR_TABLE__

#include        "Vector.h"

struct  ColorTableEntry
{
        double  ta, tb;
        Vector  ca, cb;
};

class   ColorTable
{
        int               ColorEntries;
        int               MaxEntries;
        ColorTableEntry * Entries;
public:

        ColorTable ( int = 10 );
        ~ColorTable () { delete Entries; };

        void    AddEntry ( double, double, Vector, Vector );
        Vector  FindColor ( double );
};
#include "colortbl.cpp"

#endif
