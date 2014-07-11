#pragma once

#include "..\shared_include\Structs.h"
#include "..\shared_include\Matrix4x4.h"

typedef VECTOR3		Vector3;
typedef Matrix4x4	Matrix;

class PhysicsObject
{
public:
	PhysicsObject(void);

	virtual ~PhysicsObject(void);

	virtual void update();

	virtual void update(Vector3& displacement);

	Matrix& GetWorldMatrix() {return _worldMatrix;}
	
	Vector3& GetPosition() {return _collisionHull.pos;}

	BoundingSphere& GetCollisionHull() {return _collisionHull;}

	void SetPosition(const Vector3& pos) {_collisionHull.pos = pos;}

private:
	float			_mass;					// redundant
	Matrix			_worldMatrix;
	BoundingSphere	_collisionHull;
};

