/**
 * @file ai.cpp
 * Do non-player hover cars dream of electric races?
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#include <TL-Engine.h>
#include <string>
#include "node.h"
#include "ai.h"
#include "collision.h"

using namespace tle;
using namespace desert;

RNG HoverAI::speedRNG = {HoverAI::kMinSpeedRng, HoverAI::kMaxSpeedRng};

HoverAI::HoverAI(IModel* m, string tag) : DesertVehicle(m, DesertVehicle::VehicleType::AI), kUniqueSpeed(speedRNG.getDecimalPoint())
{
	mTag = tag;
	mRadius = kCollisionRadius;
	// Pushback after collisions
	mFixed = false;
	cout << "Hover AI created" << endl;
}

HoverAI::~HoverAI()
{
	cout << "Hover AI destroyed" << endl;
}

void HoverAI::follow(ISceneNode* destination)
{
	currentTargetNode = destination;
	targetVector = { currentTargetNode->GetX(), currentTargetNode->GetZ() };

	++waypointIndex;
}

bool HoverAI::updateScene(const float kGameSpeed, const float kDeltaTime, const float distanceToPlayer, bool ahead)
{
	//int rubberMultiplier = (ahead) ? 1 : -1;
	//float cappedDistance = distanceToPlayer * rubberMultiplier;

	//if (cappedDistance > kMaxRubberDistance)
	//{
	//	cappedDistance = kMaxRubberDistance;
	//}

	//if (cappedDistance < -kMaxRubberDistance)
	//{
	//	cappedDistance = -kMaxRubberDistance;
	//}

	//SVector2D thrustVector = kThrustVector * (cappedDistance / kRubberDivider);

	if (mInvTimer > 0)
	{
		mInvTimer -= kDeltaTime;
	}
	else if (mInvTimer < 0)
	{
		mInvTimer = 0;
	}

	// If car is after some target
	if (currentTargetNode != nullptr)
	{
		node->LookAt(currentTargetNode);

		// Update movement vector
		movementThisFrame += getFacingVector2D() * (kThrustVector * kGameSpeed * kUniqueSpeed);
		// Returns true if car has almost reached target
		return ((position2D() - targetVector).length()) <= kWaypointArrivalDistance;
	}
	return false;
}

void HoverAI::modifyMovementVector(SVector2D change)
{
	movementThisFrame = change * kBounce;
	mVectorModified = true;
}

void HoverAI::applyMovementVector(const float kDeltaTime)
{
	// Apply movement vector, drag
	moveByVector(movementThisFrame * kDeltaTime);
	movementThisFrame *= kDrag;
}

void HoverAI::bounce(Collision::CollisionAxis reverse)
{
	// Reverse movement vector
	movementThisFrame = -movementThisFrame * kBounce;
}

//void HoverAI::stop()
//{
//	// Set target to zero
//	currentTarget = { 0, 0 };
//}

void HoverAI::reset()
{
	resetPosition();
	resetStage();
	movementThisFrame = { 0, 0 };
	targetVector = { 0, 0 };
	currentTargetNode = nullptr;
	waypointIndex = 0;
	mCollided = false;
	mHealth = kInitialHealth;
	mThrust = kInitialThrust;
	mInvTimer = 0.0f;
}

void HoverAI::reduceHealth(const int reduction)
{
	mHealth -= reduction;

	if (mHealth < kSpeedHealthNerf)
	{
		mThrust = kNerfedThrust;
	}
}

void HoverAI::setCollided()
{
	if (!mInvTimer)
	{
		mCollided = true;
		mInvTimer = kInvTime;
	}
}

void HoverAI::resetCollided()
{
	mCollided = false;
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
	return mCollided;
}

void HoverAI::resetWaypoint()
{
	waypointIndex = 0;
}
