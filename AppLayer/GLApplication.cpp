
/*
	Nnanna Kama
	OpenGL app interface implementation
	Can be overloaded in derived class
*/

#include "Macros.h"
#include "GLApplication.h"
#include "InputListener.h"
#include "../SceneObject.h"
#include "../SceneManagement/Camera.h"
#include "../RenderEngine/GLRenderer.h"
#include "../Physics Subsystem/PhysicsManager.h"
#include "../Particle Subsystem/ParticleSystem.h"
#include <chrono>


using namespace std::chrono;

//////////////////////////////////////////////////////////////////////////

bool GLApplication::isRunning				= false;

time_point<system_clock> gStartTime, gMRUTime;

void initTimer()
{
	gStartTime = gMRUTime = system_clock::now();
}

double getElapsedTime()
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
	glutInitWindowSize(400, 400);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	glutCreateWindow(mAppName);
	glutDisplayFunc(GLRenderer::render_callback);
	glutReshapeFunc(GLRenderer::reshape_callback);
	glutKeyboardFunc(InputListener::KeyDownCallback);
	glutKeyboardUpFunc(InputListener::KeyUpCallback);
	glutTimerFunc( 33, GLApplication::update_callback, 1 );

	glClearColor(0.5f, 0.5f, 0.5f, 0.f);  // Gray background.
	glEnable(GL_DEPTH_TEST);         // Hidden surface removal.


	mRenderer = new GLRenderer();
	mRenderer->init();
	Service<GLRenderer>::Register( mRenderer );

	//glutCreateMenu(menu);
	//glutAddMenuEntry("[ ] Animate", ' ');
	//glutAttachMenu(GLUT_RIGHT_BUTTON);

	SceneObject* sphere = new SceneObject();
	sphere->initMaterial(gDefaultShaderFilename, gDefaultVertProgram, gDefaultFragProgram);
	sphere->loadCube(1.5f);
	//sphere->loadModel("..\\media\\models\\venusm.obj");
	//sphere->loadModel("..\\media\\models\\crysponza_bubbles\\sponza.obj");

	initTimer();

	return (isRunning = true);
}


//================================================================================================================

void GLApplication::destroy(int exit_value_or_whatever_this_crap_is)
{
	Service<GLApplication>::Destroy();
}


//================================================================================================================

template<>
std::vector<SceneObject*>& GLApplication::getObjectCollection()
{
	return mSceneObjects;
}

template<>
std::vector<ParticleSystem*>& GLApplication::getObjectCollection()
{
	return mParticleSubsytems;
}



void GLApplication::update(int val)
{
	for (size_t i = 0; i < mSceneObjects.size(); i++)
	{
		mSceneObjects[i]->update();
	}

	//
	// update particle system. TODO: run on separate thread
	//
	for( size_t i = 0; i < mParticleSubsytems.size(); ++i )
	{
		mParticleSubsytems[i]->step();
	}

	CameraManager::update(false);

	mRenderer->update(1);
	
	glutPostRedisplay();
	glutTimerFunc(33, GLApplication::update_callback, 1);

	//printf ("frame time = %3.5f sec\n", getElapsedTime());
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

	exit(0);
}


//================================================================================================================

void GLApplication::update_callback(int val)
{
	Service<GLApplication>::Get()->update(val);
}


//================================================================================================================
