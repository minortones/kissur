
#include "Particles.h"

namespace ks
{

	Random::Random()
	{
		mSeed = rand() ^ ksU32(this);
	}

	Random::Random(ksU32 seed) : mSeed(seed)
	{}

	// http://school.anhb.uwa.edu.au/personalpages/kwessen/shared/Marsaglia03.html
	ksU32 Random::GetU32()
	{
		return (mSeed = 69069 * mSeed + 362437);
	}

	float Random::GetFloatBetween(float a, float b)
	{
		if (a == b)	return a;
		if (a > b)
		{
			float temp = a; a = b; b = temp;
		}

		float f = (GetU32() % 10001) * 0.0001f;			// Get random float in [0, 1] interval.

		f = f * (b - a) + a;
		return f;
	}

	void Random::GetV3PlusMinus(VECTOR3& pDest, const VECTOR3& pRange)
	{
		pDest.x = GetFloatBetween(-pRange.x, pRange.x);
		pDest.y = GetFloatBetween(-pRange.y, pRange.y);
		pDest.z = GetFloatBetween(-pRange.z, pRange.z);
	}

	Particles::Particles(ksU32 pCapacity) : mLiveCount(0)
	{
		resize(pCapacity);
	}

	void Particles::expire(ksU32 pIndex)
	{
		if (pIndex != mLiveCount)
		{
			positions[pIndex]	= positions[mLiveCount]; 
			velocities[pIndex]	= velocities[mLiveCount];
			durations[pIndex]	= durations[mLiveCount]; 
			elapsed[pIndex]		= elapsed[mLiveCount];
			seeds[pIndex]		= seeds[mLiveCount];
		}
		KS_ASSERT(mLiveCount > 0);
		--mLiveCount;
	}

	void Particles::resize(ksU32 pSize)
	{
		positions.resize(pSize);
		velocities.resize(pSize);
		durations.resize(pSize, 0);
		elapsed.resize(pSize, 0);
		seeds.resize(pSize);
	}

	ksU32 Particles::live_count() const				{ return mLiveCount; }

	void Particles::set_live_count(ksU32 pCount)	{ if (pCount < capacity()) mLiveCount = pCount; }

	ksU32 Particles::capacity() const				{ return positions.size(); }


	/////////////////////////////////////////////////////////////////////////////////////////
	// ParticleController
	/////////////////////////////////////////////////////////////////////////////////////////
	ParticleController::ParticleController()
		: mSizeDurationRange(0.f, 1.f, 0.9f, 1.3f)
		, mColourRange(0.f,0.f,0.f,0.f)
		, mVelocityRange(0.7f, 0.7f, 0.7f)
		, mBaseAcceleration(0.f, -9.8f, 0.f)
	{}

	void ParticleController::prune(Particles& pParticles, float elapsed) const
	{
		for (ksU32 i = 0; i < pParticles.live_count(); ++i)
		{
			if (pParticles.elapsed[i] > pParticles.durations[i])
			{
				pParticles.expire(i);
				--i;
			}
			else
				pParticles.elapsed[i] += elapsed;
		}
	}

	void ParticleController::emit(Emitter& pEmitter, Particles& pParticles) const
	{
		ksU32 begin = pParticles.live_count();
		ksU32 end = begin + pEmitter.mEmissionRate;
		if (end >= pParticles.capacity())
			end = pParticles.capacity() - 1;


		Random rng;
		VECTOR3 offsets;
		for (ksU32 i = begin; i < end; ++i)
		{
			pParticles.positions[i]		= pEmitter.mWorldPos;

			rng.GetV3PlusMinus(offsets, mVelocityRange);
			pParticles.velocities[i]	= pEmitter.mEmissionVelocity + offsets;
			pParticles.elapsed[i]		= 0.f;
			pParticles.durations[i]		= rng.GetFloatBetween(mSizeDurationRange.z, mSizeDurationRange.w);
		}

		pParticles.set_live_count(end);
	}

	void ParticleController::step( Particles& pParticles, float elapsed ) const
	{
		const ksU32 numParticles = pParticles.live_count();
		float halfstep = elapsed * 0.5f;
		const auto half_a_t = mBaseAcceleration * halfstep;
		VECTOR3 vel, pos;

		for (ksU32 i = 0; i < numParticles; ++i)
		{
			vel = pParticles.velocities[i];
			pos = pParticles.positions[i];
			
			vel += half_a_t;
			pos += (vel * elapsed);
			vel += half_a_t;
			
			pParticles.velocities[i] = vel;
			pParticles.positions[i] = pos;
		}
	}
}

