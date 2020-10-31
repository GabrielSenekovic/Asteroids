#pragma once
#include "Ship.h"
//Ship that can be steered

class Player :public Ship
{
public:
	Player();

	void Build() override;

	void Move(float i);
};