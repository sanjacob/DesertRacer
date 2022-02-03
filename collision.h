/**
 * @file collision.h
 * Collision Algorithms
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */


#ifndef DESERT_RACER_COLLISION_H
#define DESERT_RACER_COLLISION_H

#include "vector.h"

namespace desert
{
    class Collision
    {
    public:
        enum CollisionAxis
        {
            xAxis,
            yAxis,
            Both,
            None
        };
        static CollisionAxis circleToCircle(SVector2D a, SVector2D b, const float radiusA, const float radiusB);
        static bool sphereToSphere(SVector3D a, SVector3D b, const float radiusA, const float radiusB);
        static CollisionAxis pointToBox(SVector2D point, const float x1, const float x2, const float z1, const float z2);
        static CollisionAxis pointToBox(SVector2D point, SVector2D boxCenter, const float halfSide);
        static CollisionAxis pointToBox(SVector2D point, SVector2D boxCenter, const float halfWidth, const float halfLength);
        static bool pointToBox(SVector3D point, const float x1, const float x2, const float y1, const float y2, const float z1, const float z2);
        static CollisionAxis circleToBox(SVector2D circle, const float radius, SVector2D boxCentre, const float halfSide);
        static CollisionAxis circleToBox(SVector2D circle, const float radius, SVector2D boxCentre, const float halfWidth, const float halfLength);
    };
}

#endif
