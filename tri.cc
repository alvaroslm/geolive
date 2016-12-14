
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const double GEO_RAD = 6375000.;



int main()
{
	double A = 4 * M_PI * GEO_RAD * GEO_RAD;
	double e = 2 * M_PI * GEO_RAD;

	e /= 4.;
	A /= 8.;
	double n=1, m=8;

	printf("   l         area    -      edge length(m)   edge(miles)     edge(km)     totalnodes(x1M)    nodes\n");
	for(int i=1; e>=100.; ++i)
	{
		n += m;
		printf("level %2d: %14.6g %14.6g %14.6g %14.6g %14.5g %14.5g\n", i, A, e, e*0.621371192/1000., e/1000., n/1.E6, m);
		A /= 4.;
		e /= 2.;
		m *= 4;
	}

	return 0;
}
