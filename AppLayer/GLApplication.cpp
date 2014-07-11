
/*
	Nnanna Kama
	OpenGL app interface implementation
	Can be overloaded in derived class
*/

#include "Macros.h"
#include "TaskMgrTBB.h"
#include "GLApplication.h"
#include "InputListener.h"
#include "../SceneObject.h"
#include "../SceneManagement/Camera.h"
#include "../RenderEngine/GLRenderer.h"
#include "../Physics Subsystem/PhysicsManager.h"
#include "../Particle Subsystem/ParticleSystem.h"


bool GLApplication::isRunning				= false;

TASKSETHANDLE g_pUpdatePhysicsTaskHandle	= TASKSETHANDLE_INVALID;


//////////////////////////////////////////////////////////////////////////


GLuint m_glTimerQuery;
double m_timer = 0;

#ifdef TARGET_GL_SHADERS
void initTimer()
{
	glGenQueries(1, &m_glTimerQuery);
	glBeginQuery(GL_TIME_ELAPSED_EXT, m_glTimerQuery);
}

double getElapsedTime()
{
	glEndQuery(GL_TIME_ELAPSED_EXT);
	GLuint64 timeElapsed;
	glGetQueryObjectui64vEXT(m_glTimerQuery, GL_QUERY_RESULT, &timeElapsed);
	// accumulate time elapsed thus far
	m_timer += double(timeElapsed) / 1e9;

	// restart timer so we can ask again...
	glBeginQuery(GL_TIME_ELAPSED_EXT, m_glTimerQuery);

	return m_timer;
}
#else
void initTimer()			{}
double getElapsedTime()		{ return 0; }
#endif



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

	glClearColor(0.1, 0.1, 0.1, 0);  // Gray background.
	glEnable(GL_DEPTH_TEST);         // Hidden surface removal.


	mRenderer = new GLRenderer();
	mRenderer->init();
	Service<GLRenderer>::Register( mRenderer );

	//glutCreateMenu(menu);
	//glutAddMenuEntry("[ ] Animate", ' ');
	//glutAttachMenu(GLUT_RIGHT_BUTTON);

	SceneObject* sphere = new SceneObject();
	sphere->initMaterial(gDefaultShaderFilename, gDefaultVertProgram, gDefaultFragProgram);
	sphere->loadModel("..\\media\\models\\venusm.obj");
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
	/*
	do some general updating...
	*/
	const double elapsed	= getElapsedTime();
	const double delta		= elapsed - mElapsedS;
	mElapsedS				= elapsed;

	//
	// run physics on separate thread
	//
	/*gTaskMgr.CreateTaskSet(	PhysicsManager::update, 
							NULL,
							1,
							NULL,
							NULL,
							"Physics task",
							&g_pUpdatePhysicsTaskHandle);

	gTaskMgr.WaitForSet(g_pUpdatePhysicsTaskHandle);
	gTaskMgr.ReleaseHandle(g_pUpdatePhysicsTaskHandle);
	g_pUpdatePhysicsTaskHandle = TASKSETHANDLE_INVALID;*/


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


	//tbb::tick_count t1 = tbb::tick_count::now();

	//printf ("frame time = %3.5f sec\n", delta);
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

	// shutdown all threads
	gTaskMgr.Shutdown();

	exit(0);
}


//================================================================================================================

void GLApplication::update_callback(int val)
{
	Service<GLApplication>::Get()->update(val);
}


//================================================================================================================
