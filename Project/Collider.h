#ifndef COLLIDER_H
#define COLLIDER_H

#include <array>

struct Collider
{
	std::array<float, 3> position;
	enum COLLIDER_TYPE
	{
		LINE = 1,
		SPHERE = 1 << 1
	};
	COLLIDER_TYPE flag;
};
struct LineCollider :public Collider
{
	std::array<float, 3> s;
	std::array<float, 3> e;
	LineCollider(const std::array<float, 3>& s_in, const std::array<float, 3>& e_in);
};
struct SphereCollider :public Collider
{
	float r; //radius
	SphereCollider(const float &r_in);
};
#endif