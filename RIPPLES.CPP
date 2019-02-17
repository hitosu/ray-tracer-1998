#include "Tracer.h"
#include "Ripples.h"

void Ripples ::Apply(Vector &p, SurfaceData &t)
{
	Vector r = p - Center;
	double l = !r;

	if (l > 0.0001)
		r /= l;

	t.n += r * Amount * sin(2 * M_PI * l / WaveLength + Phase) / (1 + l * l);
	t.n = Normalize(t.n);
}
