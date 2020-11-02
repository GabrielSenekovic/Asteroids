#include "Projectile.h"
Projectile::Projectile()
{

}
Projectile::Projectile(const Projectile& a):Projectile(a.type, a.lifeTimerLimit)
{

}
Projectile::Projectile(EntityType type, int lifeTime) : Entity(type), lifeTimerLimit(lifeTime), lifeTimer(lifeTime)
{
}

bool Projectile::OnUpdate(float dt)
{
	if (lifeTimer < lifeTimerLimit)
	{
		lifeTimer += dt;
	}
	else
	{
		active = false;
		lifeTimer = 0;
		RotateStart(-rotationAngle, 1);
	}
	return true;
}

void Projectile::RotateStart(float i, int axis)
{
	rotationAngle = i;
	std::vector<float> R = MMath::GetRotationMatrix(1, i);
	direction = MMath::MatrixMultiplication(direction, R, 1, 3);
	for (int i = 0; i < vertex.size(); i++)
	{
		vertex[i] = MMath::MatrixMultiplication(vertex[i], R, 1, 3);
	}
}
void Projectile::Draw()
{
	glLineWidth(5);
	Entity::Draw();
}