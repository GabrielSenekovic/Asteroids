#include "Collider.h"

LineCollider::LineCollider(const std::array<float, 3>& s_in,const std::array<float, 3>& e_in): s(s_in), e(e_in)
{
	position = { 0,0,0 };
	flag = COLLIDER_TYPE::LINE;
}
SphereCollider::SphereCollider(const float& r_in): r(r_in)
{
	position = { 0,0,0 };
	flag = COLLIDER_TYPE::SPHERE;
}