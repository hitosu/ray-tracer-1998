#ifndef __PLANEMAP__
#define __PLANEMAP__

#include        "Tracer.h"

class   PlaneMap : public Map
{
public:
        Vector  eu, ev;

        PlaneMap ( Vector& n, Vector& e1 )
        {
                eu = e1 - n * ( n & e1 ) / ( n & n );
                ev = n ^ e1;
        };

        virtual Vector  Apply ( Vector& p ) { return Vector ( p & eu, p & ev, 0 ); };
        virtual void    FindTangent ( Vector& p, Vector& tu, Vector& tv ){ tu = eu; tv = ev; };
};

#endif