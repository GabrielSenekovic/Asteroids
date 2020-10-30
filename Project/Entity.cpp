#include "Entity.h"

Entity::Entity(EntityType type_in)
{
	srand(rd());
	direction = { 0,0,1 };
	velocity = {0,0,0};
	type = type_in;
	switch (type_in)
	{
	case EntityType::Ship:
		scale = 1;
		vertex.push_back({1,0,-1.5f});
		vertex.push_back({-1,0,-1.5f });
		vertex.push_back({0,0,3 });
		position = { 0,0,0 };
		drawMethod = GL_TRIANGLES;
		color = { 255, 0, 0 };
		speed = 2;
		accForce = 0.3f;
		turnSpeed = 2;
		life = 5;
		break;
	case EntityType::PerfectSphere: 
		velocity = { (float)(rand() % 20 + 10), 0, (float)(rand() % 20 + 10) };
		scale = 1;
		sectors = 18;
		stacks = 18;
		BuildSphere();
		position = { 30, 0, 30 };
		drawMethod = GL_QUADS; 
			//if you draw normally
			//if you do GL_Polygon it turns into a cool helix kind of a thing
			//if you do GL_TRIANGLES you get a cool windy effect
			//GL_TRIANGLES_STRIP gives like a snaky helix, also a bit windy
		color = { 0, 255, 0 };
		speed = 0;
		turnSpeed = 0;
		life = 10;
		break;
	case EntityType::ImperfectSphere: 
		velocity = { (float)(rand() % 40 -20), 0, (float)(rand() % 40 - 20) };
		scale = 1;
		sectors = 12;
		stacks = 12;
		BuildSphere();
		position = { 0, 0, 0 };
		drawMethod = GL_QUADS;
		color = { 0, 255, 0 };
		speed = 0;
		turnSpeed = 0;
		acc = 1;
		life = 1;
		break;
	}
}

void Entity::BuildSphere()
{
	float r = 10;
	float phi = 0;
	float thet = 0;
	float sectorStep = 2 * M_PI / sectors;
	float stackStep = M_PI / stacks;

	for (int i = 0; i <= stacks; i++)
	{
		for (int j = 0; j <= sectors; j++)
		{
			float x = 0, y = 0, z = 0;
			if (type == EntityType::ImperfectSphere) { x = rand() %3, y = rand() %3, z = rand() %3; }

			phi = M_PI / 2 - i * stackStep;
			thet = j * sectorStep;
			vertex.push_back({ (r + x) * cos(phi) * cos(thet), (r+y) * cos(phi) * sin(thet), (r+z) * sin(phi) });
		}
	}
}

void Entity::Update(float dt)
{
	for (int i = 0; i < 3; i++)
	{
		position[i] += velocity[i] * acc * dt; //Moves ship forward
	}
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

void Entity::UpdateVelocity()
{
	for (int j = 0; j < 3; j++)
	{
		velocity[j] = direction[j] * speed;
	}
}

void Entity::Move(float i)
{
	bool isTooFast = abs(acc + accForce * i) > accCap;

	acc = isTooFast? accCap * i * isTooFast:acc; //If its not too fast, dont touch it

	acc += (EqSgn(acc, i)) ? i * accForce * !isTooFast //if going in the same direction, accelerate
		: i * accForce * 3 * !isTooFast; //if going in the other direction, deaccelerate

	UpdateVelocity();
}
void Entity::Rotate(float i, int axis)
{
	float j = turnSpeed * i;

	std::vector<float> R = MMath::GetRotationMatrix(1, j);
	direction = MMath::MatrixMultiplication(direction, R, 1, 3);
	UpdateVelocity();
	for(int i = 0; i < vertex.size(); i++)
	{ 
		vertex[i] = MMath::MatrixMultiplication(vertex[i], R, 1, 3);
	}
}

void Entity::Draw()
{
	glBegin(drawMethod);
	glColor3ub(color[0], color[1], color[2]);

	switch (type)
	{
	case EntityType::Ship: 
		for (int i = 0; i < vertex.size(); i++)
		{
			float x = vertex[i][0] * scale + position[0];
			float y = vertex[i][1] * scale + position[1];
			float z = vertex[i][2] * scale + position[2];
			glVertex3f(x, y, z);
		}
		break;
	case EntityType::ImperfectSphere:
	case EntityType::PerfectSphere: 
		DrawSphere();
		break;
	}
	glEnd();
}

void Entity::DrawSphere()
{
	for (int i = 0; i < vertex.size(); i++)
	{
		float x = i + stacks + 2 < vertex.size() ? vertex[i][0] - vertex[i + stacks + 2][0] : 0;
		float y = i + stacks + 2 < vertex.size() ? vertex[i][1] - vertex[i + stacks + 2][1] : 0;
		float z = i + stacks + 2 < vertex.size() ? vertex[i][2] - vertex[i + stacks + 2][2] : 0;

		std::vector<float> temp1 = { x,y,z };

		x = i + stacks + 1 < vertex.size() ? vertex[i + 1][0] - vertex[i + stacks + 1][0] : 0;
		y = i + stacks + 1 < vertex.size() ? vertex[i + 1][1] - vertex[i + stacks + 1][1] : 0;
		z = i + stacks + 1 < vertex.size() ? vertex[i + 1][2] - vertex[i + stacks + 1][2] : 0;

		std::vector<float> temp2 = { x,y,z };

		std::vector<float> temp3 = Normalize(Cross(temp2, temp1));

		glNormal3f(temp3[0], temp3[1], temp3[2]);

		x = vertex[i][0] * scale + position[0];
		y = vertex[i][1] * scale + position[1];
		z = vertex[i][2] * scale + position[2];
		glVertex3f(x, y, z);

		if (i + stacks + 1 < vertex.size())
		{
			x = vertex[i + stacks + 1][0] * scale + position[0];
			y = vertex[i + stacks + 1][1] * scale + position[1];
			z = vertex[i + stacks + 1][2] * scale + position[2];
			glVertex3f(x, y, z);
		}

		if (i + stacks + 2 < vertex.size())
		{
			x = vertex[i + stacks + 2][0] * scale + position[0];
			y = vertex[i + stacks + 2][1] * scale + position[1];
			z = vertex[i + stacks + 2][2] * scale + position[2];
			glVertex3f(x, y, z);
		}

		if (i + 1 < vertex.size())
		{
			x = vertex[i + 1][0] * scale + position[0];
			y = vertex[i + 1][1] * scale + position[1];
			z = vertex[i + 1][2] * scale + position[2];
			glVertex3f(x, y, z);
		}
	}
}