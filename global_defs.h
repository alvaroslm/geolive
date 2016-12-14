
// (C) Alvaro Salmador 2011
//  All rights reserved

#ifndef __GLOBAL_DEFS_H_INCLUIDO_
#define __GLOBAL_DEFS_H_INCLUIDO_ 1

#define GEO_GLNODES 1 //fuciones de dibujar opengl
//#undef GEO_GLNODES
//#define NDEBUG // para que no salten asserts

#define VER_ICOSAEDRO 0 // 1 para ver solo ico, 2 para ver tambien lo otro

#pragma warning(disable:4996)

const double PI = 3.141592653589793238462643383279502884197;
const float F_PI = 3.141592653589793238462643383279502884197f;
const double PI2 = 2.*3.141592653589793238462643383279502884197;
const float F_PI2 = 2.f*3.141592653589793238462643383279502884197f;
const float F_GEO_R = 6375000.f;
const float F_GEO_R2 = 2.f*6375000.f;
const double D_GEO_R = 6375000.;
const double D_GEO_R2 = 2.*6375000.;
#define GEO_GL_RAD 60.f

#define GEO_ADJLON	-53.f	//offset del icosaedro
#define GEO_ADJLAT	0.f//no impl.

#define LEVEL_LEAF	15 //15 =>257m de lado max
#define LEVEL_MIN	5 //5

#include "toolkit.h"

#endif