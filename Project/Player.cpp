#include "Player.h"

Player::Player():Ship(EntityType::PlayerShip)
{
	accForce = 0.3f;
	Build();
	for (int i = 0; i < 10; i++)
	{
		projectiles.push_back(Projectile(EntityType::Laser, 5));
	}
	shootTimerLimit = (float)projectiles[0].lifeTimerLimit / projectiles.size();
	col = new SphereCollider(0.7f);
}

void Player::Build()
{
	vertex.push_back({ 1,0,-1.5f });
	vertex.push_back({ -1,0,-1.5f });
	vertex.push_back({ 0,0,3 });
	std::vector<std::array<float,3>*> temp = { &vertex[0],&vertex[1],&vertex[2] };
	std::vector<int> color = { 255, 0, 0 };
	faces.push_back(Face(temp, color,GL_TRIANGLES));
}

bool Player::OnUpdate(float dt)
{
	Ship::OnUpdate(dt);
	return life > 0;
}

void Player::Move(float i)
{
	bool isTooFast = abs(acc + accForce * i) > accCap;

	acc = isTooFast ? accCap * i * isTooFast : acc; //If its not too fast, dont touch it

	acc += (EqSgn(acc, i)) ? i * accForce * !isTooFast //if going in the same direction, accelerate
		: i * accForce * 3 * !isTooFast; //if going in the other direction, deaccelerate

	UpdateVelocity();
}

void Player::Reset()
{
	position = { 0,0,0 };
	acc = 0;
	invincibilityFrame = 0;
}

void Player::TakeDamage(const int& damage)
{
	if (!IsVulnerable()) { return; }
	life -= damage;
	Reset();
}