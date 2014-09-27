#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Matrix4x4.h"

// Forward Declarations
namespace kiss
{
	class Model;
}
class RenderData;
class Matrix4x4;
class PhysicsObject;
struct Material;



class SceneObject
{
public:
	SceneObject(void);

	~SceneObject(void);

	virtual void VRegister(void);

	virtual void update();

	virtual void loadModel(const char* filepath);
	virtual void loadCube( float pSize );
	virtual void loadQuad(float pSize);

	virtual void initMaterial( const char* filename, const char* vp_entry, const char* fp_entry );

	virtual void setRenderStates(void);


	//
	// inlines
	//
	PhysicsObject*	getRigidBody()		{return mRigidBody;}

	kiss::Model*	getModel()			{return mModel;}

	kiss32			getUID() const		{ return mUID; }

	const Matrix4x4& getMatrix() const	{ return mWorld; }


private:

	void				fillRenderData();

	PhysicsObject*		mRigidBody;

	kiss::Model*		mModel;

	Material*			mMaterial;

	RenderData*			mRenderData;

	int					mUID;					// this should ideally be constant, right? err, whatever.

	Matrix4x4			mWorld;
};


#endif