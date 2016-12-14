#include <stdio.h>
#include "vector3.h"

typedef GeographicLib::Math::real real;
typedef vector3<real> vec3r;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const real GEO_RAD = 6375000.;

inline vec3r ll2v(real lat, real lon)
{
    const real R=GEO_RAD;
    const real Rp=R*cos(lat);
    return vec3r(Rp*sin(lon), Rp*cos(lon), R*sin(lat));
}

inline real D2R(const real deg)
{
    return deg*M_PI/180.;
}

void DistanciaEsfera(real la1, real lo1, real la2, real lo2)
{
// lat -90-90
// lon -180-180

    vec3r v1 = ll2v(D2R(la1), D2R(lo1));
    vec3r v2 = ll2v(D2R(la2), D2R(lo2));

    real angl = v1.ang(v2);
    real dist = angl * GEO_RAD;

    printf("%f %f\n", (float)dist, (float)angl*180.f);
}


// para comparar usar ./Geod -i
//flta un main y eso...

