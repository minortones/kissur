
/*
	Nnanna Kama
	OpenGL app interface implementation
	Can be overloaded in derived class
*/

#include "Macros.h"
#include "GLApplication.h"
#include "InputListener.h"
#include <SceneObject.h>
#include <SceneManagement/Camera.h>
#include <RenderEngine/GLRenderer.h>
#include <Physics Subsystem/PhysicsManager.h>
#include <Particle Subsystem/ParticleSystem.h>
#include <FX\Particles.h>
#include <chrono>


using namespace std::chrono;

//////////////////////////////////////////////////////////////////////////

bool GLApplication::isRunning				= false;

time_point<system_clock> gStartTime, gMRUTime;

void initTimer()
{
	gStartTime = gMRUTime = system_clock::now();
}

double getElapsedTimeS()
{
	auto end				= system_clock::now();
	duration<double> secs	= end - gMRUTime;
	gMRUTime				= end;
	
	return secs.count();
}



//================================================================================================================

GLApplication::GLApplication(void) : mIsFullscreen(false), mRenderer(NULL), mRegisteredItemsCount(0), mElapsedS(0)
{
	strcpy_s( mAppName, sizeof(mAppName), "Simple App" );
}

//================================================================================================================

GLApplication::~GLApplication(void)
{
	quit();
}


//================================================================================================================


bool GLApplication::init(int argc, char** argv)
{
	if( isRunning )
		return true;

	glutInit(&argc, argv);
	glutInitWindowSize(600, 400);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	glutCreateWindow(mAppName);
	glutDisplayFunc(GLRenderer::render_callback);
	glutReshapeFunc(GLRenderer::reshape_callback);
	glutKeyboardFunc(InputListener::KeyDownCallback);
	glutKeyboardUpFunc(InputListener::KeyUpCallback);
	glutTimerFunc( 1, GLApplication::update_callback, 1 );

	glClearColor(0.5f, 0.5f, 0.5f, 0.f);  // Gray background.
	glEnable(GL_DEPTH_TEST);


	mRenderer = new GLRenderer();
	Service<GLRenderer>::Register( mRenderer );

	//glutCreateMenu(menu);
	//glutAddMenuEntry("[ ] Animate", ' ');
	//glutAttachMenu(GLUT_RIGHT_BUTTON);

	SceneObject* floor = new SceneObject();
	{
		floor->initMaterial(gUnlitShaderFilename, gUnlitVertProgram, gUnlitFragProgram);
		floor->loadQuad(1.f);
		Matrix4x4 rot(Matrix4x4::IDENTITY), scale(Matrix4x4::IDENTITY), trans;
		rot.SetRotateX(DEG_TO_RAD(90.f));
		float scaling = 401.f;
		scale.SetScaling(scaling);
		scale.m24 = scaling - 0.1f;		// offset the y-axis slightly below 0

		trans = scale * rot;
		floor->setMatrix(trans);
	}

	SceneObject* cube = new SceneObject();
	{
		cube->initMaterial(gDefaultShaderFilename, gDefaultVertProgram, gDefaultFragProgram);
		cube->loadCube(1.5f);
		cube->getMatrix().m24 += 1.4f;
	}

	ParticleSystem* pSys = new ParticleSystem();
	{
		pSys->initShader(gDefaultShaderFilename, gDefaultVertProgram, gDefaultFragProgram);

		ks::Emitter emz;
		emz.mWorldPos			= VECTOR3(-2.3f, 9.4f, 0.f);
		emz.mEmissionVelocity	= VECTOR3(2, 0, 0);
		emz.mMaxParticles		= 3200;
		emz.mEmissionRate		= 30;

		pSys->spawn(emz);
	}

	//sceneObj->loadModel("..\\media\\models\\venusm.obj");
	//sceneObj->loadModel("..\\media\\models\\crysponza_bubbles\\sponza.obj");

	initTimer();

	return (isRunning = true);
}


//================================================================================================================

void GLApplication::destroy(kiss32 exit_value_or_whatever_this_crap_is)
{
	Service<GLApplication>::Destroy();
	exit(0);
}


//================================================================================================================

template<>
ks::Array<SceneObject*>& GLApplication::getObjectCollection()
{
	return mSceneObjects;
}

template<>
ks::Array<ParticleSystem*>& GLApplication::getObjectCollection()
{
	return mParticleSubsytems;
}



void GLApplication::update(kiss32 pCallbackID)
{
	mElapsedS = getElapsedTimeS();

	for (size_t i = 0; i < mSceneObjects.size(); i++)
	{
		mSceneObjects[i]->update(mElapsedS);
	}

	//
	// update particle system. TODO: Jobify
	//
	for( size_t i = 0; i < mParticleSubsytems.size(); ++i )
	{
		mParticleSubsytems[i]->step(mElapsedS);
	}

	CameraManager::update(false);

	mRenderer->update(mElapsedS);
	
	glutPostRedisplay();

	//printf ("frame time = %3.5f sec\n", mElapsedS);
}


//================================================================================================================



void GLApplication::go()
{
	
	glutMainLoop();
	/*mHWND = hwnd;

	if(!init(mHWND))
		return;

	while(isRunning)
	{
		update();
		render();
	}*/
}


//================================================================================================================


void GLApplication::quit()
{
	isRunning = false;

	for(size_t i = 0; i < mParticleSubsytems.size(); i++)
	{
		delete mParticleSubsytems[i];
	}
	mParticleSubsytems.clear();
	
	for (size_t i = 0; i < mSceneObjects.size(); i++)
	{
		delete mSceneObjects[i];
	}
	mSceneObjects.clear();

	CameraManager::destroy();

	delete mRenderer;
}


//================================================================================================================

void GLApplication::update_callback(kiss32 pCallbackID)
{
	if (isRunning)
	{
		Service<GLApplication>::Get()->update(pCallbackID);
		glutTimerFunc(1, GLApplication::update_callback, pCallbackID);
	}
}


//================================================================================================================
