#pragma once
#include "Entity.h"
#include <iostream>
//A space body, ie asteroid, planet, cloud, etc
class Body : public  Entity
{
	int stacks;
	int sectors;

public:
	Body();
	Body(EntityType type, int stacks, int sectors);

	void Build() override;
	void OnCollide(Entity& other) override;
	void Draw() override;
};

class Asteroid : public Body
{
	int divisions;
	std::array<float, 3> rotationDirections;

public:
	std::vector<Asteroid> children;
	Asteroid(const Asteroid& asteroid);
	Asteroid(const int& divisions_in);

	bool OnUpdate(float dt) override;

	void AddToEntitiesList(std::vector<Entity*>& entities) override;

	void TakeDamage(const int& damage) override;
};