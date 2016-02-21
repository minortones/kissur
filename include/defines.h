#pragma once


#include <stdlib.h>

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

#ifndef TARGET_CG_SHADERS
#define TARGET_GL_SHADERS	1
#endif

#define MAX_NAME			256

typedef	unsigned int		ksU32;
typedef	int					kiss32;
typedef unsigned short		ksUShort;
typedef short				ksShort;
typedef char				ksByte;

// shader params
#define SP_GLOBAL_AMBIENT	"globalAmbient"
#define SP_LIGHT_COL		"lightColor"
#define SP_MODELVIEWPROJ	"modelViewProj"
#define SP_EYE_POS			"eyePosition"
#define SP_LIGHT_POS		"lightPosition"

#define SP_DIFFUSE			"Kd"
#define SP_AMBIENT			"Ka"
#define SP_EMISSIVE			"Ke"
#define SP_SPECULAR			"Ks"
#define SP_SHININESS		"shininess"

#define SP_POS				"in_pos"
#define SP_NOR				"in_nor"


