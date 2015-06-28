#ifndef CAMERA_H
#define CAMERA_H

/************************************************************************/
//	Basic Camera Class and Static Camera Manager class.
//	Currently only points at origin.
//	Would need extending via Interfaces to support various kinds of cam.
/************************************************************************/

#include "Array.h"
#include "Structs.h"
#include "Matrix4x4.h"

class Camera
{
public:
	Camera(void);
	~Camera(void);

	const VECTOR3&		getPosition() const				{ return mPosition; }

	void				setPosition(VECTOR3& pos)		{ mPosition = pos; }

	const VECTOR3&		getLookAt()	const				{ return mLookAt; }

	void				setLookAt(VECTOR3& look)		{ mLookAt = look; }

	const Matrix4x4&	getWorld()	const				{ return mWorld; }

	const Matrix4x4&	getView()	const				{ return mView; }

	const Matrix4x4&	getProjection()	const			{ return mProjection; }

	void		setFOV(float fov)				{ mFOV = fov; }

	float		getFOV()	const				{ return mFOV; }

	void		setAspectRatio(float ratio)		{ mAspectRatio = ratio; }

	float		getAspectRatio()	const		{ return mAspectRatio; }

	void		setNearClipPlane(float zNear)	{ mNear = zNear; }

	float		getNearClipPlane()	const		{ return mNear; }

	void		setFarClipPlane(float zFar)		{ mFar = zFar; }

	float		getFarClipPlane()	const		{ return mFar; }

	void		refreshProjectionMatrix();

	void		update();

private:

	VECTOR3		mLookAt;

	VECTOR3		mPosition;

	Matrix4x4	mWorld;

	Matrix4x4	mView;

	Matrix4x4	mProjection;

	float		mFOV;

	float		mAspectRatio;

	float		mNear;

	float		mFar;
};


class CameraManager
{
public:

	static Camera*	getMainCamera();

	static Camera*	createCamera();

	static kiss32	getNumCameras()			{ return mCameras.size(); }

	static Camera*	getCamera( kiss32 i )	{ return mCameras[i];	}

	static void		update(bool update_all_cams);

	static void		destroy();

private:

	static Camera*				mMainCamera;

	static ks::Array<Camera*>	mCameras;
};

#endif