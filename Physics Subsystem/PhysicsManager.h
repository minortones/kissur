#pragma once

#include <Array.h>

class PhysicsObject;
struct BoundingSphere;

typedef PhysicsObject					PhysicsObjectType;
typedef ks::Array<PhysicsObjectType*>	PhysicsObjectArray;



class PhysicsManager
{
public:

	PhysicsManager(void);
	~PhysicsManager(void);

	// callback to the relevant internal instance of update.
	static void update(void* pParams, int iContext, unsigned int uTaskId, unsigned int uTaskCount);
	
	void Register(PhysicsObjectType* physicsObject);


private:
	PhysicsObjectArray* _physicsObjects;

	bool broadPhaseCollision(BoundingSphere& s1, BoundingSphere& s2);

	// internal update call. do not call externally
	void _update(void* pParams, int iContext, unsigned int uTaskId, unsigned int uTaskCount);
};

