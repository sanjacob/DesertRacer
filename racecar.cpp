/**
 * @file racecar.cpp
 * Player hover car controls
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#include <TL-Engine.h>
#include <iostream>
#include "vector.h"
#include "node.h"
#include "racecar.h"
#include "keybinds.h"

using namespace tle;
using namespace desert;


HoverCar::HoverCar(IModel* m, const SControlKeybinding carKeybinding) : SceneNodeContainer(m), keybind(carKeybinding)
{
	node->SetY(kModelYOffset);
	cout << "HoverCar created" << endl;
}

HoverCar::~HoverCar()
{
	cout << "HoverCar destroyed" << endl;
}

void HoverCar::addCamera(DesertCamera cam)
{	
	if (!cameras.size())
	{
		cam.setActive();
		currentCamera = cam.getCamera();
	}

	cameras.push_back(cam);
}

void HoverCar::control(I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime)
{
	const float frameGameSpeed = kGameSpeed * kDeltaTime;
	int turnMultiplier = 1;
	float resetRearLiftSpeed;
	float boostMultiplier = 1;
	boostDragMultiplier = 1;
	carState = Stationary;
	inclinationState = NotTurning;

	// Boost
	if (boostPenaltyTimer)
	{
		boostPenaltyTimer -= kDeltaTime;

		if (boostPenaltyTimer < kDeltaTime)
		{
			boostPenaltyTimer = 0.0f;
		}

		boostDragMultiplier = kDrag;
	}
	else
	{
		if (myEngine->KeyHeld(keybind.kBoost) && health > kBoostMinimumHealth)
		{
			boostTimer += kDeltaTime;

			if (boostTimer >= kBoostMaxTimeActive)
			{
				boostPenaltyTimer = kBoostTimePenalty;
				boostTimer = 0.0f;
				boostState = Penalty;
			}
			else
			{
				boostState = Active;
				boostMultiplier = kBoost;

				if ((boostTimer + kBoostWarningTime) >= kBoostMaxTimeActive)
				{
					boostState = Warning;
				}
			}
		}
		else
		{
			boostState = Inactive;
			if (boostTimer < kDeltaTime)
			{
				boostTimer = 0;
			}
			else
			{
				boostTimer -= kDeltaTime;
			}
		}
	}

	// Move Forwards / Backwards
	if (myEngine->KeyHeld(keybind.kForwardThrust))
	{
		movementThisFrame += getFacingVector2D() * (frameGameSpeed * kThrust * boostMultiplier);
		carState = Moving;

		float currentLiftSpeed = (kRearLiftSpeed * frameGameSpeed);

		if (rearLift + currentLiftSpeed < kMaxRearLift)
		{
			rearLift += currentLiftSpeed;
			node->RotateLocalX(currentLiftSpeed);
		}
	}
	else if (myEngine->KeyHeld(keybind.kBackwardsThrust))
	{
		movementThisFrame += getFacingVector2D() * (frameGameSpeed * kThrust * kBackwardThrustMultiplier * boostMultiplier);
		//node->MoveLocalZ(-kThrust * kBackwardThrustMultiplier * frameGameSpeed);
		carState = Moving;
	}

	// Rotate
	if (myEngine->KeyHeld(keybind.kClockwiseTurn))
	{
		node->RotateY(rotationSpeed * frameGameSpeed);
		inclinationState = Turning;
		turnMultiplier = -1;
	}
	else if (myEngine->KeyHeld(keybind.kAntiClockwiseTurn))
	{
		node->RotateY(-rotationSpeed * frameGameSpeed);
		inclinationState = Turning;
		turnMultiplier = 1;
	}

	// Sin Wave / Rear Lift
	switch (carState)
	{
	case Stationary:
		if (node->GetY() != kModelYOffset)
		{
			int heightModifier = (node->GetY() > kModelYOffset) ? -1 : 1;
			node->MoveY(heightModifier * frameGameSpeed * kResetYSpeed);
			timeElapsedMoving = 0;
		}

		resetRearLiftSpeed = kResetRearSpeed * frameGameSpeed;
		rearLift -= resetRearLiftSpeed;
		node->RotateLocalX(-resetRearLiftSpeed);

		if (rearLift <= resetRearLiftSpeed)
		{
			node->RotateLocalX(-rearLift);
			rearLift = 0.0f;
		}
		break;
	case Moving:
		timeElapsedMoving += kDeltaTime;
		node->SetY(sin(timeElapsedMoving * kSinFunctionMultiplier) + kModelYOffset);
		break;
	}

	// Lean
	switch (inclinationState)
	{
	case NotTurning:
		// Slowly reset hover car lean to neutral
		if (lean)
		{
			int rotationModifier = (lean > 0) ? -1 : 1;
			float resetYSpeed = rotationModifier * kResetLeanSpeed * frameGameSpeed;

			node->RotateLocalZ(resetYSpeed);
			lean += resetYSpeed;

			if ((lean * -rotationModifier) < resetYSpeed)
			{
				node->RotateLocalZ(-lean);
				lean = 0;
			}
		}
		break;
	case Turning:
		if (carState == Moving)
		{
			// Car is turning tightly, start leaning
			float leanSpeed = turnMultiplier * kLeaningSpeed * frameGameSpeed;

			if ((lean + leanSpeed) < kMaxInclination && (lean + leanSpeed) > -kMaxInclination)
			{
				lean += (leanSpeed);
				node->RotateLocalZ(leanSpeed);
			}
		}
		break;
	}

	// Camera stuff
	for (DesertCamera& cam : cameras)
	{
		cam.control(myEngine, kGameSpeed, kDeltaTime);

		if (cam.isTriggered())
		{
			currentCamera = cam.getCamera();

			for (DesertCamera& camDeactivate : cameras)
			{
				camDeactivate.setActive(false);
			}
		}
	}

	//cout << position2D().asString() << endl;
}

void HoverCar::reduceHealth(const int reduction)
{
	health -= reduction;

	if (health < kHealthSteerNerf)
	{
		rotationSpeed = kNerfedRotation;
	}
}

void HoverCar::applyMovementVector()
{
	moveByVector(movementThisFrame);
	movementThisFrame *= (kDrag * boostDragMultiplier);
}

void HoverCar::bounce()
{
	movementThisFrame = -movementThisFrame * kBounce;
}

void HoverCar::reset()
{
	resetPosition(true, false, true);
	node->ResetOrientation();
	movementThisFrame.zeroOut();
	health = kInitialHealth;

	lean = 0;
	rearLift = 0;
	rotationSpeed = kInitialRotation;
}

HoverCar::BoostState HoverCar::getBoostState() const
{
	return boostState;
}
const float HoverCar::getCollisionRadius() const
{
	return kCollisionRadius;
}
int HoverCar::getHealth() const
{
	return health;
}

int HoverCar::getSpeed() const
{
	SVector2D scaledVector = movementThisFrame * 100;
	return scaledVector.length();
}