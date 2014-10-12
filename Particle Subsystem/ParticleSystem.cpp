/*
	Particle Class, based on Frank Luna implementation
*/

#include "ParticleSystem.h"
#include "../RenderEngine/GLRenderer.h"
#include "../AppLayer/GLApplication.h"
#include "Macros.h"
#include "RenderData.h"



//=======================================================================================================================

float GetRandomFloat(float a, float b)
{
	if( a >= b )	return a;

	float f = (rand()%10001) * 0.0001f;			// Get random float in [0, 1] interval.

	return (f*(b-a))+a;
}





//=================================================================================================================

ParticleSystem::ParticleSystem(void)	: mAccel( VECTOR3(0.0f,-9.8f,0.0f) ),
										mAABB(AABB()), mTime(0.0f), mMaxNumParticles(MAX_PARTICLES),
										mTimePerParticle(5.0f), mShader(0), mUID(-1)
{
	// Allocate memory for maximum number of particles.
	mParticles.resize(mMaxNumParticles);
	mAliveParticles.reserve(mMaxNumParticles);
	mDeadParticles.reserve(mMaxNumParticles);

	// They all start off ALIVE
	for(int i = 0; i < mMaxNumParticles; ++i)
	{
		mParticles[i].lifeTime		= 10.0f;
		mParticles[i].initialTime	= 0.0f;
		mParticles[i].initialSize	= GetRandomFloat(12.0f, 15.0f);
		mParticles[i].initialColor	= COLOR(1.0f,1.0f,1.0f,1.0f);// d3d::WHITE;
		mParticles[i].mass			= GetRandomFloat(2.0f, 4.0f);

		//random Vector
		VECTOR3 d;
		d.x = GetRandomFloat(-1.0f, 1.0f);
		d.y = GetRandomFloat(-1.0f, 1.0f);
		d.z = GetRandomFloat(-1.0f, 1.0f);

		float speed = GetRandomFloat(50.0f, 150.0f);
		mParticles[i].initialVelocity = speed*d;

		float r = GetRandomFloat(0.0f, 2.0f);
		mParticles[i].initialPos = r*d;
	}



#ifdef _DIRECT3D
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mInvWorld);
	Device->CreateVertexBuffer(	mMaxNumParticles*sizeof(Particle),
								D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|D3DUSAGE_POINTS,
								0, D3DPOOL_DEFAULT, &mVertexBuff, 0);
#else
	mWorld		= Matrix4x4::IDENTITY;
	mInvWorld	= Matrix4x4::IDENTITY;
	//mVertexBuff = calloc(mMaxNumParticles, sizeof(Particle));	//mVertexBuff	= new Particle [mMaxNumParticles];
	//mRenderData = new RenderData(mVertexBuff, 0);
#endif

	VRegister();
}

//=================================================================================================================

ParticleSystem::~ParticleSystem(void)
{
	delete mRenderData;
	//SAFE_DELETE_ARRAY(mVertexBuff);
	//SAFE_DELETE(mShader);				// destroyed by factory
}

//=================================================================================================================


ParticleSystem::ParticleSystem(const VECTOR3 &accel, const AABB &box, int maxNumParticles, float timePerParticle)
/* init vars */
: mAccel(accel), mAABB(box), mTime(0.0f), mMaxNumParticles(maxNumParticles), mTimePerParticle(timePerParticle), mUID(-1)
{
	
	ParticleSystem();

}



//=================================================================================================================

void ParticleSystem::setWorldMtx(const MATRIX4X4& world)
{
	mWorld = world;

	// Compute the change of coordinates matrix that changes coordinates 
	// relative to world space so that they are relative to the particle
	// system's local space.
	
#ifdef _DIRECT3D
	D3DXMatrixInverse(&mInvWorld, 0, &mWorld);
#else
	mInvWorld = mWorld.Inverse();
#endif
}




//=================================================================================================================
	// Rebuild the dead and alive list.
	// Note: resize(0) does not deallocate memory or change the capacity of the vector
	// Fill the vertexBuffer with the particle positions

void ParticleSystem::rebuildDOAlist(float dt)
{
	mTime += dt;
	mDeadParticles.resize(0);
	mAliveParticles.resize(0);
	//memset( mVertexBuff, 0, mMaxNumParticles * sizeof(Particle) );

	// For each particle.
	for(int i = 0; i < mMaxNumParticles; ++i)
	{
		// Is the particle dead?
  		if( (mTime - mParticles[i].initialTime) > mParticles[i].lifeTime)
		{
			mDeadParticles.push_back(&mParticles[i]);
		}
		else
		{
			mAliveParticles.push_back(&mParticles[i]);
		}
	}


	// A negative/zero mTimePerParticle value means not to emit any particles.
	if( mTimePerParticle > 0.0f )
	{
		// Emit particles.
		static float timeAccum = 0.0f;
		timeAccum += dt;
	}


	// copy data into vertex buffer
	size_t idx = 0;
	size_t num_live_particles = mParticles.size();
	for(size_t i = 0; i < num_live_particles; ++i)
	{
		mVertexBuff[i] = mParticles[i];
		//memcpy(mVertexBuff, &mAliveParticles, mMaxNumParticles * sizeof(Particle));
		/*mVertexBuff[idx++] = (void*)mAliveParticles[i];
		mVertexBuff[idx++] = mAliveParticles[i];
		mVertexBuff[idx++] = mAliveParticles[i];*/

#ifdef _DEBUG
		//printf("Particle %d: %f, %f, %f\n", i, mVertexBuff[i], mVertexBuff[i+1], mVertexBuff[i+2]);
#endif
	
	}
}


//=================================================================================================================
/*
	Rebuild dead/alive lists (function call)
	Flush renderData to renderer.
*/

void ParticleSystem::step(void)
{
	rebuildDOAlist(0.0f);


	// fill up RenderData array and submit
	mRenderData->indexBuffer	= NULL;
	mRenderData->vertexBuffer	= mVertexBuff;
	mRenderData->vertexSize		= 3;
	mRenderData->renderMode		= GL_POINTS;
	mRenderData->stride			= sizeof(Particle);
	mRenderData->numIndices		= mMaxNumParticles;

	//printf("------------PSYS STEP-------------------\n");
	Service<GLRenderer>::Get()->addRenderData( mRenderData );


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

void ParticleSystem::initShader(const char *filename, const char* vp_entry, const char* fp_entry)
{
	mShader = RenderResourceFactory::findOrCreateShader( filename );
	
	mShader->loadProgram(filename, vp_entry, fp_entry);
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
	mShader->disableFragProfile();
	mShader->disableVertProfile();
}

//=================================================================================================================


void ParticleSystem::setRenderStates()
{
#ifdef _DIRECT3D
	// bla bla

#else

	//glPolygonMode( GL_FRONT_AND_BACK, GL_QUADS );
	//glColor3f(0.0f, 1.0f, 0.0f);
	//glPointSize( 0.9f );
	//glDepthFunc();

#endif
}

//=================================================================================================================
