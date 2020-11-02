#pragma once
#include "Ship.h"
//Ship that can be steered

class Player :public Ship
{
public:
	Player();

	void Build() override;

	bool OnUpdate(float dt) override;

	void Move(float i);
	void ResetMovement();

	void TakeDamage(const int& damage) override;
};