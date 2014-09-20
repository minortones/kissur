
#include "PhysicsObject.h"
#include "PhysicsManager.h"
#include "../AppLayer/Service.h"


PhysicsManager::PhysicsManager(void)
{
	_physicsObjects = new PhysicsObjectArray();
}


PhysicsManager::~PhysicsManager(void)
{
}

void PhysicsManager::update(	void* pParams,
								int iContext,
								unsigned int uTaskId,
								unsigned int uTaskCount)
{
	Service<PhysicsManager>::Get()->_update(pParams, iContext, uTaskId, uTaskCount);
}

void PhysicsManager::_update(	void* pParams,
								int iContext,
								unsigned int uTaskId,
								unsigned int uTaskCount)
{
	int totalObjects = _physicsObjects->size();
	
	// TODO: could potentially use "parrallel_for"
	for (int i=0; i<totalObjects; i++)
	{
		for (int j=i+1; j<totalObjects; j++)
		{
			bool collided = broadPhaseCollision((*_physicsObjects)[i]->GetCollisionHull(), (*_physicsObjects)[j]->GetCollisionHull());
			
			// TODO: branch to separate thread
			if(collided)
			{
				//TODO: compute collisionParams (force, vel, direction, etc)
			}
		}
		(*_physicsObjects)[i]->update();
	}


	//printf("PhysicsManager total objects = %.1d\n", totalObjects);
}

void PhysicsManager::Register(PhysicsObjectType* physicsObject)
{
	_physicsObjects->push_back( physicsObject );
}


bool PhysicsManager::broadPhaseCollision(BoundingSphere& s1, BoundingSphere& s2)
{
	Vector3 relPos	= s1.pos - s2.pos;
	float dist		= relPos.x * relPos.x + relPos.y * relPos.y + relPos.z * relPos.z;
	float minDist	= s1.radius + s2.radius;
	
	return dist <= minDist * minDist;
}
