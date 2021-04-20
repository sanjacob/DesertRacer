/**
 * @file ai.h
 * 
 * Non player-hover cars that follow waypoints.
 * 
 * In python, you would simply do
 * import ai 
 * ai.think()
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#ifndef DESERT_RACER_AI_H
#define DESERT_RACER_AI_H

#include <TL-Engine.h>
#include <iostream>
#include "vector.h"
#include "node.h"


namespace desert {
	/**
	* Non-player hover cars capable of following ISceneNodes, also offer collision detection
	*/
	class HoverAI : public SphereCollisionModel, public VectorBasedMovement
	{
	public:
		/**
		* @param model The hover car IModel
		*/
		HoverAI(tle::IModel* model);
		~HoverAI();
		/**
		* Face the Scene Node and start moving towards it
		* @param destination ISceneNode to move to
		*/
		void follow(tle::ISceneNode* destination, bool nextWaypoint = true);
		/**
		* Update model position based on movement vector / collisions
		* @param kGameSpeed Global game speed
		* @param kDeltaTime Time elapsed since last frame
		*/
		bool updateScene(const float kGameSpeed, const float kDeltaTime);
		// Move model by movement vector
		void applyMovementVector();
		// Reverse movement vector
		void bounce();
		// Stop following any target
		void stop();

		// Reduce hover car health
		void reduceHealth(const int reduction);
		// Called if car collision is detected
		void setCollided();
		// Reset state for next frame
		void resetCollided();

		const float getCollisionRadius() const;
		unsigned int getWaypointIndex() const;
		bool hasCollided() const;

	private:
		const float kWaypointArrivalDistance = 5.0f;
		const float kCollisionRadius = 3.0f;
		// Bounce multiplier for dramatic effect
		const float kBounce = 1.2f;
		// Much less health than players have
		const int kInitialHealth = 10;

		SVector2D currentTarget, facingVector;

		float movingSpeed = 4.0f;
		unsigned int waypointIndex = 0;
		int health = kInitialHealth;
		bool collided = false;
	};
};

#endif
