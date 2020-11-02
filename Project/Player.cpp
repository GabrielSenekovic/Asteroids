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
	//lower side
	vertices.push_back({ 1,-0.5f,-1.5f });
	vertices.push_back({ -1,-0.5f,-1.5f });
	vertices.push_back({ 0,-0.5f,3 });
	//upper sidedd
	vertices.push_back({ 1,0.5f,-1.5f });
	vertices.push_back({ -1,0.5f,-1.5f });
	vertices.push_back({ 0,0.5f,3 }); 

	vertices.push_back({ 1 * 1.5f,0,-1.5f * 1.5f }); //6
	vertices.push_back({ -1 * 1.5f,0,-1.5f * 1.5f }); //8
	vertices.push_back({ 0,0,3 }); //9

	std::vector<std::array<float, 3>*> temp = { &vertices[0],&vertices[1],&vertices[2] };
	std::vector<int> color = { 255, 0, 0 };
	faces.push_back(Face(temp, color, GL_TRIANGLES));

	temp = { &vertices[3],&vertices[4],&vertices[5] };
	color = { 0, 255, 0 };
	faces.push_back(Face(temp, color, GL_TRIANGLES));

	temp.push_back(0);
	temp = { &vertices[0],&vertices[6],&vertices[8], &vertices[2]};
	color = { 0, 0, 255 };
	faces.push_back(Face(temp, color,GL_QUADS));

	temp = { &vertices[3],&vertices[6],&vertices[8], &vertices[5] };
	faces.push_back(Face(temp, color, GL_QUADS));

	temp = { &vertices[4],&vertices[7],&vertices[8], &vertices[5] };
	faces.push_back(Face(temp, color, GL_QUADS));

	temp = { &vertices[1],&vertices[7],&vertices[8], &vertices[2] };
	faces.push_back(Face(temp, color, GL_QUADS));

	temp = { &vertices[0],&vertices[6],&vertices[7], &vertices[1] };
	faces.push_back(Face(temp, color, GL_QUADS));

	temp = { &vertices[3],&vertices[6],&vertices[7], &vertices[4] };
	faces.push_back(Face(temp, color, GL_QUADS));
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

void Player::ResetMovement()
{
	position = { 0,0,0 };
	acc = 0;
	invincibilityFrame = 0;
}

void Player::TakeDamage(const int& damage)
{
	if (!IsVulnerable()) { return; }
	life -= damage;
	ResetMovement();
}