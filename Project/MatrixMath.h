#ifndef MATRIXMATH_H
#define MATRIXMATH_H
#include <vector>
#include <cmath>

namespace MMath
{
	float Magnitude(std::vector<float> a, int b);

	std::vector<float> GetRotationMatrix(int axis, float j);

	std::vector<float> GetScalingMatrix(float x, float y, float z);

	std::vector<float> MatrixMultiplication(std::vector<float> a, std::vector<float> b, int a_rows, int b_cols);
}

#endif