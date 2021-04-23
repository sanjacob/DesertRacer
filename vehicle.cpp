/**
 * @file vehicle.cpp
 * Vehicle interface
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#include <TL-Engine.h>
#include "vehicle.h"

using namespace tle;
using namespace desert;


DesertVehicle::DesertVehicle(IModel* m, VehicleType t) : SphereCollisionModel(m),  type(t)
{

}

void DesertVehicle::nextStage()
{
	++mStage;
}

void DesertVehicle::nextLap()
{
	++mLap;
}

void DesertVehicle::resetStage()
{
	mStage = 0;
	mLap = 0;
}

string DesertVehicle::getTag()
{
	return mTag;
}

bool DesertVehicle::isAI()
{
	return (type == AI);
}

bool DesertVehicle::isPlayer()
{
	return (type == Player);
}

DesertVehicle::VehicleType DesertVehicle::getType()
{
	return type;
}

int DesertVehicle::getCurrentStage() const
{
	return mStage;
}

int DesertVehicle::getCurrentLap() const
{
	return mLap;
}

void DesertVehicle::setDistanceToCheckpoint(const float distance)
{
	mDistanceToCheckpoint = distance;
}

float DesertVehicle::getDistanceToCheckpoint()
{
	return mDistanceToCheckpoint;
}

void DesertVehicle::setRacePosition(const int position)
{
	mRacePosition = position;
}

int DesertVehicle::getRacePosition()
{
	return mRacePosition;
}

bool DesertVehicle::operator<(const DesertVehicle& other) const
{
	if (mLap == other.mLap)
	{
		if (mStage == other.mStage)
		{
			return mDistanceToCheckpoint < other.mDistanceToCheckpoint;
		}

		return mStage > other.mStage;
	}

	return mLap > other.mLap;
}

bool DesertVehicle::compare(DesertVehicle* a, DesertVehicle* b)
{
	return *a < *b;
}

void DesertVehicle::resetWaypoint() {}