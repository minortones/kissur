/*
	Nnanna Kama
	Render class Impl
*/



#include "Macros.h"
#include "GLRenderer.h"
#include "Light.h"
#include "RenderData.h"
#include "RenderResourceFactory.h"
#include "../Particle Subsystem/ParticleSystem.h"
#include "../SceneObject.h"
#include "../SceneManagement/Camera.h"
#include "../AppLayer/GLApplication.h"


const float gLightRadius		= 25.f;
const Light gDefaultLight		= { VECTOR3(gLightRadius*sin(myLightAngle), 50.5f, gLightRadius*cos(myLightAngle) ), VECTOR3( 0.95f, 0.95f, 0.95f ) };
const VECTOR3 gGlobalAmbient( 0.1f, 0.1f, 0.1f );


///////////////////////////////////////////////////////////////////////////////////
template<>				kissU32 getGLType(const float* pT)			{ return GL_FLOAT; }
template<>				kissU32 getGLType(const kissU32* pT)		{ return GL_UNSIGNED_INT; }
template<>				kissU32 getGLType(const kissUShort* pT)		{ return GL_UNSIGNED_SHORT; }
template<>				kissU32 getGLType(const kissByte* pT)		{ return GL_UNSIGNED_BYTE; }
///////////////////////////////////////////////////////////////////////////////////

//================================================================================================================

GLRenderer::GLRenderer(void) : mDefaultMaterial(NULL), mMRUShader(NULL)
{	
}


//================================================================================================================


GLRenderer::~GLRenderer(void)
{
	SAFE_DELETE(mDefaultMaterial);
	mRenderData.clear();
	RenderResourceFactory::shutDown();
}


//================================================================================================================


void GLRenderer::init()
{
	if(!mDefaultMaterial)
	{
		mDefaultMaterial					= new Material( RenderResourceFactory::findOrCreateShader(gDefaultShaderFilename) );
		setRedPlasticMaterial(mDefaultMaterial);

		mDefaultMaterial->ShaderContainer->loadProgram(gDefaultShaderFilename, gDefaultVertProgram, gDefaultFragProgram);
		mDefaultMaterial->ShaderContainer->bindProgram();
		mDefaultMaterial->ShaderContainer->setVectorParameter( SP_GLOBAL_AMBIENT, &gGlobalAmbient);
		mDefaultMaterial->ShaderContainer->setVectorParameter( SP_LIGHT_COL, &gDefaultLight.color );
		mDefaultMaterial->ShaderContainer->unbindProgram();
		
		CameraManager::createCamera();
	}

//#ifndef GET_PARAM
//#define GET_PARAM(name) \
//	myCgVertexParam_##name = \
//	mShaderContainer->getNamedParam(#name); \
//	checkForCgError("could not get " #name " parameter");
//
//	GET_PARAM(modelViewProj);
//	GET_PARAM(globalAmbient);
//	GET_PARAM(lightColor);
//	GET_PARAM(lightPosition);
//	GET_PARAM(eyePosition);
//	GET_PARAM(Ke);
//	GET_PARAM(Ka);
//	GET_PARAM(Kd);
//	GET_PARAM(Ks);
//	GET_PARAM(shininess);
	//#endif
}



//================================================================================================================

void GLRenderer::update( float pElaspedS )
{
	CHECK_GL_ERROR;
}


//================================================================================================================

void GLRenderer::addRenderData( const RenderData* r )
{
	mRenderData.push_back( r );
}

//================================================================================================================
/*
	This should be auto-called every frame
	to prevent the buffers from growing infinitely
	TODO: clearing and repopulating the render-list every frame is potentially expensive
	Only remove items when necessary.
*/

void GLRenderer::flushRenderData()
{
	mRenderData.clear();
}


void GLRenderer::uploadShaderConstants( Material* pMat, const Camera* pCam, const Matrix4x4& pTransform )
{
	SimpleShaderContainer* shader	= pMat->ShaderContainer;
	const VECTOR3& camPos			= pCam->getPosition();
	const Matrix4x4& view			= pCam->getView();
	const Matrix4x4& projection		= pCam->getProjection();
	
	Matrix4x4 inv( pTransform.Inverse() );
	Matrix4x4 mvp( projection * view * pTransform );

	VECTOR3 invEye( inv.TransformNormal( camPos ) );
	VECTOR3 invLight( inv.TransformNormal( gDefaultLight.position ) );


	if( mMRUShader != shader )
	{
		if ( mMRUShader )
		{
			mMRUShader->disableVertProfile();
			mMRUShader->disableFragProfile();
			mMRUShader->unbindProgram();
		}

		if ( !shader->isBound() )
		{
			shader->bindProgram();
			shader->enableVertProfile();
			shader->enableFragProfile();
		}
	}
	
	
	shader->setVectorParameter( SP_EYE_POS,			&invEye);
	shader->setVectorParameter( SP_LIGHT_POS,		&invLight);
	shader->setMatrixParameter( SP_MODELVIEWPROJ,	&mvp);		// Set parameter with row-major matrix.

	pMat->SetShaderParams();
}


//================================================================================================================

void GLRenderer::render()
{
	CHECK_GL_ERROR;

	Material *mat;
	const Camera* cam		= CameraManager::getMainCamera();

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );

	for( RenderDataArray::iterator itr = mRenderData.begin(); itr != mRenderData.end(); itr++ )
	{
		const RenderData* rd( *itr );
		mat					= rd->material ? rd->material : mDefaultMaterial;

		uploadShaderConstants( mat, cam, rd->Transform );

		const float* vb		= (const float*)rd->vertexBuffer;
		const kissU32* ib	= rd->indexBuffer;
		kiss32 vert_size	= rd->vertexSize;
		kiss32 stride		= rd->stride;
		kiss32 num_indices	= rd->numIndices;
		const float* norms	= vb + rd->normOffset;

		glVertexPointer( vert_size, GET_GLTYPE(vb), stride, vb);
		glNormalPointer( GET_GLTYPE(norms), stride, norms);

#ifdef TARGET_GL_SHADERS
		// these correspond to glBindAttribLocation()
		glVertexAttribPointer(SA_POSITION,	vert_size, GET_GLTYPE(vb),		GL_FALSE, stride, vb);
		glVertexAttribPointer(SA_NORMAL,	vert_size, GET_GLTYPE(norms),	GL_FALSE, stride, norms);
#endif

		glDrawElements(rd->renderMode, num_indices, GET_GLTYPE(ib), ib);

		mMRUShader = mat->ShaderContainer;
	}

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	


	glutSwapBuffers();

	flushRenderData();

	CHECK_GL_ERROR;
	
	//glutReportErrors();
}


//================================================================================================================


void GLRenderer::reshape(int width, int height) const
{
	float aspect	= float(width) / float(height);
	for( kiss32 i = 0; i < CameraManager::getNumCameras(); ++i )
	{
		Camera* cam	= CameraManager::getCamera(i);

		cam->setAspectRatio( aspect );
		cam->refreshProjectionMatrix();
	}
	glViewport(0, 0, width, height);
}


//================================================================================================================

void GLRenderer::render_callback()
{
	Service<GLRenderer>::Get()->render();
}


//================================================================================================================

void GLRenderer::reshape_callback(int width, int height)
{
	Service<GLRenderer>::Get()->reshape(width, height);
}


//================================================================================================================