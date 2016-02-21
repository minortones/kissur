


#include "ParticleSystem.h"
#include <FX\Particles.h>
#include <RenderEngine\GLRenderer.h>
#include <AppLayer\GLApplication.h>
#include <Macros.h>
#include <RenderData.h>


ParticleSystem::ParticleSystem() : mMaterial( nullptr )
{
	glPointSize(5);

	initShader(gUnlitShaderFilename, gUnlitVertProgram, gUnlitFragProgram);

	VRegister();
}

//=================================================================================================================

ParticleSystem::~ParticleSystem(void)
{
	for (auto& i : mRenderGroups)
		delete i.second;

	for (auto& i : mParticleGroups)
	{
		delete i.first;
		delete i.second;
	}

	delete mMaterial;
}


size_t ParticleSystem::spawn(ks::Emitter& pDesc)
{
	ks::Emitter* em		= new ks::Emitter(pDesc);
	ks::Particles* p	= new ks::Particles(em->mMaxParticles);
	mParticleGroups[em] = p;
	
	RenderData* rg		= new RenderData( p->positions.data(), nullptr, Matrix4x4::IDENTITY );
	rg->vertexSize		= 3;
	rg->renderMode		= GL_POINTS;
	rg->stride			= sizeof(VECTOR3);
	rg->material		= mMaterial;
	
	mRenderGroups[em]	= rg;

	return size_t(em);
}

void ParticleSystem::destroy(size_t pEmitterID)
{
	ks::Emitter* em = reinterpret_cast<ks::Emitter*>(pEmitterID);
	auto rgi = mRenderGroups.find(em);
	if (rgi != mRenderGroups.end())
	{
		delete rgi->second;
		mRenderGroups.erase(rgi);
	}

	auto pgi = mParticleGroups.find(em);
	if (pgi != mParticleGroups.end())
	{
		delete pgi->first;
		delete pgi->second;
		mParticleGroups.erase(pgi);
	}
}



void ParticleSystem::step(float elapsed)
{
	GLRenderer* renderer = Service<GLRenderer>::Get();
	ks::ParticleController c;

	for (auto& i : mParticleGroups)
	{
		auto& em = *i.first;
		auto& p = *i.second;

		c.prune(p, elapsed);
		c.emit(em, p);
		c.step(p, elapsed);

		auto rg			= mRenderGroups[i.first];
		rg->numIndices	= p.live_count();

		renderer->addRenderData(rg);
	}

}

//=================================================================================================================
/*
	Registers this object with the Application and Renderer
	would'a been named register() for consistency but that's a c++ intrinsic
*/

void ParticleSystem::VRegister()
{
	mUID = Service<GLApplication>::Get()->registerObject(this);
}

//=================================================================================================================
/*
	Load a cg shader file
	Grab handles to its parameters
*/

void ParticleSystem::initShader(const char *name, const char* vp_entry, const char* fp_entry)
{
	if (mMaterial == nullptr)
	{
		mMaterial = new Material();
		mMaterial->ShaderContainer = RenderResourceFactory::findOrCreateShader(name);

		mMaterial->ShaderContainer->loadProgram(name, vp_entry, fp_entry);

		Material::setRedPlasticMaterial(mMaterial);
	}
}


//=================================================================================================================


/*
	this assumes a unified shader framework
	meaning every ParticleSys shader SHOULD have the same params and param names
*/
void ParticleSystem::setShaderParams()
{
	/*
		TODO: work in progress...
	*/
	/*mShader->enableVertProfile();
	mShader->enableFragProfile();

	mShader->setFloatParameter("gTime", 2.0f);

	mShader->setFloatParameter("gViewportHeight", 480);

	mShader->setVectorParameter("gAccel", VECTOR3(0,0,0));

	mShader->setVectorParameter("gEyePosL", VECTOR3(0,0,30));

	mShader->setMatrixParameter("gWVP", &Matrix4x4::IDENTITY);*/

}


//=================================================================================================================

void ParticleSystem::unsetShaderParams()
{
	mMaterial->ShaderContainer->disableFragProfile();
	mMaterial->ShaderContainer->disableVertProfile();
}

//=================================================================================================================

