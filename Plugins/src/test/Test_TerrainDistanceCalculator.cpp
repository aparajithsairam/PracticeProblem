#include "gtest/gtest.h"
#include "TerrainDistanceCalculator.h"

#include <fstream>

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

float CalculateTerrainDistance(
	const std::string& heightMapFilePath, 
	int aX, int aY,
	int bX, int bY)
{
	const int sizeOfHtMap = 512 * 512;
	std::vector<uint8_t> htMap(sizeOfHtMap);
	std::ifstream inFile(heightMapFilePath, std::ios_base::binary);
	inFile.read((char*)&htMap[0], sizeOfHtMap);

	return CalculateTerrainDistance(&htMap[0], aX, aY, bX, bY);
}

float CalculateTerrainDistanceDifference(
	const std::string& heightMapFilePath1,
	const std::string& heightMapFilePath2,
	int aX, int aY,
	int bX, int bY)
{
	return CalculateTerrainDistance(heightMapFilePath1, aX, aY, bX, bY)
		- CalculateTerrainDistance(heightMapFilePath2, aX, aY, bX, bY);
}

TEST(TerrainDistanceCalculator, WrongPoint)
{
	std::cout << CalculateTerrainDistance(
		TEST_DATA_PATH "/pre.data",
		2, 3,
		2, -3) << "\n";
}

TEST(TerrainDistanceCalculator, SamePoint)
{
	std::cout << CalculateTerrainDistance(
		TEST_DATA_PATH "/pre.data",
		2, 3,
		2, 3) << "\n";
}

TEST(TerrainDistanceCalculator, VerticalDistance)
{
	std::cout << CalculateTerrainDistance(
		TEST_DATA_PATH "/pre.data",
		2, 3,
		2, 5) << "\n";
}

TEST(TerrainDistanceCalculator, HorizontalDistance)
{
	std::cout << CalculateTerrainDistance(
		TEST_DATA_PATH "/post.data",
		2, 30,
		10, 30) << "\n";
}

TEST(TerrainDistanceCalculator, Diagonal45DegDistance)
{
	std::cout << CalculateTerrainDistance(
		TEST_DATA_PATH "/post.data",
		2, 10,
		12, 20) << "\n";
}

TEST(TerrainDistanceCalculator, Diagonal135DegDistance)
{
	std::cout << CalculateTerrainDistance(
		TEST_DATA_PATH "/post.data",
		2, 100,
		62, 40) << "\n";
}

TEST(TerrainDistanceCalculator, NonSpecialCase)
{
	std::cout << CalculateTerrainDistance(
		TEST_DATA_PATH "/pre.data",
		2, 30,
		10, 50) << "\n";
}

TEST(TerrainDistanceDifferenceCalculator, Diff)
{
	std::cout << CalculateTerrainDistanceDifference(
		TEST_DATA_PATH "/pre.data",
		TEST_DATA_PATH "/post.data",
		2, 3,
		4, 5) << "\n";
}