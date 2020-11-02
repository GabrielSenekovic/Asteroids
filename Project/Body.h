#pragma once
#include "Entity.h"
//A space body, ie asteroid, planet, cloud, etc
class Body : public  Entity
{
	int stacks;
	int sectors;

public:

	Body(EntityType type, int stacks, int sectors);

	void Build() override;
	void OnCollide(Entity& other) override;
	void Draw() override;
};