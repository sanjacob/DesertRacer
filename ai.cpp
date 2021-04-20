/**
 * @file ai.cpp
 * Do non-player hover cars dream of electric races?
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#include <TL-Engine.h>
#include "node.h"
#include "ai.h"
#include "collision.h"

using namespace tle;
using namespace desert;


HoverAI::HoverAI(IModel* m) : SphereCollisionModel(m)
{
	mRadius = kCollisionRadius;
	cout << "Hover AI created" << endl;
}

HoverAI::~HoverAI()
{
	cout << "Hover AI destroyed" << endl;
}

void HoverAI::follow(ISceneNode* destination, bool nextWaypoint)
{
	// Look towards node
	node->LookAt(destination);
	// Obtain the facing vector
	facingVector = getFacingVector2D();
	
	// Set next target, increase waypoint index
	if (nextWaypoint)
	{
		currentTarget = { destination->GetX(), destination->GetZ() };
		++waypointIndex;
	}
}

bool HoverAI::updateScene(const float kGameSpeed, const float kDeltaTime)
{
	// If car is after some target
	if (!currentTarget.isZero())
	{
		float frameTiming = kGameSpeed * kDeltaTime;
		SVector2D moveVector = facingVector * movingSpeed * frameTiming;

		// Update movement vector
		movementThisFrame += moveVector;
		// Returns true if car has almost reached target
		return (position2D() - currentTarget).length() <= kWaypointArrivalDistance;
	}
	return false;
}

void HoverAI::applyMovementVector()
{
	// Apply movement vector, drag
	moveByVector(movementThisFrame);
	movementThisFrame *= 0.95;
}

void HoverAI::bounce()
{
	// Reverse movement vector
	movementThisFrame = -movementThisFrame * kBounce;
}

void HoverAI::stop()
{
	// Set target to zero
	currentTarget = { 0, 0 };
}

void HoverAI::reduceHealth(const int reduction)
{
	health -= reduction;
}

void HoverAI::setCollided()
{
	collided = true;
}

void HoverAI::resetCollided()
{
	collided = false;
}

const float HoverAI::getCollisionRadius() const
{
	return kCollisionRadius;
}

unsigned int HoverAI::getWaypointIndex() const
{
	return waypointIndex;
}

bool HoverAI::hasCollided() const
{
	return collided;
}