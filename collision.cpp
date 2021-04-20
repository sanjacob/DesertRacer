/**
 * @file collision.cpp
 * Collision Algorithms
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#include <iostream>
#include "vector.h"
#include "collision.h"

using namespace std;
using namespace desert;

bool Collision::circleToCircle(SVector2D a, SVector2D b, const float radiusA, const float radiusB)
{
	SVector2D c = a - b;
	return c.length() <= (radiusA + radiusB);
}

SVector2D Collision::circleToCircleV(SVector2D a, SVector2D b, const float radiusA, const float radiusB)
{
	SVector2D displace;
	SVector2D c = a - b;

	const float l = c.length();
	const float overlap = (radiusA + radiusB - l);

	if (overlap >= 0)
	{
		float scaleRatio = overlap / l;
		displace = c * scaleRatio;
	}

	return displace;
}

bool Collision::sphereToSphere(SVector3D a, SVector3D b, const float radiusA, const float radiusB)
{
	SVector3D c = a - b;
	return c.length() <= (radiusA + radiusB);
}

bool Collision::pointToBox(SVector2D point, const float x1, const float x2, const float y1, const float y2)
{
	return ((point.x >= x1 && point.x <= x2) && (point.y >= y1 && point.y <= y2));
}

bool Collision::pointToBox(SVector2D point, SVector2D boxCentre, const float halfWidth, const float halfLength)
{
	return pointToBox(point, boxCentre.x - halfWidth, boxCentre.x + halfWidth, boxCentre.y - halfLength, boxCentre.y + halfLength);
}

bool Collision::pointToBox(SVector2D point, SVector2D boxCentre, const float halfSide)
{
	return pointToBox(point, boxCentre, halfSide, halfSide);
}

bool Collision::pointToBox(SVector3D point, const float x1, const float x2, const float y1, const float y2, const float z1, const float z2)
{
	return ((point.x >= x1 && point.x <= x2) && (point.y >= y1 && point.y <= y2) && (point.z >= z1 && point.z <= z2));
}

bool Collision::circleToBox(SVector2D circle, const float radius, SVector2D boxCentre, const float halfSide)
{
	return pointToBox(circle, boxCentre, halfSide + radius);
}

bool Collision::circleToBox(SVector2D circle, const float radius, SVector2D boxCentre, const float halfWidth, const float halfLength)
{
	return pointToBox(circle, boxCentre, halfWidth + radius, halfLength + radius);
}