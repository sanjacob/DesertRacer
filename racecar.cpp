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
#include "particle.h"

using namespace tle;
using namespace desert;

const string HoverCar::kDefaultModelName = "Racecar";

HoverCar::HoverCar(IModel* m, const SControlKeybinding carKeybinding, IMesh* flareMesh) : DesertVehicle(m, DesertVehicle::VehicleType::Player), mKeybind(carKeybinding)
{
	mTag = "Player 1";
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
		mCurrentCamera = cam.getCamera();
	}

	cameras.push_back(cam);
}

float HoverCar::processBoost(tle::I3DEngine* myEngine, const float kDeltaTime)
{
	float boostMultiplier = 1;

	// If time penalty is active
	if (boostPenaltyTimer)
	{
		// Reduce remaining penalty time
		boostPenaltyTimer -= kDeltaTime;

		// Cutoff
		if (boostPenaltyTimer < kDeltaTime)
		{
			boostPenaltyTimer = 0.0f;
		}

		// Apply double drag
		boostDragMultiplier = kDrag;
	}
	else
	{
		// Detect boost key press
		// Health needs to be above a certain limit for the boost to work
		if (myEngine->KeyHeld(mKeybind.kBoost) && health > kBoostMinimumHealth)
		{
			//
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
		// Boost is not being used and not time penalised
		else
		{
			boostState = Inactive;
			boostTimer -= kDeltaTime;

			if (boostTimer < kDeltaTime)
			{
				boostTimer = 0;
			}
		}
	}

	return boostMultiplier;
}

void HoverCar::processThrust(tle::I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime, const float boostMultiplier)
{
	// Move Forwards / Backwards
	if (myEngine->KeyHeld(mKeybind.kForwardThrust))
	{
		movementThisFrame += getFacingVector2D() * (kThrustVector * kGameSpeed * boostMultiplier);
		carState = Moving;

		float currentLiftSpeed = (kRearLiftSpeed * kGameSpeed * kDeltaTime);

		if (rearLift + currentLiftSpeed < kMaxRearLift)
		{
			rearLift += currentLiftSpeed;
			node->RotateLocalX(currentLiftSpeed);
		}
	}
	else if (myEngine->KeyHeld(mKeybind.kBackwardsThrust))
	{
		movementThisFrame += getFacingVector2D() * (kThrustVector * kGameSpeed * kBackwardThrustMultiplier * boostMultiplier);
		//node->MoveLocalZ(-kThrust * kBackwardThrustMultiplier * frameGameSpeed);
		carState = Moving;
	}
}

int HoverCar::processTurn(tle::I3DEngine* myEngine, const float frameSpeed)
{
	int turnMultiplier = 1;

	// Rotate
	if (myEngine->KeyHeld(mKeybind.kClockwiseTurn))
	{
		node->RotateY(rotationSpeed * frameSpeed);
		inclinationState = Turning;
		turnMultiplier = -1;
	}
	else if (myEngine->KeyHeld(mKeybind.kAntiClockwiseTurn))
	{
		node->RotateY(-rotationSpeed * frameSpeed);
		inclinationState = Turning;
		turnMultiplier = 1;
	}

	return turnMultiplier;
}

void HoverCar::processLean(const float frameSpeed, const int turnMultiplier)
{
	switch (inclinationState)
	{
	case NotTurning:
		// Slowly reset hover car lean to neutral
		if (lean)
		{
			int rotationModifier = (lean > 0) ? -1 : 1;
			float resetYSpeed = rotationModifier * kResetLeanSpeed * frameSpeed;

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
			float leanSpeed = turnMultiplier * kLeaningSpeed * frameSpeed;

			if ((lean + leanSpeed) < kMaxInclination && (lean + leanSpeed) > -kMaxInclination)
			{
				lean += (leanSpeed);
				node->RotateLocalZ(leanSpeed);
			}
		}
		break;
	}
}

void HoverCar::processBobble(const float kGameSpeed, const float kDeltaTime)
{
	const float frameSpeed = kGameSpeed * kDeltaTime;
	float resetRearLiftSpeed;

	// Sin Wave / Rear Lift
	switch (carState)
	{
	case Stationary:
		if (node->GetY() != kModelYOffset)
		{
			int heightModifier = (node->GetY() > kModelYOffset) ? -1 : 1;
			node->MoveY(heightModifier * frameSpeed * kResetYSpeed);

			if ((node->GetY() - kModelYOffset) < (frameSpeed * kResetYSpeed))
			{
				node->SetY(kModelYOffset);
			}

			timeElapsedMoving = 0;
		}

		resetRearLiftSpeed = kResetRearSpeed * frameSpeed;
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
}

void HoverCar::control(I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime)
{
	const float frameSpeed = kGameSpeed * kDeltaTime;
	// Reset states
	inclinationState = NotTurning;
	carState = Stationary;
	boostDragMultiplier = 1;

	int turnMultiplier = processTurn(myEngine, frameSpeed);
	float boostMultiplier = processBoost(myEngine, kDeltaTime);

	processThrust(myEngine, kGameSpeed, kDeltaTime, boostMultiplier);
	processBobble(kGameSpeed, kDeltaTime);
	processLean(frameSpeed, turnMultiplier);
	controlCameras(myEngine, kGameSpeed, kDeltaTime);

	damageTimer += kDeltaTime;
}

void HoverCar::controlCameras(I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime)
{
	// Camera stuff
	for (DesertCamera& cam : cameras)
	{
		cam.control(myEngine, kGameSpeed, kDeltaTime);

		if (cam.isTriggered())
		{
			mCurrentCamera = cam.getCamera();

			for (DesertCamera& camDeactivate : cameras)
			{
				camDeactivate.setActive(false);
			}
		}
	}
}

void HoverCar::reduceHealth(const int reduction)
{
	if (damageTimer > kDamageBuffer)
	{
		health -= reduction;
		damageTimer = 0;

		if (health < kHealthSteerNerf)
		{
			rotationSpeed = kNerfedRotation;
		}
	}
}

void HoverCar::applyMovementVector(const float kDeltaTime)
{
	moveByVector(movementThisFrame * kDeltaTime);
	movementThisFrame *= (kDrag * boostDragMultiplier);
	if (movementThisFrame.length() < kDragCutoff)
	{
		movementThisFrame.zeroOut();
	}
}

SVector2D HoverCar::getMovementVector()
{
	return movementThisFrame;
}

void HoverCar::bounce(Collision::CollisionAxis reverse)
{
	switch (reverse)
	{
	case desert::Collision::xAxis:
		movementThisFrame.x = -movementThisFrame.x * kBounce;
		break;
	case desert::Collision::yAxis:
		movementThisFrame.y = -movementThisFrame.y * kBounce;
		break;
	default:
		movementThisFrame = -movementThisFrame * kBounce;
		break;
	}
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
	resetStage();
}

ICamera* HoverCar::getCamera()
{
	return mCurrentCamera;
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

float HoverCar::getSpeed() const
{
	SVector2D scaledVector = movementThisFrame;
	return scaledVector.length() * kWorldScale;
}

bool HoverCar::speedOverCollisionThreshold() const
{
	return getSpeed() > kCollisionSpeedThreshold;
}