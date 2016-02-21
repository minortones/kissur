#pragma once


#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#ifndef NULL
#define NULL 0;
#endif

#ifndef FUNCTIONPTR
#define FUNCTIONPTR void ( *func)(void)
#endif

#ifndef DLLENTRY
#ifdef SOMERANDOMSHITTHATAINTDEFINED
#define DLLENTRY __declspec(dllexport)
#else
#define DLLENTRY
#endif
#endif


//#define TARGET_CG_SHADERS	1
#define TARGET_GL_SHADERS	1

typedef	unsigned int	ksU32;
typedef	int				kiss32;

/*
	Code courtesy http://sourceforge.net/projects/daedalus-n64/
	FFXMaths.h
	Used as is, save for inclusion/exclusion of standard headers
*/


static float InvSqrt(float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x;			// get bits for floating value
	i = 0x5f3759df - (i>>1);	// gives initial guess y0
	x = *(float*)&i;			// convert bits back to float
	x = x*(1.5f-xhalf*x*x);		// Newton step, repeating increases accuracy
	//x = x*(1.5f-xhalf*x*x);		// Newton step, repeating increases accuracy
	//x = x*(1.5f-xhalf*x*x);		// Newton step, repeating increases accuracy
	//x = x*(1.5f-xhalf*x*x);		// Newton step, repeating increases accuracy
	//x = x*(1.5f-xhalf*x*x);		// Newton step, repeating increases accuracy

	return x;
}

static inline float Sqrt( float x )
{
	return x * InvSqrt( x );
}

static inline float frand()
{
	int x = (rand()*rand()) & 0x007fffff;
	x |= 0x3f800000;				// 1.0f in IEEE fp
	return (*(float*)&x) - 1.0f;
}

// Returns in rand -1 .. +1
static inline float frand_11()
{
	return (frand() * 2.0f) - 1.0f;
}

static inline float frand_range( float lo, float hi )
{
	return lo + (hi - lo) * frand();
}

