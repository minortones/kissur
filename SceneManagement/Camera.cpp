#include "Camera.h"
#include "Macros.h"
#include "../AppLayer/InputListener.h"


Camera::Camera(void) : mNear(1), mFar(90000), mAspectRatio(0.75f), mFOV(60.0f)
{
	mLookAt		= VECTOR3(0,0,0);
	mPosition	= VECTOR3(0, 0, 30);
}


void Camera::refreshProjectionMatrix()
{
	buildProjectionMatrix( mFOV, mAspectRatio, mNear, mFar, mProjection.m[0] );
}


Camera::~Camera(void)
{
}

void Camera::update()
{
	kissU32 downKey = InputListener::getKeyDown();
	float speed = 10.3f;

	if( downKey & KEYPRESS_UP )
		mPosition.z -= speed;

	if ( downKey & KEYPRESS_DOWN )
		mPosition.z += speed;

	if ( downKey & KEYPRESS_LEFT )
		mPosition.x += speed;
		
	if ( downKey & KEYPRESS_RIGHT )
		mPosition.x -= speed;


	buildViewMatrix(mPosition.x, mPosition.y, mPosition.z,
					mLookAt.x, mLookAt.y, mLookAt.z,
					0, 1, 0,
					mView.m[0] );
}






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Camera*					CameraManager::mMainCamera	= NULL;
std::vector<Camera*>	CameraManager::mCameras		= std::vector<Camera*>();



Camera* CameraManager::createCamera()
{
	mMainCamera = new Camera();
	mCameras.push_back(mMainCamera);
	return mMainCamera;

}


Camera* CameraManager::getMainCamera()
{
	if (!mMainCamera && mCameras.empty())
	{
		return createCamera();
	}
	return mMainCamera;
}

void CameraManager::update(bool update_all_cams)
{
	if (update_all_cams)
	{
		for ( kissU32 i = 0; i < mCameras.size(); i++  )
		{
			mCameras[i]->update();
		}
	}
	else if (mMainCamera)
		mMainCamera->update();
}

void CameraManager::destroy()
{
	for (kissU32 i = 0; i < mCameras.size(); i++)
	{
		SAFE_DELETE(mCameras[i]);
	}
	mCameras.empty();
}