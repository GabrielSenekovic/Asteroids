#include "Entity.h"

Entity::Face::Face(std::vector<std::array<float, 3>*> a, std::vector<int>& color, int drawMethod) :
	vertices(a), drawMethod(drawMethod), color(color)
{
	if (vertices.size() > 2)
	{
		std::array<float, 3> temp1 = MMath::MatrixSubtraction(*vertices[0], *vertices[1]);
		std::array<float, 3> temp2 = MMath::MatrixSubtraction(*vertices[1], *vertices[2]);
		std::array<float, 3> cross = Cross(temp1, temp2);
		normal = Normalize(Cross(temp1, temp2)); 
	}
}

Entity::Entity()
{
	life = 0;
	scale = 0;
	speed = 0;
	turnSpeed = 0;
	type = EntityType::PlayerShip;
}
Entity::Entity(EntityType type_in)
{
	srand(rd());
	direction = { 0,0,1 };
	velocity = {0,0,0};
	type = type_in;
	switch (type_in)
	{
	case EntityType::PlayerShip:
		scale = 1;
		position = { 0,0,0 };
		speed = 2;
		turnSpeed = 1.5f;
		life = 5;
		break;
	case EntityType::PerfectSphere: 
		velocity = { (float)(rand() % 20 + 10), 0, (float)(rand() % 20 + 10) };
		scale = 1;
		//sectors 4 stacks 2 = 8 sided die
		//5,4 also works
		/*(5,3), (3,3), (3,2), (10,9), (4,8)*/
		position = GetValidStartPosition();
			//if you draw normally
			//if you do GL_Polygon it turns into a cool helix kind of a thing
			//if you do GL_TRIANGLES you get a cool windy effect
			//GL_TRIANGLES_STRIP gives like a snaky helix, also a bit windy
		speed = 0;
		turnSpeed = 0;
		life = 10;
		break;
	case EntityType::ImperfectSphere: 
		velocity = { (float)(rand() % 40 -20), 0, (float)(rand() % 40 - 20) };
		scale = 1;
		position = GetValidStartPosition();
		speed = rand() % 3 + 2;
		turnSpeed = 2;
		acc = 1;
		life = 1;
		invincibilityFrameLimit = 2;
		break;
	case EntityType::Laser:
		{
			active = false;
			scale = 1;
			speed = 200;
			vertices.push_back({ 0, 0, 0 });
			vertices.push_back({ 0, 0, 20 });
			position = { 0,0,0 };
			velocity = { 0,0,0 };
			std::vector<std::array<float, 3>*> temp = { &vertices[0],&vertices[1]};
			std::vector<int> color = { 100, 255, 255 };
			faces.push_back(Face(temp, color, GL_LINE_STRIP));
			acc = 1;
			col = new SphereCollider(0.5f);
		}
		break;
	}
}

void Entity::Build()
{
	
}

bool Entity::Update(float dt)
{
	if (!active) { return true; }
	invincibilityFrame += !IsVulnerable() ? dt*6 : 0;
	for (int i = 0; i < 3; i++)
	{
		position[i] += velocity[i] * acc * dt; //Moves ship forward
	}
	if (col != nullptr){col->position = position;}
	return OnUpdate(dt);
}
bool Entity::OnUpdate(float dt)
{
	return true;
}

void Entity::CheckWorldBounds(int i)
{
	for (int j = 0; j < vertices.size(); j++)
	{
		std::array<float, 3> temp = vertices[j];
		for (int k = 0; k < 3; k++)
		{
			temp[k] += position[k];
		}
		if (Magnitude(temp, 3) < i)
		{
			return;
		}
	}
	//If you got this far, then all vertices of this object are out of bounds
	for (int j = 0; j < 3; j++)
	{
		if (position[j] < -i)
		{
			position[j] += i*2;
		}
		if (position[j] > i)
		{
			position[j] -= i*2;
		}
	}
}
std::array<float,3> Entity::GetValidStartPosition()
{
	int w[2] = { -10, 10 };
	float x = rand() % 50 *w[rand()%2];
	float y = 0;
	float z = rand() % 50 *w[rand()%2];
	return { x,y,z };
}

void Entity::Rotate(float i, int axis)
{
	float j = turnSpeed * i; rotationAngle += j;

	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = MMath::MatrixMultiplication(vertices[i], MMath::GetRotationMatrix(axis, j), 1, 3);
	}
}

bool Entity::IsVulnerable()
{
	return invincibilityFrame >= invincibilityFrameLimit;
}

void Entity::Draw()
{
	if (!active) { return; }

	for (int i = 0; i < faces.size(); i++)
	{
		//going through all faces of the model
		glBegin(faces[i].drawMethod);
		(int)invincibilityFrame % 2 == 0 && !IsVulnerable() ? Color(masterColor) : Color(faces[i].color);
		glNormal3f(faces[i].normal[0], faces[i].normal[1], faces[i].normal[2]);
		for (int j = 0; j < faces[i].vertices.size(); j++)
		{
			//going through all vector3 and vector 4 of the face
			std::array<float, 3> temp = *faces[i].vertices[j]; //Get vertices from the face
			for (int k = 0; k < temp.size(); k++)
			{
				temp[k] = temp[k] * scale + position[k]; //Move all variables of the vertex
			}
			glVertex3f(temp[0], temp[1], temp[2]);
		}
		glEnd();
	}
}
void Entity::OnCollide(Entity& other)
{

}
void Entity::TakeDamage(const int& damage)
{
	if (!IsVulnerable()) { return; }
	life -= damage;
	active = life > 0;
	invincibilityFrame = 0;
}
void Entity::AddToEntitiesList(std::vector<Entity*>& entities)
{

}

void Entity::ResetMovement()
{
	velocity = { (float)(rand() % 40 - 20), 0, (float)(rand() % 40 - 20) };
}