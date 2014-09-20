
#include "SceneObject.h"
#include "shared_include/Macros.h"
#include "AppLayer/GLApplication.h"
#include "AssetLoader/kissModel.h"
#include "Physics Subsystem/PhysicsObject.h"
#include "shared_include/RenderData.h"
#include "RenderEngine/GLRenderer.h"
#include "SceneManagement/Camera.h"

using namespace kiss;

SceneObject::SceneObject(void) : mUID(-1),
								mModel(NULL),
								mMaterial(NULL),
								mRigidBody(NULL),
								mRenderData(NULL),
								mWorld(Matrix4x4::IDENTITY)
{
	//mRigidBody	= new PhysicsObject;
	VRegister();
}


//================================================================================================================


SceneObject::~SceneObject(void)
{
	//SAFE_DELETE(mModel);
	delete mRenderData;
	delete mMaterial;
}


//================================================================================================================


void SceneObject::VRegister()
{
	mUID = Service<GLApplication>::Get()->registerObject( this );
}


//================================================================================================================


void SceneObject::loadModel(const char* filepath)
{
	mModel = new Model();
	if ( mModel->loadModelFromFile(filepath) )
	{
		mModel->computeNormals();
		mModel->compileModel(eptAll);
		mRenderData				= new RenderData(mModel->getCompiledVertices(), mModel->getCompiledIndices(eptTriangles), mWorld);
		mRenderData->stride		= mModel->getCompiledVertexSize() * sizeof(float);
		mRenderData->batchCount	= mModel->getCompiledIndexCount( eptTriangles);
		mRenderData->normOffset	= mModel->getCompiledNormalOffset();
		mRenderData->vertexSize	= mModel->getPositionSize();
		mRenderData->renderMode	= GL_TRIANGLES;

	}
	else
	{
		printf("Error loading model '%s'\n", filepath);
		SAFE_DELETE(mModel);
	}
}


//================================================================================================================


void SceneObject::initMaterial( const char* filename, const char* vp_entry, const char* fp_entry )
{
	mMaterial = new Material( RenderResourceFactory::findOrCreateShader( filename ) );

	setBrassMaterial( mMaterial );

	mMaterial->ShaderContainer->loadProgram(filename, vp_entry, fp_entry);
}

//================================================================================================================


void SceneObject::setRenderStates()
{

}

//================================================================================================================

void SceneObject::update()
{
	if ( mRenderData )
	{
		mRenderData->material	= mMaterial;
		//Service<GLRenderer>::Get()->addRenderData( mRenderData );
	}
}

//================================================================================================================