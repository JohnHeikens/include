//use AMP for graphics acceleration

//use this for mandelbrot sets!
/*
#include "graphics.h"
#include <amp.h>

using namespace concurrency;
void AmpFog(color FogColor, fp multiplier)
{
	int width = 100, height = 100;
	vec3* colors = new vec3[width * height];
	fp* depthbuffer = new fp[width * height];
	array_view<vec3, 1> AMPcolors(width * height, colors);
	array_view<fp, 1> AMPdepth(width * height, depthbuffer);
	parallel_for_each(AMPcolors.extent,
		[=](index<1> idx) restrict(amp)
		{
			AMPcolors[idx] *= AMPdepth[idx];
		}
	);
}
*/