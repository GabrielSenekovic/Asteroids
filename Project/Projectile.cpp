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
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = MMath::MatrixMultiplication(vertices[i], R, 1, 3);
	}
}
void Projectile::Draw()
{
	glDisable(GL_LIGHTING);
	glLineWidth(5);
	Entity::Draw();
	glEnable(GL_LIGHTING);
}