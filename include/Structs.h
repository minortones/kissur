/*
	Rip-off of common data-types
	Implemented for easier porting.
*/

#ifndef KISS_STRUCTS_H
#define KISS_STRUCTS_H

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "defines.h"

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



#ifndef KISS_INFINITY
#define KISS_INFINITY	3.402823466e+38F		/* max float val */
#endif

#ifndef PI
#define PI	3.14159265358979323846
#endif


#define RAD_TO_DEG(rad)		((rad) * (180.f / PI)	)
#define DEG_TO_RAD(deg)		((deg) * (PI / 180.f)	)



/*
	From Vector4.h
	courtesy http://sourceforge.net/projects/daedalus-n64/
	Used as is.
*/



class v4
{
public:
	v4() {}
	v4( float _x, float _y, float _z, float _w ) : x( _x ), y( _y ), z( _z ), w( _w ) {}

	float Normalise()
	{
		float	len( Length() );
		if(len > 0.0001f)
		{
			float r( 1.0f / len );
			x *= r;
			y *= r;
			z *= r;
			w *= r;
		}

		return len;
	}

	v4 operator+( const v4 & v ) const
	{
		return v4( x + v.x, y + v.y, z + v.z, w + v.w );
	}
	v4 operator-( const v4 & v ) const
	{
		return v4( x - v.x, y - v.y, z - v.z, w - v.w );
	}

	v4 operator*( float s ) const
	{
		return v4( x * s, y * s, z * s, w * s );
	}


	float Length() const
	{
		return 1.0f / InvSqrt( (x*x)+(y*y)+(z*z)+(w*w) );
	}

	float Dot( const v4 & rhs ) const
	{
		return (x*rhs.x) + (y*rhs.y) + (z*rhs.z) + (w*rhs.w);
	}


	float x, y, z, w;
};





//=======================================================================================================================

/*
	Simple Vector3 struct.
	
*/

typedef struct VECTOR3
{
public:
	VECTOR3();
    VECTOR3( const float * );
    VECTOR3( const VECTOR3& );
    VECTOR3( float x, float y, float z );

    // casting
    operator float* ();
    operator const float* () const;

    // assignment operators
    VECTOR3& operator += ( const VECTOR3& );
    VECTOR3& operator -= ( const VECTOR3& );
    VECTOR3& operator *= ( float );
    VECTOR3& operator /= ( float );

    // unary operators
    VECTOR3 operator + () const;
    VECTOR3 operator - () const;

    // binary operators
    VECTOR3 operator + ( const VECTOR3& ) const;
    VECTOR3 operator - ( const VECTOR3& ) const;
    VECTOR3 operator * ( float ) const;
    VECTOR3 operator / ( float ) const;

    friend VECTOR3 operator * ( float, const struct VECTOR3& );

    bool operator == ( const VECTOR3& ) const;
    bool operator != ( const VECTOR3& ) const;


	// member data
	float x,y,z;

}VECTOR3;



//=======================================================================================================================

typedef struct COLOR
{
	COLOR();
	COLOR( float, float, float, float );
	
	// assignment
    COLOR& operator += ( const COLOR& );
    COLOR& operator -= ( const COLOR& );
    COLOR& operator *= ( float );
    COLOR& operator /= ( float );

    // unary
    COLOR operator + () const;
    COLOR operator - () const;

    // binary
    COLOR operator + ( const COLOR& ) const;
    COLOR operator - ( const COLOR& ) const;
    COLOR operator * ( float ) const;
    COLOR operator / ( float ) const;

    friend COLOR operator * ( float, const COLOR& );

    bool operator == ( const COLOR& ) const;
    bool operator != ( const COLOR& ) const;

	// member data
	float r,g,b,a;

}COLOR;	// Vector4;


//=======================================================================================================================

/*
	Bounding Volumes
*/


typedef struct AABB 
{
	// Initialize to an infinitely large bounding box.
	AABB()
		: minPt(KISS_INFINITY, KISS_INFINITY, KISS_INFINITY),
		  maxPt(-KISS_INFINITY, -KISS_INFINITY, -KISS_INFINITY){}

    VECTOR3 center()
	{
		return 0.5f*(minPt+maxPt);
	}

	VECTOR3 minPt;
	VECTOR3 maxPt;

}AABB;


typedef struct BoundingSphere
{
	BoundingSphere()
		: pos(0.0f, 0.0f, 0.0f), radius(0.0f){}

	VECTOR3 pos;
	float radius;

}BoundingSphere;

//=======================================================================================================================

#endif

#include "Structs.inl"