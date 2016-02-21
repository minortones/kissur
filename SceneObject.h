#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "defines.h"
#include "Matrix4x4.h"

// Forward Declarations
namespace ks
{
	class Model;
}
class RenderData;
class PhysicsObject;
struct Material;



class SceneObject
{
public:
	SceneObject(void);

	~SceneObject(void);

	virtual void VRegister(void);

	virtual void update(float elapsed);

	virtual void loadModel(const char* filepath);
	virtual void loadCube( float pSize );
	virtual void loadQuad(float pSize);

	virtual void initMaterial( const char* filename, const char* vp_entry, const char* fp_entry );

	virtual void setRenderStates(void);


	//
	// inlines
	//
	PhysicsObject*	getRigidBody()				{return mRigidBody;}

	ks::Model*		getModel()					{return mModel;}

	kiss32			getUID() const				{ return mUID; }

	const Matrix4x4& getMatrix() const			{ return mWorld; }

	Matrix4x4& getMatrix()						{ return mWorld; }

	void setMatrix(const Matrix4x4& pVal)		{ mWorld = pVal; }


private:

	void				fillRenderData();

	PhysicsObject*		mRigidBody;

	ks::Model*			mModel;

	Material*			mMaterial;

	RenderData*			mRenderData;

	int					mUID;					// this should ideally be constant, right? err, whatever.

	Matrix4x4			mWorld;
};


#endif