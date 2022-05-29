#include "gtest/gtest.h"
#include "TerrainDistanceCalculator.h"

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(TerrainDistanceCalculator, Test1)
{
	std::cout << CalculateTerrainDistance() << "\n";
}