/**
 * @file ai.h
 *
 * Non player-hover cars that follow waypoints.
 *
 * Now with rubberbanding!
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#ifndef DESERT_RACER_AI_H
#define DESERT_RACER_AI_H

#include <TL-Engine.h>
#include <iostream>
#include <string>
#include "vector.h"
#include "node.h"
#include "vehicle.h"


namespace desert {
	/**
	* Non-player hover cars capable of following ISceneNodes, also offer collision detection
	*/
	class HoverAI : public DesertVehicle
	{
	public:
		/**
		* @param model The hover car IModel
		*/
		HoverAI(tle::IModel* model, std::string tag = "");
		~HoverAI();
		/**
		* Face the Scene Node and start moving towards it
		* @param destination ISceneNode to move to
		*/
		void follow(tle::ISceneNode* destination);
		/**
		* Update model position based on movement vector / collisions
		* @param kGameSpeed Global game speed
		* @param kDeltaTime Time elapsed since last frame
		*/
		bool updateScene(const float kGameSpeed, const float kDeltaTime, const float distanceToPlayer, bool ahead);

		void modifyMovementVector(SVector2D change);
		// Move model by movement vector
		void applyMovementVector(const float kDeltaTime = 1);
		// Reverse movement vector
		void bounce(Collision::CollisionAxis reverse = Collision::Both);
		// Stop following any target
		//void stop();

		// Reduce hover car health
		void reduceHealth(const int reduction = 1);
		// Called if car collision is detected
		void setCollided();
		// Reset state for next frame
		void resetCollided();

		const float getCollisionRadius() const;
		unsigned int getWaypointIndex() const;
		bool hasCollided() const;
		void reset();
		void resetWaypoint();
	private:
		const float kWaypointArrivalDistance = 4.0f;
		const float kCollisionRadius = 3.0f;
		// Bounce multiplier
		const float kBounce = 1.1f;
		const float kDrag = 0.87;
		// Much less health than players have
		const int kInitialHealth = 20;
		const int kSpeedHealthNerf = 10;

		SVector2D targetVector, facingVector;
		const float kInitialThrust = 7.0f;
		const float kNerfedThrust = 4.5f;
		float mThrust = kInitialThrust;
		unsigned int waypointIndex = 0;
		int mHealth = kInitialHealth;
		bool mCollided = false;
		const float kMaxRubberDistance = 100;
		const float kRubberDivider = -30;

		const float kInvTime = 5;
		float mInvTimer = 0;


		tle::ISceneNode* currentTargetNode = nullptr;
	};
};

#endif
