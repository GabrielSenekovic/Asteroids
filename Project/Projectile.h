#pragma once
#include "Entity.h"

class Projectile : public Entity
{
public:
	int lifeTimerLimit;
	float lifeTimer;

	Projectile();
	Projectile(const Projectile&);
	Projectile(EntityType type, int lifeTime);

	bool OnUpdate(float dt) override;

	void RotateStart(float i, int axis);
	void Draw() override;
};