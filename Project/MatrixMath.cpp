#include "MatrixMath.h"

float MMath::Magnitude(std::vector<float> a, int b) //b is the length of the vector
{
	int c = 0;
	for (int i = 0; i < b; i++)
	{
		c += pow(a[i], 2);
	}
	return sqrt(c);
}

std::vector<float> MMath::GetRotationMatrix(int axis, float j)
{
	j*= 3.1415f / 180;
	switch (axis)
	{
	case 0: return //X axis
	{
		1, 0,		0,
		0, cos(j),  sin(j),
		0, -sin(j), cos(j)
	}; 
	case 1: return//Y axis
	{
		cos(j),  0,		-sin(j),
		0,		 1,		 0,
		sin(j),  0,	     cos(j)
	};
	case 2: return //Z axis
	{
		cos(j),  sin(j), 0,
		-sin(j), cos(j), 0,
		0,		 0,	     1
	};
	}
}
std::vector<float> MMath::GetScalingMatrix(float x, float y, float z)
{
	return 
	{
		x, 0, 0,
		0, y, 0,
		0, 0, z
	};
}

std::vector<float> MMath::MatrixMultiplication(std::vector<float> a, std::vector<float> b, int a_rows, int b_cols)
{
	std::vector<float> c = {};
	for (int i = 0; i < a_rows * b_cols; i++)
	{
		int x = i % b_cols; int y = i / b_cols;
		if (y > a_rows) { break; }
		float temp = 0;
		for (int j = 0; j < b_cols; j++)
		{
			float a_val = a[y * b_cols + j];
			float b_val = b[x + b_cols * j];
			temp += a_val * b_val;
		}
		c.push_back(temp);
	} return c;
}