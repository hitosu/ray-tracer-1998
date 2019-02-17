
#ifndef __GEOMETRY__
#define __GEOMETRY__

#include        "Vector.h"
#include        "Tracer.h"

#define EPS     0.01

class   Sphere : public GObject
{
public:
        Vector          Loc;                            // center
        double          Radius;
        double          Radius2;                        // squared radius

        Sphere ( Vector& c, double r ) { Loc = c; Radius = r; Radius2 = r*r; };

        virtual int     Intersect ( Ray&, double& );
        virtual Vector  FindNormal ( Vector& );
};

class   Plane : public GObject
{
public:                                         // Plane Eq. (n,r) + D = 0
        Vector  n;                              // unit plane normal
        double  D;                              // distance from origin

        Plane ( Vector& normal, double dist ) { n = normal; D = dist; };
        Plane ( double, double, double, double );       // ax + by + cz + d = 0

        virtual int     Intersect ( Ray&, double& );
        virtual Vector  FindNormal ( Vector& ) { return n; };
};

class   Rect : public GObject
{
public:
        Vector  Loc;
        Vector  Side1, Side2;
        Vector  n;
        Vector  ku, kv;
        double  u0, v0;

        Rect ( Vector&, Vector&, Vector& );

        virtual int     Intersect ( Ray&, double& );
        virtual Vector  FindNormal ( Vector& ) { return n; };
};

class   Triangle : public Rect
{
public:
        Triangle ( Vector& l, Vector& s1, Vector& s2 ) : Rect ( l, s1, s2 ) {};

        virtual int     Intersect ( Ray&, double& );
};

class   Box : public GObject
{
        Vector  n [3];                  // normals no sides, sides are : ( p, n ) + d = 0;
        double  d1 [3], d2 [3];         // dist, for plane eq., d1 [i] < d2 [i]
        Vector  Center;                 // center of
public:
        Vector  Loc;                    // origin
        Vector  e1, e2, e3;             // main edges

        Box ( Vector&, Vector&, Vector&, Vector& );
        Box ( Vector&, double, double, double );

        virtual int     Intersect ( Ray&, double& );
        virtual Vector  FindNormal ( Vector& );

private:
        void            InitNormals ();
};

class   Cylinder : public GObject
{
        Vector  e1, e2;
        double  d1, d2;                         // parameters of edges
        double  Len;                            // length of cylinder
        double  Len2;                           // squared length ( vector Dir squared )
        double  Radius2;
        double  Radius4;
public:
        Vector  Loc;
        Vector  Dir;
        double  Radius;

        Cylinder ( Vector&, Vector&, double );

        virtual int     Intersect ( Ray&, double& );
        virtual Vector  FindNormal ( Vector& );
};

///////////////////////////   Lights   ///////////////////////////

class   PointLight : public LightSource
{
public:
        Vector  Loc;
        double  DistScale;

        PointLight ( Vector& l, double d = 1.0 ) : LightSource () { Loc = l; DistScale = d; };

        virtual double  Shadow ( Vector&, Vector& );
};

class   SphericLight : public LightSource
{
public:
        Vector  Loc;
        double  Radius;
        double  DistScale;

        SphericLight ( Vector& l, double r, double d = 1.0 ) : LightSource () { Loc = l; Radius = r; DistScale = d; };

        virtual double  Shadow ( Vector&, Vector& );
};

class   SpotLight : public LightSource
{
public:
        Vector  Loc;
        Vector  Dir;
        double  ConeAngle, EndConeAngle;        // cosines of main angle and fall-off angle
        int     BeamDistribution;
        double  DistScale;

        SpotLight ( Vector& l, Vector& d, double a, double da, int bd, double dscale = 1.0 ) : LightSource ()
        {
                Loc              = l;
                Dir              = d;
                ConeAngle        = a;
                EndConeAngle     = da;
                BeamDistribution = bd;
                DistScale        = dscale;
        };

        virtual double  Shadow ( Vector&, Vector& );
};

/////////////////////////////////////////////////////////////////////////////

extern  double  GeomThreshold;                  // min. ray length accounted for
                                                // if ray length to intersection point is
                                                // lesser than this value, assume NO INTERSECTION
#include "geometry.cpp"

#endif
