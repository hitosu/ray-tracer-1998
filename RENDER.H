#ifndef __RENDER__
#define __RENDER__

#include        <stdlib.h>

void    RenderScene ( double, double, int, int, char * );
void    DistributedRenderScene ( double, double, int, int, int, int, char * );
void    AdaptiveDistributedRenderScene ( double, double, int, int, int, int, double, char * );

#include "render.cpp"

#endif