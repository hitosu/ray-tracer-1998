#ifndef __TRACER__
#define __TRACER__

#include        <math.h>
#include        <stdlib.h>
#include        "Vector.h"

#define MAX_LIGHTS      10
#define MAX_SOLIDS      100
#define INFINITY        30000

struct  Medium                          // main properties of the medium
{
        double  nRefr;                  // refraction coefficient
        double  Betta;                  // attenuation coefficient
};

struct  SurfaceData                     // surface charactericstics at a given point
{
        double  Ka;                     // ambient light coefficient
        double  Kd;                     // diffuse light coefficient
        double  Ks;                     // specular light coefficient
        double  Kr;                     // reflected ray coefficient
        double  Kt;                     // transparent light coefficient
        Vector  Color;                  // object's color
        Medium  Med;                    // medium of the object
        int     p;                      // Phong's coeff.
        Vector  n;                      // normal at a given point
        Vector  MapCoord;               // mapping coordinates
};

class   LightSource                     // model of an abstract light source
{
public:
        Vector  Color;

        LightSource () { Color = 1; };
        virtual ~LightSource () {};     // force virtual destructor in all descendants

        virtual double  Shadow ( Vector&, Vector& ) = 0;
};

class   GObject;                        // class predeclaration

class   Texture                                 // generic texture class
{
public:
        Texture * next;
        GObject * object;
        Vector    Offs;
        Vector    Scale;

        Texture () { next = NULL; object = NULL; Offs = 0; Scale = 1; };
        virtual ~Texture () {};

        virtual void    Apply ( Vector& p, SurfaceData& t ) = 0;
};

class   Map                                     // generic mapping class
{
public:
        virtual ~Map () {};                     // force virtual destructor

        virtual Vector  Apply ( Vector& ) = 0;  // map point
        virtual void    FindTangent ( Vector&, Vector&, Vector& ) = 0;
};

class   GObject                         // model of an abstract geometric object
{
public:
        SurfaceData   DefMaterial;      // default material ( if no texture specified )
        Map         * Mapping;
        Texture     * Material;

        GObject () { Mapping = NULL; Material = NULL; };
        virtual ~GObject ();

        void            FindTexture ( Vector& p, SurfaceData& t );
        void            Add ( Texture * );
        virtual int     Intersect ( Ray&, double& ) = 0;
        virtual Vector  FindNormal ( Vector& ) = 0;
};

class   Environment                     // simplest model of environment
{
public:
        LightSource * Light [MAX_LIGHTS];
        int           LightsCount;
        GObject     * Solid [MAX_SOLIDS];
        int           SolidsCount;

        Environment () { LightsCount = SolidsCount = 0; };
        ~Environment ();

        void              Add ( LightSource * );
        void              Add ( GObject * );
        virtual GObject * Intersect ( Ray&, double& );
        virtual Vector    ShadeBackground ( Ray& );
};

/////////////////////////////  Globals  ////////////////////////////////////

extern  Vector          Eye;                            // camera position
extern  Vector          EyeDir;                         // camera viewing direction
extern  Vector          Vx, Vy;                         // image plane basis ( Vx - hor, Vy - vert )
extern  Medium          Air;                            // basic materials
extern  Medium          Glass;
extern  int             Level;                          // current recusion level
extern  double          Threshold;
extern  int             MaxLevel;                       // max. levels of recursion
extern  Vector          Ambient;                        // ambient light intensity
extern  Vector          Background;
extern  Environment   * Scene;
extern  long            TotalRays;

/////////////////////////// Function definitions /////////////////////////

void    Camera ( double, double, Ray& );                                // get ray for given sample point
void    SetCamera ( Vector&, Vector&, Vector& );                // set new camera parameters
Vector  Trace  ( Medium&, double, Ray& );                       // trace a ray in given medium, ret. ray length
Vector  Shade  ( Medium&, double, Vector&, Vector&, GObject * ); // compute light at given point
double  SawWave ( double );

inline  double  SineWave ( double x )
{
        return 0.5 * ( 1.0 + sin ( x ) );
}

inline  double  Mod ( double x, double y )
{
        if ( ( x = fmod ( x, y ) ) < 0 )
                return x + y;
        else
                return x;
}

inline  double  Rnd ()
{
        return ( (double) rand () ) / (double) RAND_MAX;
}

#include "tracer.cpp"

#endif
