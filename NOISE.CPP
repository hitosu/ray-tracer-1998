
#include <StdLib.h>
#include "Tracer.h"

static double NoiseTable[NOISE_DIM][NOISE_DIM][NOISE_DIM];

inline double Sqr(double t)
{
        return t * t;
}

inline double Spline(double t)
{
        return t * t * (3 - 2 * t);
}

void InitNoise()
{
        randomize();
        int i, j, k;

        for (i = 0; i < NOISE_DIM; i++)
                for (j = 0; j < NOISE_DIM; j++)
                        for (k = 0; k < NOISE_DIM; k++)
                                NoiseTable[i][j][k] = (double)rand() / (double)RAND_MAX;
}

double Noise(const Vector &p)
{
        double sx = Mod(p.x, NOISE_DIM);
        double sy = Mod(p.y, NOISE_DIM);
        double sz = Mod(p.z, NOISE_DIM);
        int ix = (int)sx;
        int iy = (int)sy;
        int iz = (int)sz;
        int jx = ix + 1;
        int jy = iy + 1;
        int jz = iz + 1;

        if (jx >= NOISE_DIM)
                jx = 0;

        if (jy >= NOISE_DIM)
                jy = 0;

        if (jz >= NOISE_DIM)
                jz = 0;

        sx = Spline(sx - ix);
        sy = Spline(sy - iy);
        sz = Spline(sz - iz);

        return (1 - sx) * (1 - sy) * (1 - sz) * NoiseTable[ix][iy][iz] +
               (1 - sx) * (1 - sy) * sz * NoiseTable[ix][iy][jz] +
               (1 - sx) * sy * (1 - sz) * NoiseTable[ix][jy][iz] +
               (1 - sx) * sy * sz * NoiseTable[ix][jy][jz] +
               sx * (1 - sy) * (1 - sz) * NoiseTable[jx][iy][iz] +
               sx * (1 - sy) * sz * NoiseTable[jx][iy][jz] +
               sx * sy * (1 - sz) * NoiseTable[jx][jy][iz] +
               sx * sy * sz * NoiseTable[jx][jy][jz];
}

Vector Noise3d(const Vector &p)
{
        Vector res;

        double sx = Mod(p.x, NOISE_DIM);
        double sy = Mod(p.y, NOISE_DIM);
        double sz = Mod(p.z, NOISE_DIM);
        int ix = (int)sx;
        int iy = (int)sy;
        int iz = (int)sz;
        int jx, jy, jz;

        sx = Spline(sx - ix);
        sy = Spline(sy - iy);
        sz = Spline(sz - iz);

        for (int i = 0; i < 3; i++)
        {
                ix = (ix + 5) % NOISE_DIM;
                iy = (iy + 5) % NOISE_DIM;
                iz = (iz + 5) % NOISE_DIM;

                if ((jx = ix + 1) >= NOISE_DIM)
                        jx = 0;

                if ((jy = iy + 1) >= NOISE_DIM)
                        jy = 0;

                if ((jz = iz + 1) >= NOISE_DIM)
                        jz = 0;

                res[i] = (1 - sx) * (1 - sy) * (1 - sz) * NoiseTable[ix][iy][iz] +
                         (1 - sx) * (1 - sy) * sz * NoiseTable[ix][iy][jz] +
                         (1 - sx) * sy * (1 - sz) * NoiseTable[ix][jy][iz] +
                         (1 - sx) * sy * sz * NoiseTable[ix][jy][jz] +
                         sx * (1 - sy) * (1 - sz) * NoiseTable[jx][iy][iz] +
                         sx * (1 - sy) * sz * NoiseTable[jx][iy][jz] +
                         sx * sy * (1 - sz) * NoiseTable[jx][jy][iz] +
                         sx * sy * sz * NoiseTable[jx][jy][jz];
        }

        return res;
}

double Turbulence(const Vector &p, int Octaves)
{
        double k = 1;
        double res = 0;
        Vector r = p;

        for (int i = 0; i < Octaves; i++)
        {
                res += Noise(r) * k;
                r *= 2;
                k *= 0.5;
        }

        return res;
}

Vector Turbulence3d(const Vector &p, int Octaves)
{
        double k = 1;
        Vector r = p;
        Vector res(0);

        for (int i = 0; i < Octaves; i++)
        {
                res += Noise3d(r) * k;
                r *= 2;
                k *= 0.5;
        }

        return res;
}
