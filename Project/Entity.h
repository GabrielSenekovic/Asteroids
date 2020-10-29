#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Macros.h"
#include <cmath>
#include "MatrixMath.h"
#include <random>

using namespace MMath;

class Entity
{
public:
	enum class EntityType
	{
		Ship,
		PerfectSphere,
		ImperfectSphere
	};
	std::vector<float> position;
	std::vector<float> direction;
	float turnSpeed;
	float acc = 0;
	int life;

private:
	std::vector<std::vector<float>> vertex;
	std::vector<int> color;

	float scale;
	int drawMethod;

	float speed;
	std::vector<float> velocity;
	float accForce; int accCap = 50;

	int stacks;
	int sectors;

	EntityType type;

	std::random_device rd;

public:
	Entity(EntityType type);
	void BuildSphere();

	void UpdateVelocity();

	void Update(float dt);
	void CheckWorldBounds(int i);
	void Move(float i);
	void Rotate(float i, int axis);

	void Draw();
};

#endif