#include "Body.h"

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
			vertex.push_back({ (r + x) * cos(phi) * cos(thet), (r + y) * cos(phi) * sin(thet), (r + z) * sin(phi) });
		}
	}
	std::vector<std::array<float,3>*> temp = { &vertex[0],&vertex[1],&vertex[2] }; //change later
	std::vector<int> color = { 0, 255, 0 };
	faces.push_back(Face(temp, color, GL_QUADS));
	col = new SphereCollider(r);
}

void Body::Draw()
{
	if (!active) { return; }
	glBegin(faces[0].drawMethod);
	Color(faces[0].color);

	for (int i = 0; i < vertex.size(); i++)
	{
		float x = i + stacks + 2 < vertex.size() ? vertex[i][0] - vertex[i + stacks + 2][0] : 0;
		float y = i + stacks + 2 < vertex.size() ? vertex[i][1] - vertex[i + stacks + 2][1] : 0;
		float z = i + stacks + 2 < vertex.size() ? vertex[i][2] - vertex[i + stacks + 2][2] : 0;

		std::array<float,3> temp1 = { x,y,z };

		x = i + stacks + 1 < vertex.size() ? vertex[i + 1][0] - vertex[i + stacks + 1][0] : 0;
		y = i + stacks + 1 < vertex.size() ? vertex[i + 1][1] - vertex[i + stacks + 1][1] : 0;
		z = i + stacks + 1 < vertex.size() ? vertex[i + 1][2] - vertex[i + stacks + 1][2] : 0;

		std::array<float,3> temp2 = { x,y,z };

		std::array<float,3> temp3 = Normalize(Cross(temp2, temp1));

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