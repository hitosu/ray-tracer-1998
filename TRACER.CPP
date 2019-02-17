#include <malloc.h>
#include <stdlib.h>

/////////////////////////////  Globals  ////////////////////////////////////

Vector Eye(0, 0, 0);                // camera position
Vector EyeDir(0, 0, 1);             // viewing directing ( normal to image plane )
Vector Vx(1, 0, 0);                 // image plane basis - horizontal vector
Vector Vy(0, 1, 0);                 // vertical vector
Vector Ambient(1.0);                // ambient light intensity
Vector Background(0.0, 0.05, 0.05); // background color
Medium Air = {1, 0};                // basic mediums : Air
Medium Glass = {1.5, 0};            // glass
int Level = 0;                      // current recusion level
double Threshold = 0.01;            // accuracy of computations
int MaxLevel = 10;                  // max. levels of recursion
Environment *Scene;
long TotalRays = 0l;

////////////////////////// GObject methods /////////////////////////////

GObject ::~GObject()
{
        if (Mapping != NULL)
                delete Mapping;

        for (Texture *m = Material; m != NULL; m = Material)
        {
                Material = Material->next;
                delete m;
        }
}

void GObject ::Add(Texture *m)
{
        m->next = Material;
        m->object = this;

        Material = m;
}

void GObject ::FindTexture(Vector &p, SurfaceData &t)
{
        t = DefMaterial;
        t.n = FindNormal(p);

        if (Mapping != NULL)
                t.MapCoord = Mapping->Apply(p);

        for (Texture *m = Material; m != NULL; m = m->next)
                m->Apply(p, t);
}

////////////////////////// Environment methods /////////////////////////

Environment ::~Environment()
{
        int i;
        //delete all contained objects
        for (i = 0; i < LightsCount; i++)
                delete Light[i];

        for (i = 0; i < SolidsCount; i++)
                delete Solid[i];
}

void Environment ::Add(LightSource *l)
{
        if (LightsCount < MAX_LIGHTS - 1)
                Light[LightsCount++] = l;
}

void Environment ::Add(GObject *o)
{
        if (SolidsCount < MAX_SOLIDS - 1)
                Solid[SolidsCount++] = o;
}

//////////////////////////////////////////////////
// find closest intersection with scene objects //
//////////////////////////////////////////////////

GObject *Environment ::Intersect(Ray &ray, double &t)
{
        GObject *ClosestObj = NULL;
        double ClosestDist = INFINITY;

        for (int i = 0; i < SolidsCount; i++)    // check every object
                if (Solid[i]->Intersect(ray, t)) // if intersect then check distance
                        if (t < ClosestDist)
                        {
                                ClosestDist = t;
                                ClosestObj = Solid[i];
                        }

        t = ClosestDist;

        return ClosestObj;
}
#pragma argsused // turn off parameter no used warning
Vector Environment ::ShadeBackground(Ray &ray)
{
        return Background;
}

////////////////////////// Functions ////////////////////

void SetCamera(Vector &Org, Vector &Dir, Vector &UpDir)
{
        Eye = Org;                   // eye point
        EyeDir = Dir;                // viewing direction
        Vx = Normalize(UpDir ^ Dir); // build orthogonal basis of image plane
        Vy = Normalize(Dir ^ Vx);    // EyeDir orthgonal to this basic ( image plane )
}

///////////////////////////////////////////////////////
// get a pixel ray for a given screen point ( x, y ) //
///////////////////////////////////////////////////////

void Camera(double x, double y, Ray &ray)
{
        ray.Org = Eye;
        ray.Dir = Normalize(EyeDir + Vx * x + Vy * y);
}

/////////////////////////////////////////
// Trace a given ray through the scene //
/////////////////////////////////////////

Vector Trace(Medium &CurMed, double Weight, Ray &ray)
{
        GObject *Obj;
        double t = INFINITY;
        Vector Color;

        Level++;
        TotalRays++;

        if ((Obj = Scene->Intersect(ray, t)) != NULL) // ray hit some object
        {
                Color = Shade(CurMed, Weight, ray.Point(t), ray.Dir, Obj);

                if (CurMed.Betta > Threshold)
                        Color *= exp(-t * CurMed.Betta);
        }
        else
                Color = Scene->ShadeBackground(ray);

        Level--;

        return Color;
}

/////////////////////////////////////////////////////////////
// compute light coming from point p in the direction View //
//       using Whitted's illumination model                //
/////////////////////////////////////////////////////////////

Vector Shade(Medium &CurMed, double Weight, Vector &p, Vector &View, GObject *Obj)
{
        SurfaceData txt;
        Ray ray;
        Vector Color;
        Vector l;  // light vector
        double Sh; // light shadow coeff.
        Vector h;  // vector between -View and light vector
        double ln, vn;
        int Entering = 1; // flag whether we're entering or leaving object

        Obj->FindTexture(p, txt);

        if ((vn = View & txt.n) > 0) // force ( -View, n ) > 0
        {
                txt.n = -txt.n;
                vn = -vn;
                Entering = 0;
        }

        ray.Org = p;                          // since all rays will be cast from here
        Color = Ambient * txt.Color * txt.Ka; // get ambient light

        for (int i = 0; i < Scene->LightsCount; i++)
                if ((Sh = Scene->Light[i]->Shadow(p, l)) > Threshold)
                        if ((ln = l & txt.n) > Threshold) // if light is visible
                        {
                                if (txt.Kd > Threshold) // compute direct diffuse light
                                        Color += Scene->Light[i]->Color * txt.Color * (txt.Kd * Sh * ln);

                                if (txt.Ks > Threshold) // compute direct specular light, via Phong shading
                                {                       // compute half-vector between -View and l
                                        h = Normalize(l - View);
                                        Color += Scene->Light[i]->Color * (txt.Ks *
                                                                           Sh * pow(txt.n & h, txt.p));
                                }
                        }

        double rWeight = Weight * txt.Kr; // weight of reflected ray
        double tWeight = Weight * txt.Kt; // weight of transmitted ray
                                          // check for reflected ray
        if (rWeight > Threshold && Level < MaxLevel)
        {
                ray.Dir = View - txt.n * (2 * vn); // get reflected ray direction
                Color += txt.Kr * Trace(CurMed, rWeight, ray);
        }
        // check for transmitted ray
        if (tWeight > Threshold && Level < MaxLevel)
        { // relative index of refraction
                double Eta = CurMed.nRefr / (Entering ? txt.Med.nRefr : Air.nRefr);
                double ci = -vn;                             // cosine of incident angle
                double ctSq = 1 + Eta * Eta * (ci * ci - 1); // squared cosine of transm. angle

                if (ctSq > Threshold) // not a Total Internal Reflection
                {
                        ray.Dir = View * Eta + txt.n * (Eta * ci - sqrt(ctSq));

                        if (Entering) // ray enters object ( txt.Med )
                                Color += txt.Kt * Trace(txt.Med, tWeight, ray);
                        else // ray leaves object ( Air )
                                Color += txt.Kt * Trace(Air, tWeight, ray);
                }
        }

        return Color;
}

double SawWave(double t)
{
        t -= floor(t);

        return t >= 0.5 ? 2 * (1 - t) : 2 * t;
}
