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

const string DesertCheckpoint::kDefaultModelName = "Checkpoint";

Collision::CollisionAxis DesertCheckpoint::collision(SVector2D position, const float collisionRadius, bool saveAxis)
{
	SVector2D strutDistance = { kHalfLength, 0 };
	if (mAlignment == zAligned)
		strutDistance = { 0, kHalfLength };
	SVector2D strutA = position2D() - strutDistance, strutB = position2D() + strutDistance;

	Collision::CollisionAxis collisionA = Collision::circleToCircle(position, strutA, kStrutRadius, collisionRadius);
	Collision::CollisionAxis collisionB = Collision::circleToCircle(position, strutB, kStrutRadius, collisionRadius);

	if (collisionA == Collision::CollisionAxis::Both || collisionB == Collision::CollisionAxis::Both)
		return Collision::CollisionAxis::Both;
	
	return Collision::CollisionAxis::None;
}

bool DesertCheckpoint::checkpointCollision(SVector2D position)
{
	if (mAlignment == zAligned)
		return Collision::pointToBox(position, position2D(), kHalfWidth, kHalfLength) == Collision::CollisionAxis::Both;
	return Collision::pointToBox(position, position2D(), kHalfLength, kHalfWidth) == Collision::CollisionAxis::Both;
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
