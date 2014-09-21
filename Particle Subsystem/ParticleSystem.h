
/*
	Nnanna Kama
	Base particle system class.
	based on Frank Luna's: Introduction to 3D Game Programming with DirectX 9.0c
*/

#pragma once

#include "Matrix4x4.h"
#include <vector>

#ifdef _DIRECT3D

#include <d3dx9.h>
#include <d3dx9math.h>

// link dx9 libs just in case
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")


#define MATRIX4x4	D3DXMATRIX

#else


#define MATRIX4X4	Matrix4x4

#endif


#define MAX_PARTICLES		(64)




//=======================================================================================================================


struct Particle
{
	VECTOR3		initialPos;
	VECTOR3		initialVelocity;
	float       initialSize; // In pixels.
	float       initialTime;
	float       lifeTime;
	float       mass;
	COLOR		initialColor;

	
#ifdef _DIRECT3D
	static IDirect3DVertexDeclaration9* Decl;
#endif
};





//=======================================================================================================================


class SimpleShaderContainer;
class RenderData;


class ParticleSystem
{

public:
	ParticleSystem(void);
	~ParticleSystem(void);


	ParticleSystem(	const VECTOR3& accel, const AABB& box,
					int   maxNumParticles,float timePerParticle);

	// Access Methods

	float getTime()				{ return mTime;}

	void setTime(float t)		{ mTime = t;}

	const AABB& getAABB()const	{ return mAABB;}


	MATRIX4X4 getWorldInv()		const	{return mInvWorld;}
	
	MATRIX4X4 getWorld()		const	{return mWorld;}
	
	VECTOR3 getAccel()			const	{return mAccel;}

	kiss32	getUID()			const	{ return mUID; }
	
	
	void setWorldMtx(const MATRIX4X4& world);

	
	virtual void rebuildDOAlist (float dt);

	virtual void initShader( const char* filename, const char* vp_entry, const char* fp_entry );

	virtual void step(void);

	virtual void VRegister(void);

	virtual void setShaderParams(void);

	virtual void unsetShaderParams(void);

	virtual void setRenderStates(void);



protected:
	
	MATRIX4X4				mWorld;
	MATRIX4X4				mInvWorld;
	VECTOR3					mAccel;
	AABB					mAABB;
	float					mTime;
	const int				mMaxNumParticles;
	float					mTimePerParticle;

	std::vector<Particle>	mParticles;
	std::vector<Particle*>	mAliveParticles;
	std::vector<Particle*>	mDeadParticles;


	SimpleShaderContainer*		mShader;
	RenderData*				mRenderData;

#ifdef _DIRECT3D
	IDirect3DVertexBuffer9* mVertexBuff;
#else
	Particle				mVertexBuff[MAX_PARTICLES];

	kiss32					mUID;

#endif


private:

};


//=======================================================================================================================