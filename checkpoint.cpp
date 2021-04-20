/**
 * @file checkpoint.cpp
 * Handles checkpoints collision detection / effects
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#include <TL-Engine.h>
#include <iostream>
#include "node.h"
#include "checkpoint.h"
#include "collision.h"

using namespace std;
using namespace tle;
using namespace desert;

DesertCheckpoint::DesertCheckpoint(IModel* checkpointModel, NodeAlignment alignment, IMesh* cross) : BoxCollisionModel(checkpointModel, alignment), crossMesh(cross)
{
	mHalfLength = kHalfLength;
	mHalfWidth = kHalfWidth;
	crossModel = crossMesh->CreateModel(0, kCrossInactiveY);
	crossModel->AttachToParent(node);
	crossModel->Scale(kCrossScale);
}

bool DesertCheckpoint::collision(SVector2D position, const float collisionRadius)
{
	SVector2D strutDistance = { kHalfLength, 0 };
	if (mAlignment == zAligned)
		strutDistance = { 0, kHalfLength };
	SVector2D strutA = position2D() - strutDistance, strutB = position2D() + strutDistance;
	
	bool collisionA = Collision::circleToCircle(position, strutA, kStrutRadius, collisionRadius);
	bool collisionB = Collision::circleToCircle(position, strutB, kStrutRadius, collisionRadius);

	return collisionA + collisionB;
}

bool DesertCheckpoint::checkpointCollision(SVector2D position)
{
	if (mAlignment == zAligned)
		return Collision::pointToBox(position, position2D(), kHalfWidth, kHalfLength);
	return Collision::pointToBox(position, position2D(),  kHalfLength, kHalfWidth);
}

void DesertCheckpoint::setCrossed()
{
	state = RecentlyCrossed;
	crossModel->SetY(kCrossActiveY);
	crossElapsed = 0.0f;
}

void DesertCheckpoint::reset()
{
	state = Uncrossed;
	crossModel->SetY(kCrossInactiveY);
}

void DesertCheckpoint::updateScene(const float kDeltaTime)
{
	if (state == RecentlyCrossed)
	{
		crossElapsed += kDeltaTime;

		if (crossElapsed > kCrossLifetime)
		{
			// Hide cross
			state = Uncrossed;
			crossModel->SetY(kCrossInactiveY);
		}
	}
}