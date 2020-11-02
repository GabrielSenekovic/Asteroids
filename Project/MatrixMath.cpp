#include "MatrixMath.h"

float MMath::Magnitude(std::array<float,3> a, int b) //b is the length of the vector
{
	int c = 0;
	for (int i = 0; i < b; i++)
	{
		c += pow(a[i], 2);
	}
	return sqrt(c);
}
float MMath::Distance(std::array<float,3> a, std::array<float,3> b)
{
	return Magnitude(MatrixSubtraction(a,b), 3);
}

std::array<float,3> MMath::Normalize(std::array<float, 3> a)
{
	return DivideByScalar(a, Magnitude(a, 3));
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
float MMath::Area(std::array<float,3> a, std::array<float,3> b)
{
	return Magnitude(Cross(a, b), 3);
}
float MMath::Dot(std::array<float,3> a, std::array<float,3> b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
float MMath::GetAngle(std::array<float, 3> a, std::array<float, 3> b)
{
	return acos(Dot(Normalize(a), Normalize(b)));
}
std::array<float,3> MMath::Cross(std::array<float, 3> a, std::array<float, 3> b)
{
	//Assume its two vectors with 3 values
	return {
		a[1] * b[2] - a[2] * b[1],
		a[2] * b[0] - a[0] * b[2],
		a[0] * b[1] - a[1] * b[0]
	};
}
bool MMath::IsParallel(std::array<float,3> a, std::array<float,3> b)
{
	return Cross(a,b) == std::array<float,3>{0,0,0};
}
std::vector<float> MMath::MultiplyByScalar(std::vector<float> a, float b)
{
	return { a[0] * b, a[1] * b, a[2] * b };
}
std::array<float, 3> MMath::MultiplyByScalar(std::array<float, 3> a, float b)
{
	return { a[0] * b, a[1] * b, a[2] * b };
}
std::vector<float> MMath::DivideByScalar(std::vector<float> a, float b)
{
	return {a[0] / b, a[1] / b, a[2] / b};
}
std::array<float, 3> MMath::DivideByScalar(std::array<float, 3> a, float b)
{
	return { a[0] / b, a[1] / b, a[2] / b };
}

std::vector<float> MMath::MatrixAddition(std::vector<float> a, std::vector<float> b)
{
	std::vector<float> c = {};
	for (int i = 0; i < a.size(); i++)
	{
		c.push_back(a[i] + b[i]);
	}
	return c;
}
std::array<float,3> MMath::MatrixAddition(std::array<float, 3> a, std::array<float, 3> b)
{
	std::array<float, 3> c = {};
	for (int i = 0; i < a.size(); i++)
	{
		c[i] = a[i] + b[i];
	}
	return c;
}
std::vector<float> MMath::MatrixSubtraction(std::vector<float> a, std::vector<float> b)
{
	std::vector<float> c = {};
	for (int i = 0; i < a.size(); i++)
	{
		c.push_back(a[i] - b[i]);
	}
	return c;
}
std::array<float, 3> MMath::MatrixSubtraction(std::array<float, 3> a, std::array<float, 3> b)
{
	std::array<float,3> c = {};
	for (int i = 0; i < a.size(); i++)
	{
		c[i] = a[i] - b[i];
	}
	return c;
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
std::array<float, 3> MMath::MatrixMultiplication(std::array<float, 3> a, std::vector<float> b, int a_rows, int b_cols)
{
	std::array<float,3> c = {};
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
		c[i] = temp;
	} return c;
}