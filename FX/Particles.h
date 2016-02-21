
#ifndef KS_PARTICLES_H
#define KS_PARTICLES_H

#pragma once;

#include "Structs.h"
#include <Array.h>

namespace ks {

//#if PREFER_STL_VECTOR
//
//#include <vector>
//	template <typename T>
//	using fxvector = std::vector
//#else
//
//#include <Array.h>
//	template <typename T>
//	using fxvector = typename ks::Array
//#endif

	struct Random
	{
		Random();
		Random(ksU32 seed);

		ksU32	GetU32();
		float	GetFloatBetween(float x, float y);
		void	GetV3PlusMinus(VECTOR3& pDest, const VECTOR3& pRange);

	private:
		ksU32 mSeed;
	};

	struct Particles
	{
		Array<VECTOR3>	positions;
		Array<VECTOR3>	velocities;
		Array<float>	durations;
		Array<float>	elapsed;
		Array<ksU32>	seeds;		// use to re-generate size & colour

		Particles(ksU32 pCapacity);

		void expire(ksU32 pIndex);
		void resize(ksU32 pSize);

		ksU32 live_count() const;
		ksU32 capacity() const;
		void set_live_count(ksU32 pCount);
	private:
		ksU32	mLiveCount;
	};


	struct Emitter
	{
		VECTOR3	mWorldPos;
		VECTOR3 mEmissionVelocity;
		ksU32	mEmissionRate;
		ksU32	mMaxParticles;
	};


	struct ParticleController
	{
		ParticleController();

		void prune(Particles& pParticles, float elapsed) const;
		void emit(Emitter& pEmitter, Particles& pParticles) const;

		void step(Particles& pParticles, float elapsed) const;


		v4		mSizeDurationRange;		// (x,y)-> size range; (z,w)-> duration range
		v4		mColourRange;
		VECTOR3	mVelocityRange;
		VECTOR3 mBaseAcceleration;
	};

}


#endif