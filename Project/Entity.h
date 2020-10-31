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
		PlayerShip,
		PerfectSphere,
		ImperfectSphere,
		Laser
	};
	std::vector<float> position;
	std::vector<float> direction;

	float turnSpeed; float rotationAngle = 0;
	float acc = 0;
	int life;
	bool active = true;
	std::vector<float> velocity;
	float speed;

protected:

	struct Face
	{
		std::vector<std::vector<float>*> vertex; //list of vectors 
		std::vector<float> normal;
		int drawMethod;
		std::vector<int> color;

		Face(std::vector<std::vector<float>*>& a, std::vector<int>& color, int drawMethod);
	};

	std::vector<std::vector<float>> vertex;
	std::vector<Face> faces;

	float scale;

	EntityType type;

	std::random_device rd;

public:
	Entity();
	Entity(EntityType type);
	virtual void Build();

	void Update(float dt); virtual void OnUpdate(float dt);
	void CheckWorldBounds(int i);
	virtual void Rotate(float i, int axis);

	virtual void Draw();
};

#endif