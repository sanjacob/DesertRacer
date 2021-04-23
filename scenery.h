/**
 * @file scenery.h
 * Provides classes for 
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#ifndef DESERT_RACER_WALL_H
#define DESERT_RACER_WALL_H

#include <TL-Engine.h>
#include <iostream>

#include "vector.h"
#include "node.h"

namespace desert {
	class DesertWall : public BoxCollisionModel
	{
	public:
		DesertWall(tle::IModel* m, NodeAlignment alignment);
		~DesertWall();
		static const std::string kDefaultModelName;
	protected:
		const int kHalfWidth = 2.0f, kHalfLength = 8.5f;
	};

	class DesertTower : public BoxCollisionModel
	{
	public:
		DesertTower(tle::IModel* m, NodeAlignment alignment);
		~DesertTower();
		static const std::string kDefaultModelName;
	protected:
		const int kHalfWidth = 8.0f, kHalfLength = 8.0f;
	};

	class CustomSphereCModel : public SphereCollisionModel
	{
	public:
		CustomSphereCModel(tle::IModel* m, const float radius);
		~CustomSphereCModel();
	};
};

#endif

