#include <malloc.h>
#include <mem.h>

double GeomThreshold = 0.001;

////////////////////// Sphere methods ///////////////////////

int Sphere ::Intersect(Ray &ray, double &t)
{
        Vector l = Loc - ray.Org; // direction vector
        double L2OC = l & l;      // squared distance
        double tca = l & ray.Dir; // closest dist to center
        double t2hc = Radius2 - L2OC + tca * tca;
        double t2;

        if (t2hc <= 0.0)
                return 0;

        t2hc = sqrt(t2hc);

        if (tca < t2hc) // we are inside
        {
                t = tca + t2hc;
                t2 = tca - t2hc;
        }
        else // we are outside
        {
                t = tca - t2hc;
                t2 = tca + t2hc;
        }

        if (fabs(t) < GeomThreshold)
                t = t2;

        return t > GeomThreshold;
}

Vector Sphere ::FindNormal(Vector &p)
{
        return (p - Loc) / Radius;
}

////////////////////// Plane methods ///////////////////////

Plane ::Plane(double a, double b, double c, double d)
{
        n = Vector(a, b, c);

        double Norm = !n;

        n /= Norm;
        D = d / Norm;
}

int Plane ::Intersect(Ray &ray, double &t)
{
        double vd = n & ray.Dir;

        if (vd > -EPS && vd < EPS)
                return 0;

        t = -((n & ray.Org) + D) / vd;

        return t > GeomThreshold;
}

////////////////////// Rect methods ///////////////////////

Rect ::Rect(Vector &l, Vector &s1, Vector &s2)
{
        Loc = l;
        Side1 = s1;
        Side2 = s2;
        n = Normalize(Side1 ^ Side2);

        double s11 = Side1 & Side1;
        double s12 = Side1 & Side2;
        double s22 = Side2 & Side2;
        double d = s11 * s22 - s12 * s12; // determinant

        ku = (Side1 * s22 - Side2 * s12) / d;
        kv = (Side2 * s11 - Side1 * s12) / d;
        u0 = -(Loc & ku);
        v0 = -(Loc & kv);
}

int Rect ::Intersect(Ray &r, double &t)
{
        double vd = n & r.Dir;

        if (vd > -EPS && vd < EPS)
                return 0;

        if ((t = ((Loc - r.Org) & n) / vd) < GeomThreshold)
                return 0;

        Vector p = r.Point(t);
        double u = u0 + (p & ku);
        double v = v0 + (p & kv);

        return u > 0 && v > 0 && u < 1 && v < 1;
}

////////////////////// Triangle methods ///////////////////////

int Triangle ::Intersect(Ray &r, double &t)
{
        double vd = n & r.Dir;

        if (vd > -EPS && vd < EPS)
                return 0;

        if ((t = ((Loc - r.Org) & n) / vd) < GeomThreshold)
                return 0;

        Vector p = r.Point(t);
        double u = u0 + (p & ku);
        double v = v0 + (p & kv);

        return u > 0 && v > 0 && u + v < 1;
}

//////////////////////////// Box methods /////////////////////////

Box ::Box(Vector &l, Vector &s1, Vector &s2, Vector &s3)
{
        Loc = l;
        e1 = s1;
        e2 = s2;
        e3 = s3;

        Center = Loc + (e1 + e2 + e3) * 0.5;
        InitNormals();
}

Box ::Box(Vector &l, double a, double b, double c)
{
        Loc = l;
        e1 = Vector(a, 0, 0);
        e2 = Vector(0, b, 0);
        e3 = Vector(0, 0, c);

        Center = Loc + (e1 + e2 + e3) * 0.5;
        InitNormals();
}

void Box ::InitNormals()
{
        n[0] = Normalize(e1 ^ e2);
        d1[0] = -(n[0] & Loc);
        d2[0] = -(n[0] & (Loc + e3));

        n[1] = Normalize(e1 ^ e3);
        d1[1] = -(n[1] & Loc);
        d2[1] = -(n[1] & (Loc + e2));

        n[2] = Normalize(e2 ^ e3);
        d1[2] = -(n[2] & Loc);
        d2[2] = -(n[2] & (Loc + e1));

        for (int i = 0; i < 3; i++)
                if (d1[i] > d2[i]) // flip normals, so that d1 < d2
                {
                        d1[i] = -d1[i];
                        d2[i] = -d2[i];
                        n[i] = -n[i];
                }
}

int Box ::Intersect(Ray &r, double &t)
{
        double tNear = -INFINITY; // tNear = max t1
        double tFar = INFINITY;   // tFar  = min t2
        double t1, t2;
        double vd, vo;

        for (int i = 0; i < 3; i++) // process each slab
        {
                vd = r.Dir & n[i];
                vo = r.Org & n[i];

                if (vd > EPS) // t1 < t2, since d1 [i] < d2 [i]
                {
                        t1 = -(vo + d2[i]) / vd;
                        t2 = -(vo + d1[i]) / vd;
                }
                else if (vd < -EPS) // t1 < t2, since d1 [i] < d2 [i]
                {
                        t1 = -(vo + d1[i]) / vd;
                        t2 = -(vo + d2[i]) / vd;
                }
                else // abs ( vd ) < Threshold => ray is parallel to slab
                {
                        if (vo < d1[i] || vo > d2[i])
                                return 0;
                        else
                                continue;
                }

                if (t1 > tNear)
                        tNear = t1;

                if (t2 < tFar)
                        if ((tFar = t2) < GeomThreshold)
                                return 0;

                if (tNear > tFar)
                        return 0;
        }

        t = tNear;

        return t > GeomThreshold;
}

Vector Box ::FindNormal(Vector &p)
{
        double MinDist = INFINITY;
        int index = 0;
        double d, Dist1, Dist2;
        Vector normal;

        for (int i = 0; i < 3; i++)
        {
                d = p & n[i];
                Dist1 = fabs(d + d1[i]); // distances from point to
                Dist2 = fabs(d + d2[i]); // pair of parallel planes

                if (Dist1 < MinDist)
                {
                        MinDist = Dist1;
                        index = i;
                }
                if (Dist2 < MinDist)
                {
                        MinDist = Dist2;
                        index = i;
                }
        }

        normal = n[index]; // normal to plane, the point belongs to
        if (((p - Center) & normal) < 0.0)
                normal = -normal; // normal must point outside of center

        return normal;
}

/////////////////////////// Cylinder methods ////////////////////////////

Cylinder ::Cylinder(Vector &l, Vector &d, double r)
{
        Loc = l;
        Dir = d;
        Radius = r;
        Radius2 = r * r;
        Radius4 = Radius2 * Radius2;
        Len2 = Dir & Dir;
        Len = (double)sqrt(Len2);

        if (fabs(Dir.x) + fabs(Dir.y) > fabs(Dir.z))
                e1 = Vector(Dir.y, -Dir.x, 0.0);
        else
                e1 = Vector(0.0, Dir.z, -Dir.y);

        e1 = Normalize(e1) * Radius;
        e2 = Normalize(Dir ^ e1) * Radius;
        d1 = -(Loc & Dir);
        d2 = -((Loc + Dir) & Dir);
}

int Cylinder ::Intersect(Ray &r, double &t)
{
        Vector l = r.Org - Loc;
        double u0 = l & e1;
        double u1 = r.Dir & e1;
        double v0 = l & e2;
        double v1 = r.Dir & e2;
        double l0 = l & Dir;
        double l1 = r.Dir & Dir;
        double a = u1 * u1 + v1 * v1;
        double b = u0 * u1 + v0 * v1;
        double c = u0 * u0 + v0 * v0 - Radius4;
        double d = b * b - a * c;

        if (d <= 0.0)
                return 0;

        d = sqrt(d);

        double t1 = (-b - d) / a; // t1 < t2, since a > 0
        double t2 = (-b + d) / a;
        double len1 = (l0 + t1 * l1) / Len2;
        double len2 = (l0 + t2 * l1) / Len2;
        Vector p;
        // now check for top/bottom intersections
        if (l1 > EPS)
        {                       // check t1
                if (len1 < 0.0) // bottom intersection
                {
                        t1 = -((r.Org & Dir) + d1) / l1;
                        p = r.Point(t1) - Loc;
                        if ((p & p) >= Radius2)
                                t1 = -1;
                }
                else if (len1 > 1.0) // top intersection
                {
                        t1 = -((r.Org & Dir) + d2) / l1;
                        p = r.Point(t1) - Loc - Dir;
                        if ((p & p) >= Radius2)
                                t1 = -1;
                }
                // check t2
                if (len2 < 0) // bottom intersection
                {
                        t2 = -((r.Org & Dir) + d1) / l1;
                        p = r.Point(t2) - Loc;
                        if ((p & p) >= Radius2)
                                t2 = -1;
                }
                else if (len2 > 1.0) // top intersection
                {
                        t2 = -((r.Org & Dir) + d2) / l1;
                        p = r.Point(t2) - Loc - Dir;
                        if ((p & p) >= Radius2)
                                t2 = -1;
                }
        }
        else if (l1 < -EPS)
        {                     // check t1
                if (len1 < 0) // top intersection
                {
                        t1 = -((r.Org & Dir) + d2) / l1;
                        p = r.Point(t1) - Loc - Dir;
                        if ((p & p) >= Radius2)
                                t1 = -1;
                }
                else if (len1 > 1.0) // bottom intersection
                {
                        t1 = -((r.Org & Dir) + d1) / l1;
                        p = r.Point(t1) - Loc;
                        if ((p & p) >= Radius2)
                                t1 = -1;
                }
                // check t2
                if (len2 < 0.0) // top intersection
                {
                        t2 = -((r.Org & Dir) + d2) / l1;
                        p = r.Point(t2) - Loc - Dir;
                        if ((p & p) >= Radius2)
                                t2 = -1;
                }
                else if (len2 > 1.0) // bottom intersection
                {
                        t2 = -((r.Org & Dir) + d1) / l1;
                        p = r.Point(t2) - Loc;
                        if ((p & p) >= Radius2)
                                t2 = -1;
                }
        }

        if (t1 > GeomThreshold)
        {
                t = t1;
                return 1;
        }

        return (t = t2) > GeomThreshold;
}

Vector Cylinder ::FindNormal(Vector &p)
{
        double t = ((p - Loc) & Dir) / Len2; // parameter along Dir
        Vector n;

        if (t < EPS) // bottom
                n = -Dir / Len;
        else if (t > 1.0 - EPS) // top
                n = Dir / Len;
        else // point on tube
                n = Normalize(p - Loc - Dir * t);

        return n;
}

///////////////////////// Lights implementation /////////////////////////

double PointLight ::Shadow(Vector &p, Vector &l)
{
        l = Loc - p; // vector to light source

        double Dist = !l;                      // distance to light source
        double Attenuation = DistScale / Dist; // distance attenuation of light
        double t;

        l /= Dist; // Normalize vector l

        Ray ray(p, l); // shadow ray
        SurfaceData Texture;
        GObject *Occlude;
        // check all occluding objects
        Attenuation = Attenuation * Attenuation; // distance attenuation is prop. to squared dist.

        while ((Occlude = Scene->Intersect(ray, t)) != NULL && Dist > t)
        { // adjust ray origin and get transparency koeff.
                Occlude->FindTexture(ray.Org = ray.Point(t), Texture);

                if (Texture.Kt < Threshold) // object is opaque
                        return 0;

                if ((Attenuation *= Texture.Kt) < Threshold)
                        return 0;

                Dist -= t;
        }

        return Attenuation;
}

double SpotLight ::Shadow(Vector &p, Vector &l)
{
        l = Loc - p;

        double Dist = !l;                      // distance to light source
        double Attenuation = DistScale / Dist; // distance attenuation of light

        l /= Dist;

        double ld = -(Dir & l);

        if (ld < EndConeAngle)
                return 0;

        double f1 = pow(ld, BeamDistribution);
        double f2 = (ld > ConeAngle ? 1.0 : (ld - EndConeAngle) / (ConeAngle - EndConeAngle));
        double t;
        Ray ray(p, l); // shadow ray
        SurfaceData Texture;
        GObject *Occlude;

        Attenuation *= Attenuation * f1 * f2;

        // check all occluding objects
        while ((Occlude = Scene->Intersect(ray, t)) != NULL && Dist > t)
        { // adjust ray origin and get transparency koeff.
                Occlude->FindTexture(ray.Org = ray.Point(t), Texture);

                if (Texture.Kt < Threshold) // object is opaque
                        return 0;

                if ((Attenuation *= Texture.Kt) < Threshold)
                        return 0;

                Dist -= t;
        }

        return Attenuation;
}

double SphericLight ::Shadow(Vector &p, Vector &l)
{
        l = Loc - p + RndVector() * Radius; // vector to random point on source

        double Dist = !l;                      // distance to light source
        double Attenuation = DistScale / Dist; // distance attenuation of light
        double t;

        l /= Dist; // Normalize vector l

        Ray ray(p, l); // shadow ray
        SurfaceData Texture;
        GObject *Occlude;
        // check all occluding objects
        while ((Occlude = Scene->Intersect(ray, t)) != NULL && Dist > t)
        { // adjust ray origin and get transparency koeff.
                Occlude->FindTexture(ray.Org = ray.Point(t), Texture);

                if (Texture.Kt < Threshold) // object is opaque
                        return 0;

                if ((Attenuation *= Texture.Kt) < Threshold)
                        return 0;

                Dist -= t;
        }

        return Attenuation;
}
