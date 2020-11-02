#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <array>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Macros.h"
#include <cmath>
#include "MatrixMath.h"
#include <random>
#include "Collider.h"

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
	std::array<float, 3> position;
	std::array<float, 3> direction;

	float turnSpeed; float rotationAngle = 0;
	float acc = 0;
	int life;
	bool active = true;
	std::vector<float> velocity;
	float speed;
	EntityType type;
	Collider* col = nullptr;

protected:

	struct Face
	{
		std::vector<std::array<float, 3>*> vertex; //list of vectors 
		std::vector<float> normal;
		int drawMethod;
		std::vector<int> color;

		Face(std::vector<std::array<float, 3>*>& a, std::vector<int>& color, int drawMethod);
	};

	std::vector<std::array<float, 3>> vertex;
	std::vector<Face> faces;

	float invincibilityFrame = 20;
	float invincibilityFrameLimit = 20;

	float scale;

	std::random_device rd;

public:
	Entity();
	Entity(EntityType type);
	virtual void Build();

	bool Update(float dt); virtual bool OnUpdate(float dt);
	void CheckWorldBounds(int i);
	virtual void Rotate(float i, int axis);

	std::array<float, 3> GetValidStartPosition();

	bool IsVulnerable();

	virtual void OnCollide(Entity& other);
	virtual void TakeDamage(const int& damage);

	virtual void Draw();
};

#endif