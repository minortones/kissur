#include "PhysicsObject.h"
#include "PhysicsManager.h"
#include "../AppLayer/Service.h"


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
	_worldMatrix.Transform(_collisionHull.pos);
}

void PhysicsObject::update(Vector3& disp)
{
	_collisionHull.pos += disp;
	_worldMatrix.Transform(_collisionHull.pos);
}
