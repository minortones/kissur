#ifndef GL_RENDENDER
#define GL_RENDENDER

/*
	Nnanna Kama
	Singleton Render class
	Renderer requires cgToolkit 3.0 to run. TODO: add as error msg.
*/

#include <vector>
#include "RenderResourceFactory.h"
#include "Material.h"
#include "Matrix4x4.h"

#ifdef TARGET_CG_SHADERS
#include "glut.h"
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif



class Camera;
class RenderData;
class ParticleSystem;
class SimpleShaderContainer;

typedef std::vector<const RenderData*>	RenderDataArray;

class GLRenderer
{
public:

	GLRenderer(void);

	virtual ~GLRenderer(void);

	static  void				render_callback(void);

	static	void				reshape_callback(int width, int height);

	virtual	void	init();

	virtual void	render();

	virtual void	reshape(int width, int height) const;

	virtual void	update(int );

	virtual void	addRenderData( const RenderData* r );

	virtual void	flushRenderData( void );


private:

	void			uploadShaderConstants( Material* pMat, const Camera* pCam, const Matrix4x4& pTransform );

	Material*		mDefaultMaterial;

	SimpleShaderContainer* mMRUShader;					// most recently used shader.

	
	/*
		data: array of vertex & index buffers for rendering
	*/
	RenderDataArray		mRenderData;

};


static float myLightAngle = -0.4f;   /* Angle light rotates around scene. */


template<typename T>	kissU32 getGLType(const T* pT);

#define GET_GLTYPE(x)	getGLType(x)

#endif