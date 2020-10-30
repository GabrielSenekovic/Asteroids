#ifndef MATRIXMATH_H
#define MATRIXMATH_H
#include <vector>
#include <cmath>

namespace MMath
{
	//Get matrix
	std::vector<float> GetRotationMatrix(int axis, float j);
	std::vector<float> GetScalingMatrix(float x, float y, float z);

	//Math involving colors
	std::vector<float> GetAverageColor(std::vector<float>, ...);

	//Calculations only involving vectors
	float Magnitude(std::vector<float> a, int b); //Length of the vector
	float Distance(std::vector<float> a, std::vector<float> b);
	std::vector<float> Normalize(std::vector<float> a);
	float Area(std::vector<float> a, std::vector<float> b);
	float Dot(std::vector<float> a, std::vector<float> b);
	float GetAngle(std::vector<float> a, std::vector<float> b);
	bool IsParallel(std::vector<float> a, std::vector<float> b); //if the cross product is vector 0, then theyre parallel
	std::vector<float> Cross(std::vector<float> a, std::vector<float> b); //Get third perpendcular with this

	//Calculations involving matrices
	std::vector<float> MultiplyByScalar(std::vector<float> a, float b);
	std::vector<float> DivideByScalar(std::vector<float> a, float b);
	std::vector<float> MatrixAddition(std::vector<float> a, std::vector<float> b);
	std::vector<float> MatrixSubtraction(std::vector<float> a, std::vector<float> b);
	std::vector<float> MatrixMultiplication(std::vector<float> a, std::vector<float> b, int a_rows, int b_cols);
}

#endif