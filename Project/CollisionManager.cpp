#include "CollisionManager.h"

bool CollisionManager::CheckCollision(const Entity* a, const Entity* b)
{
	int mask = a->col->flag | b->col->flag;
	switch (mask)
	{
	case Collider::COLLIDER_TYPE::LINE: //line vs line
		break;
	case Collider::COLLIDER_TYPE::SPHERE: //sphere vs sphere
		return SphereVSSphere
		(static_cast<SphereCollider*>(a->col), static_cast<SphereCollider*>(b->col));
	case Collider::COLLIDER_TYPE::LINE | Collider::COLLIDER_TYPE::SPHERE: //sphere vs line
		return SphereVSLine
		(
			static_cast<SphereCollider*>(a->col->flag == Collider::COLLIDER_TYPE::SPHERE? a->col : b->col), 
			static_cast<LineCollider*>(b->col->flag == Collider::COLLIDER_TYPE::LINE ? b->col : a->col),
			b->col->flag == Collider::COLLIDER_TYPE::LINE ? b->direction : a->direction
		);
	}
	
	return false;
}
bool CollisionManager::SphereVSSphere(const SphereCollider* a, const SphereCollider* b)
{
	float distance = MMath::Magnitude(MMath::MatrixSubtraction(a->position, b->position), 3);
	return distance < a->r * a->scale + b->r * b->scale;
}
bool CollisionManager::SphereVSLine(const SphereCollider* a, const LineCollider* b, const std::array<float, 3>& direction)
{
	//Couldnt get it to work, worked on it for hours


	/*std::array<float, 3> line_start = MMath::MatrixAddition(b->s, b->position);
	std::array<float, 3> line_end = MMath::MatrixAddition(b->e, b->position);
	std::array<float, 3> center = MMath::MatrixAddition(a->c, a->position);
	std::array<float, 3> vectorFromStartToCenter = MMath::MatrixSubtraction(line_start, center);
	
	//d^2 - 4ac       d = 2D(O-C)     a = 1      c = |O- C|^2 - r^2
	float d = MMath::Dot(direction, vectorFromStartToCenter)* 2;
	float c = std::pow(MMath::Magnitude(vectorFromStartToCenter, 3), 2) - std::pow(a->r *2, 2);
	float discriminant = std::pow(d, 2) - 4 * c;

	std::cout << discriminant << std::endl;

	if (discriminant >= 0)
	{
		return true;
	}*/
	return false;
}