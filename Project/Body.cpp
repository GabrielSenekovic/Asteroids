#include "Body.h"
Body::Body(): Entity(EntityType::PerfectSphere), stacks(15), sectors(15)
{

}
Body::Body(EntityType type, int stacks, int sectors):stacks(stacks), sectors(sectors), Entity(type)
{
	Build();
}

void Body::Build()
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
			if (type == EntityType::ImperfectSphere) { x = rand() % 3, y = rand() % 3, z = rand() % 3; }

			phi = M_PI / 2 - i * stackStep;
			thet = j * sectorStep;
			vertices.push_back({ (r + x) * cos(phi) * cos(thet), (r + y) * cos(phi) * sin(thet), (r + z) * sin(phi) });
		}
	}
	std::vector<std::array<float,3>*> temp = { &vertices[0],&vertices[1],&vertices[2] }; //change later
	std::vector<int> color = { 0, 255, 0 };
	faces.push_back(Face(temp, color, GL_QUADS));
	col = new SphereCollider(r);
}

void Body::Draw()
{
	if (!active) { return; }
	glBegin(faces[0].drawMethod);
	Color(faces[0].color);

	for (int i = 0; i < vertices.size(); i++)
	{
		float x = i + stacks + 2 < vertices.size() ? vertices[i][0] - vertices[i + stacks + 2][0] : 0;
		float y = i + stacks + 2 < vertices.size() ? vertices[i][1] - vertices[i + stacks + 2][1] : 0;
		float z = i + stacks + 2 < vertices.size() ? vertices[i][2] - vertices[i + stacks + 2][2] : 0;

		std::array<float,3> temp1 = { x,y,z };

		x = i + stacks + 1 < vertices.size() ? vertices[i + 1][0] - vertices[i + stacks + 1][0] : 0;
		y = i + stacks + 1 < vertices.size() ? vertices[i + 1][1] - vertices[i + stacks + 1][1] : 0;
		z = i + stacks + 1 < vertices.size() ? vertices[i + 1][2] - vertices[i + stacks + 1][2] : 0;

		std::array<float,3> temp2 = { x,y,z };

		std::array<float,3> temp3 = Normalize(Cross(temp2, temp1));

		glNormal3f(temp3[0], temp3[1], temp3[2]);

		x = vertices[i][0] * scale + position[0];
		y = vertices[i][1] * scale + position[1];
		z = vertices[i][2] * scale + position[2];
		glVertex3f(x, y, z);

		if (i + stacks + 1 < vertices.size())
		{
			x = vertices[i + stacks + 1][0] * scale + position[0];
			y = vertices[i + stacks + 1][1] * scale + position[1];
			z = vertices[i + stacks + 1][2] * scale + position[2];
			glVertex3f(x, y, z);
		}

		if (i + stacks + 2 < vertices.size())
		{
			x = vertices[i + stacks + 2][0] * scale + position[0];
			y = vertices[i + stacks + 2][1] * scale + position[1];
			z = vertices[i + stacks + 2][2] * scale + position[2];
			glVertex3f(x, y, z);
		}

		if (i + 1 < vertices.size())
		{
			x = vertices[i + 1][0] * scale + position[0];
			y = vertices[i + 1][1] * scale + position[1];
			z = vertices[i + 1][2] * scale + position[2];
			glVertex3f(x, y, z);
		}
	}
	glEnd();
}

void Body::OnCollide(Entity& other)
{
	switch (other.type)
	{
		case EntityType::PlayerShip: other.TakeDamage(1); break;
		case EntityType::ImperfectSphere: break;
		case EntityType::Laser: TakeDamage(1); break;
	}
}

Asteroid::Asteroid(const Asteroid& asteroid): Asteroid(asteroid.divisions)
{

}

Asteroid::Asteroid(const int& divisions_in): Body(EntityType::ImperfectSphere, 10, 10), divisions(divisions_in)
{
	if (divisions <= 0) { return; }
	for (int i = 0; i < 3; i++)
	{
		children.push_back(Asteroid(divisions-1));
	}
	for (int i = 0; i < 3; i++)
	{
		children[i].active = false;
	}
	rotationDirections = {(float) (rand() % 3 - 1),(float)(rand() % 3 - 1), (float)(rand() % 3 - 1) };
}

void Asteroid::TakeDamage(const int& damage)
{
	if (!IsVulnerable()) { return; }
	Entity::TakeDamage(damage);
	for (int i = 0; i < children.size(); i++)
	{
		//Release baby asteroids
		children[i].active = true;
		children[i].position = position;
		children[i].speed *= 1.8f;
		children[i].scale = scale * 0.7f;
		children[i].col->scale = scale * 0.7f;
		children[i].invincibilityFrame = 0;
	}
}
void Asteroid::AddToEntitiesList(std::vector<Entity*>& entities)
{
	for (int i = 0; i < children.size(); i++)
	{
		children[i].AddToEntitiesList(entities);
		entities.push_back(&children[i]);
	}
}

bool Asteroid::OnUpdate(float dt)
{
	for (int i = 0; i < rotationDirections.size(); i++)
	{
		Rotate(rotationDirections[i], i);
	}
	/*int temp[2] = { -1,1 };
	Rotate(temp[rand()%2], rand()%3);*/ //makes them move like eggs
	return true;
}