#include "Entity.h"

Entity::Face::Face(std::vector<std::vector<float>*>& a, std::vector<int>& color, int drawMethod): 
	vertex(a), drawMethod(drawMethod), color(color)
{
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
		turnSpeed = 2;
		life = 5;
		break;
	case EntityType::PerfectSphere: 
		velocity = { (float)(rand() % 20 + 10), 0, (float)(rand() % 20 + 10) };
		scale = 1;
		//sectors 4 stacks 2 = 8 sided die
		//5,4 also works
		/*(5,3), (3,3), (3,2), (10,9), (4,8)*/
		position = { 30, 0, 30 };
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
		position = { 0, 0, 0 };
		speed = 0;
		turnSpeed = 0;
		acc = 1;
		life = 1;
		break;
		case EntityType::Laser:
		{
			active = false;
			scale = 1;
			speed = 100;
			vertex.push_back({ 0, 0, 0 });
			vertex.push_back({ 0, 0, 20 });
			position = { 0,0,0 };
			velocity = { 0,0,0 };
			std::vector<std::vector<float>*> temp = { &vertex[0],&vertex[1]};
			std::vector<int> color = { 100, 255, 255 };
			faces.push_back(Face(temp, color, GL_LINE_STRIP));
			acc = 1;
		}
			break;
	}
}

void Entity::Build()
{
	
}

void Entity::Update(float dt)
{
	if (!active) { return; }
	for (int i = 0; i < 3; i++)
	{
		position[i] += velocity[i] * acc * dt; //Moves ship forward
	}
	OnUpdate(dt);
}
void Entity::OnUpdate(float dt)
{

}

void Entity::CheckWorldBounds(int i)
{
	for (int j = 0; j < vertex.size(); j++)
	{
		std::vector<float> temp = vertex[j];
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

void Entity::Rotate(float i, int axis)
{
}

void Entity::Draw()
{
	if (!active) { return; }

	for (int i = 0; i < faces.size(); i++)
	{
		glBegin(faces[i].drawMethod);
		Color(faces[i].color);
		for (int j = 0; j < faces[i].vertex.size(); j++)
		{
			std::vector<float> temp = *faces[i].vertex[j]; //Get vertices from the face
			for (int k = 0; k < temp.size(); k++)
			{
				temp[k] = temp[k] * scale + position[k]; //Move all variables of the vertex
			}
			glVertex3f(temp[0], temp[1], temp[2]);
		}
	}
	glEnd();
}
