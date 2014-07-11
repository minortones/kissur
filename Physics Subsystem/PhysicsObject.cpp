#include "PhysicsObject.h"
#include "PhysicsManager.h"
#include "tbb\spin_mutex.h"
#include "../AppLayer/Service.h"

typedef tbb::spin_mutex MutexType;
MutexType updateMutex;


PhysicsObject::PhysicsObject(void)
{
	// auto register with the manager.
	Service<PhysicsManager>::Get()->Register( this );
}


PhysicsObject::~PhysicsObject(void)
{
}

void PhysicsObject::update()
{
	MutexType::scoped_lock lock;
	lock.acquire(updateMutex);

	_worldMatrix.Transform(_collisionHull.pos);

	lock.release();
}

void PhysicsObject::update(Vector3& disp)
{
	MutexType::scoped_lock lock;
	lock.acquire(updateMutex);

	_collisionHull.pos += disp;
	_worldMatrix.Transform(_collisionHull.pos);

	lock.release();
}
