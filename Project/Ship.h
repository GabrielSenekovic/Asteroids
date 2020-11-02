#pragma once
#include "Entity.h"
#include "Projectile.h"
//Entity that can hold projectiles, turn and stuff
class Ship:public Entity
{
protected:
	float accForce; int accCap = 50;
	float shootTimerLimit = 20;
	float shootTimer = 0;

	int projectileIndex = 0;

public:
	std::vector<Projectile> projectiles;

	Ship(EntityType type);

	bool OnUpdate(float dt) override;

	void Build() override;

	void Rotate(float i, int axis);
	void Shoot();
	void UpdateVelocity();

	void Draw() override;
};