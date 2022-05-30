#include "TerrainDistanceCalculator.h"
#include <Eigen/Dense>
#include <Eigen/src/Geometry/OrthoMethods.h> // needed for cross product

const int HEIGHT_MAP_DIM = 512;

Eigen::Vector3f find3DCoordinate(
	const uint8_t* htMap,
	int x, int y)
{
	float z = 11 * htMap[x * HEIGHT_MAP_DIM + y];
	return Eigen::Vector3f(30 * x, 30 * y, z);
}

float verticalDistance(
	const uint8_t* htMap,
	int aX, int bX,
	int abY)
{
	find3DCoordinate(htMap, aX, bX);
	return 0;
}

float CalculateTerrainDistance(
	const uint8_t* htMap,
	int aX, int aY,
	int bX, int bY)
{
	verticalDistance(htMap, 0, 1, 3);
	return 0.0f;
}