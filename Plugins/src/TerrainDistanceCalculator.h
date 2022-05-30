#pragma once

#include <vector>

#if defined(__CYGWIN32__)
#define UNITY_INTERFACE_API __stdcall
#define UNITY_INTERFACE_EXPORT __declspec(dllexport)
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WINAPI_FAMILY)
#define UNITY_INTERFACE_API __stdcall
#define UNITY_INTERFACE_EXPORT __declspec(dllexport)
#elif defined(__MACH__) || defined(__ANDROID__) || defined(__linux__)
#define UNITY_INTERFACE_API
#define UNITY_INTERFACE_EXPORT
#else
#define UNITY_INTERFACE_API
#define UNITY_INTERFACE_EXPORT
#endif

extern "C"
{
	// Approach: Treat the problem as 2D initially
	// Find the intersection of line segment from A to B
	// with every triangle edge (within bounds of AB) of height map
	// Sort them according to distance from A to B
	// Now for converting this to 3D:
	// At every intersection point find the 3D coordinate
	// Sum the distance between consecutive 3D points
	// Return sum

	// Corner cases are vertical line, horizontal line, 45 deg diagonal line
	// Also have to consider corner case in 3D when the "cutting plane"
	// is passing through a triangle which is vertcial
	UNITY_INTERFACE_EXPORT float UNITY_INTERFACE_API CalculateTerrainDistance(
		const uint8_t* htMap,
		int aX, int aY,
		int bX, int bY);
}