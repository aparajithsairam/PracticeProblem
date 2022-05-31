#include "TerrainDistanceCalculator.h"
#include <Eigen/Dense>
#include <Eigen/src/Geometry/OrthoMethods.h> // needed for cross product

#include <map>

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

// This function has been copied from internet and modified by me; hopefully it works
// Bool indicates if intersection happened or not
std::pair<bool, Eigen::Vector2f> lineIntersection(
	const Eigen::Vector2f& A,
	const Eigen::Vector2f& B,
	const Eigen::Vector2f& C,
	const Eigen::Vector2f& D)
{
	// Line AB represented as a1x + b1y = c1
	double a1 = B.y() - A.y();
	double b1 = A.x() - B.x();
	double c1 = a1 * (A.x()) + b1 * (A.y());

	// Line CD represented as a2x + b2y = c2
	double a2 = D.y() - C.y();
	double b2 = C.x() - D.x();
	double c2 = a2 * (C.x()) + b2 * (C.y());

	double determinant = a1 * b2 - a2 * b1;

	if (determinant == 0)
	{
		// The lines are parallel
		return std::make_pair(false, Eigen::Vector2f());
	}
	else
	{
		double x = (b2 * c1 - b1 * c2) / determinant;
		double y = (a1 * c2 - a2 * c1) / determinant;
		return std::make_pair(true, Eigen::Vector2f(x, y));
	}
}

std::pair<bool, Eigen::Vector2f> lineSegmentIntersection(
	const Eigen::Vector2f& A,
	const Eigen::Vector2f& B,
	const Eigen::Vector2f& C,
	const Eigen::Vector2f& D)
{
	bool intersected;
	Eigen::Vector2f intPt;
	std::tie(intersected, intPt) = lineIntersection(A, B, C, D);
	if(!intersected)
		return { false, Eigen::Vector2f() };

	float minX = std::min(A.x(), std::min(B.x(), std::min(C.x(), D.x())));
	float minY = std::min(A.y(), std::min(B.y(), std::min(C.y(), D.y())));

	float maxX = std::max(A.x(), std::max(B.x(), std::max(C.x(), D.x())));
	float maxY = std::max(A.y(), std::max(B.y(), std::max(C.y(), D.y())));

	if (intPt.x() >= minX && intPt.x() <= maxX &&
		intPt.y() >= minY && intPt.y() <= maxY)
	{
		return { true, intPt };
	}
	return { false, Eigen::Vector2f() };
}

// can be optimized a lot if time permits
// Totally untested
float nonSpecialCaseDistance(
	const uint8_t* htMap,
	int aX, int aY,
	int bX, int bY)
{
	std::vector<std::pair<Eigen::Vector2f, Eigen::Vector2f>> lineSegments;
	
	for (int i = 0; i < HEIGHT_MAP_DIM; ++i)
	{
		// all vertical lines
		lineSegments.push_back({ Eigen::Vector2f(i, 0), Eigen::Vector2f(i, HEIGHT_MAP_DIM - 1) });
		// all horizontal lines
		lineSegments.push_back({ Eigen::Vector2f(0, i), Eigen::Vector2f(HEIGHT_MAP_DIM - 1, i) });
		
	}

	// all diagonal lines
	for (int i = 0; i < HEIGHT_MAP_DIM - 1; ++i)
	{
		for (int j = 0; j < HEIGHT_MAP_DIM - 1; ++j)
		{
			lineSegments.push_back({ Eigen::Vector2f(i + 1, j), Eigen::Vector2f(i, j + 1) });
		}
	}

	Eigen::Vector2f a(aX, aY), b(bX, bY);
	std::map<float, Eigen::Vector2f> distanceIntersection; // will sort by distance automatically
	for (int i = 0; i < lineSegments.size(); ++i)
	{
		bool intersected;
		Eigen::Vector2f intPt;
		std::tie(intersected, intPt) = lineSegmentIntersection(a, b, lineSegments[i].first, lineSegments[i].second);
		if (intersected)
		{
			distanceIntersection.insert({ (a - intPt).norm(), intPt });
		}
	}


	float totalDist = 0.0f;
	Eigen::Vector2f* prev = nullptr;
	for (auto distIntersect : distanceIntersection)
	{
		if (prev)
		{
			totalDist += (find3DCoordinate(htMap, distIntersect.second.x(), distIntersect.second.y())
				- find3DCoordinate(htMap, prev->x(), prev->y())).norm();
		}
		prev = &distIntersect.second;
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

	return nonSpecialCaseDistance(htMap, aX, aY, bX, bY);
}