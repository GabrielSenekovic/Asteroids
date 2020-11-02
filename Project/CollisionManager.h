#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Collider.h"
#include "Entity.h"
#include "MatrixMath.h"
#include <algorithm>
#include <iostream>

class CollisionManager
{
public:
	bool CheckCollision(const Entity* a, const Entity* b);
	bool SphereVSSphere(const SphereCollider* a, const SphereCollider* b);
	bool SphereVSLine(const SphereCollider* a, const LineCollider* b, const std::array<float, 3>& direction);
};

#endif