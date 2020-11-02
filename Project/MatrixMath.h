#ifndef MATRIXMATH_H
#define MATRIXMATH_H

#include <vector>
#include <array>
#include <cmath>

namespace MMath
{
	//Get matrix
	std::vector<float> GetRotationMatrix(int axis, float j);
	std::vector<float> GetScalingMatrix(float x, float y, float z);

	//Math involving colors
	std::vector<float> GetAverageColor(std::vector<float>, ...);

	//Calculations only involving vectors
	float Magnitude(const std::array<float,3> a, int b); //Length of the vector
	float Distance(std::array<float,3> a, std::array<float,3> b);
	std::array<float,3> Normalize(std::array<float,3> a);
	float Area(std::array<float, 3> a, std::array<float, 3> b);
	float Dot(std::array<float,3> a, std::array<float,3> b);
	float GetAngle(std::array<float,3> a, std::array<float,3> b);
	bool IsParallel(std::array<float,3> a, std::array<float,3> b); //if the cross product is vector 0, then theyre parallel
	std::array<float,3> Cross(std::array<float,3> a, std::array<float,3> b); //Get third perpendcular with this

	//Calculations involving matrices
	std::vector<float> MultiplyByScalar(std::vector<float> a, float b);
	std::array<float,3> MultiplyByScalar(std::array<float,3> a, float b);
	std::vector<float> DivideByScalar(std::vector<float> a, float b);
	std::array<float,3> DivideByScalar(std::array<float,3> a, float b);
	std::vector<float> MatrixAddition(std::vector<float> a, std::vector<float> b);
	std::array<float,3> MatrixAddition(std::array<float,3> a, std::array<float,3> b);
	std::vector<float> MatrixSubtraction(std::vector<float> a, std::vector<float> b);
	std::array<float,3> MatrixSubtraction(std::array<float,3> a, std::array<float,3> b);
	std::vector<float> MatrixMultiplication(std::vector<float> a, std::vector<float> b, int a_rows, int b_cols);
	std::array<float,3> MatrixMultiplication(std::array<float,3> a, std::vector<float> b, int a_rows, int b_cols);
}
#endif