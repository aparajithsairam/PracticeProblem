#include "TerrainDistanceCalculator.h"
#include <Eigen/Dense>
#include <Eigen/src/Geometry/OrthoMethods.h> // needed for cross product

const int HEIGHT_MAP_DIM = 512;
const float EPSILON = 0.001f;

Eigen::Vector3f find3DCoordinate(
	const uint8_t* htMap,
	int x, int y)
{
	float z = 11 * htMap[x * HEIGHT_MAP_DIM + y];
	return Eigen::Vector3f(30 * x, 30 * y, z);
}


Eigen::Vector3f find3DCoordinate(
	const uint8_t* htMap,
	float x, int y)
{
	int floorX = floor(x);
	if (floorX < 0)
		floorX = 0;
	int ceilX = ceil(x);

	auto floor3D = find3DCoordinate(htMap, floorX, y);
	if (ceilX >= HEIGHT_MAP_DIM)
		return floor3D;

	auto ceil3D = find3DCoordinate(htMap, ceilX, y);

	return floor3D * (x - floorX) + ceil3D * (ceilX - x);
}

Eigen::Vector3f find3DCoordinate(
	const uint8_t* htMap,
	int x, float y)
{
	int floorY = floor(y);
	if (floorY < 0)
		floorY = 0;
	int ceilY = ceil(y);

	auto floor3D = find3DCoordinate(htMap, x, floorY);
	if (ceilY >= HEIGHT_MAP_DIM)
		return floor3D;

	auto ceil3D = find3DCoordinate(htMap, x, ceilY);

	return floor3D * (y - floorY) + ceil3D * (ceilY - y);
}

Eigen::Vector3f find3DCoordinate(
	const uint8_t* htMap,
	float x, float y)
{
	int roundX = round(x);
	int roundY = round(y);

	if (abs(x - roundX) < EPSILON &&
		abs(y - roundY) < EPSILON)
	{
		return find3DCoordinate(htMap, roundX, roundY);
	}

	if (abs(x - roundX) < EPSILON)
		return find3DCoordinate(htMap, roundX, y);

	if (abs(y - roundY) < EPSILON)
		return find3DCoordinate(htMap, x, roundY);

	int floorX = floor(x);
	int floorY = floor(y);
	int ceilX = ceil(x);
	int ceilY = ceil(y);

	auto a = find3DCoordinate(htMap, ceilX, floorY);
	auto b = find3DCoordinate(htMap, floorX, ceilY);
	float sqrtOf2 = sqrt(2.0f);
	float distARatio = (Eigen::Vector2f(x, y) - Eigen::Vector2f(ceilX, floorY)).norm() / sqrtOf2;
	float distBRatio = (Eigen::Vector2f(x, y) - Eigen::Vector2f(floorX, ceilY)).norm() / sqrtOf2;

	return a * distARatio + b * distBRatio;
}

float verticalDistance(
	const uint8_t* htMap,
	int abX,
	int aY, int bY)
{
	float totalDist = 0.0f;
	if (aY > bY)
		std::swap(aY, bY);
	for (int y = aY; y < bY; ++y)
	{
		auto aVec = find3DCoordinate(htMap, abX, y);
		auto bVec = find3DCoordinate(htMap, abX, y + 1);
		totalDist += (aVec - bVec).norm();
	}
	return totalDist;
}

float horizontalDistance(
	const uint8_t* htMap,
	int aX, int bX,
	int abY)
{
	float totalDist = 0.0f;
	if (aX > bX)
		std::swap(aX, bX);
	for (int x = aX; x < bX; ++x)
	{
		auto aVec = find3DCoordinate(htMap, x, abY);
		auto bVec = find3DCoordinate(htMap, x + 1, abY);
		totalDist += (aVec - bVec).norm();
	}
	return totalDist;
}

float CalculateTerrainDistance(
	const uint8_t* htMap,
	int aX, int aY,
	int bX, int bY)
{
	if (aX < 0 || aX >= HEIGHT_MAP_DIM ||
		aY < 0 || aY >= HEIGHT_MAP_DIM ||
		bX < 0 || bX >= HEIGHT_MAP_DIM ||
		bY < 0 || bY >= HEIGHT_MAP_DIM)
		return -1.0f;

	if (aX == bX &&
		aY == bY)
		return 0.0f;

	if (aX == bX)
		return verticalDistance(htMap, aX, aY, bY);

	if (aY == bY)
		return horizontalDistance(htMap, aX, bX, aY);

	return 0.0f;
}