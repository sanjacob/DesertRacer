/**
 * @file vehicle.h
 * Vehicle interface
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#ifndef DESERT_RACER_VEHICLE_H
#define DESERT_RACER_VEHICLE_H

#include <TL-Engine.h>
#include <string>
#include "vector.h"
#include "node.h"


namespace desert
{
	class DesertVehicle : public SphereCollisionModel, public VectorBasedMovement
	{
	public:
		enum VehicleType
		{
			Player,
			AI
		};
		DesertVehicle(tle::IModel* m, VehicleType t);
		void nextStage();
		void nextLap();
		void resetStage();
		VehicleType getType();
		bool isPlayer();
		bool isAI();
		std::string getTag();
		int getCurrentStage() const;
		int getCurrentLap() const;
		void setDistanceToCheckpoint(const float distance);
		float getDistanceToCheckpoint();
		void setRacePosition(const int position);
		int getRacePosition();
		bool operator<(const DesertVehicle& other) const;
		static bool compare(DesertVehicle* a, DesertVehicle* b);
		virtual void reduceHealth(const int reduction = 1) = 0;
		virtual void resetWaypoint();
	protected:
		const VehicleType type;
		float mDistanceToCheckpoint = 0;
		std::string mTag;
		int mStage = 0;
		int mLap = 0;
		int mRacePosition = 0;
	};
};

#endif