
#pragma once

#include "Matrix4x4.h"
#include "Structs.h"
#include <Array.h>
#include <map>

struct Material;
class RenderData;

namespace ks
{
	struct Particles;
	struct Emitter;
}



class ParticleSystem
{

public:
	ParticleSystem();
	~ParticleSystem();

	size_t spawn(ks::Emitter& pDesc);

	void destroy(size_t pEmitterID);

	ksU32 getUID() const								{ return mUID; }
	

	void initShader( const char* filename, const char* vp_entry, const char* fp_entry );

	void step(float elapsed);

	void setShaderParams();

	void unsetShaderParams();


private:
	void VRegister(void);

	ksU32					mUID;
	Material*				mMaterial;

	std::map<ks::Emitter*, ks::Particles*>	mParticleGroups;
	std::map<ks::Emitter*, RenderData*>		mRenderGroups;

};


//=======================================================================================================================