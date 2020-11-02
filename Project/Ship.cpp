#include "Ship.h"

Ship::Ship(EntityType type): Entity(type), accForce(0)
{
	Build();
	shootTimer = shootTimerLimit;
}

void Ship::Build()
{
}

bool Ship::OnUpdate(float dt)
{
	if (shootTimer < shootTimerLimit)
	{
		shootTimer += dt;
	}
	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i].Update(dt);
	}
	return true;
}

void Ship::Rotate(float i, int axis)
{
	float j = turnSpeed * i; rotationAngle += j;

	std::vector<float> R = MMath::GetRotationMatrix(axis, j);
	direction = MMath::MatrixMultiplication(direction, R, 1, 3);
	UpdateVelocity();
	for (int i = 0; i < vertex.size(); i++)
	{
		vertex[i] = MMath::MatrixMultiplication(vertex[i], R, 1, 3);
	}
}
void Ship::UpdateVelocity()
{
	for (int j = 0; j < 3; j++)
	{
		velocity[j] = direction[j] * speed;
	}
}
void Ship::Shoot()
{
	if (shootTimer >= shootTimerLimit)
	{
		if (!projectiles[projectileIndex].active)
		{
			projectiles[projectileIndex].active = true;
			projectiles[projectileIndex].position = position;
			projectiles[projectileIndex].direction = direction;
			projectiles[projectileIndex].RotateStart(rotationAngle, 1);
			for (int i = 0; i < projectiles[projectileIndex].velocity.size(); i++)
			{
				projectiles[projectileIndex].velocity[i] = velocity[i] * acc + direction[i] * projectiles[projectileIndex].speed;
			}
			projectiles[projectileIndex].lifeTimer = 0;
			shootTimer = 0;
			projectileIndex++; projectileIndex %= projectiles.size();
		}
	}
}
void Ship::Draw()
{
	Entity::Draw();
	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i].Draw();
	}
}