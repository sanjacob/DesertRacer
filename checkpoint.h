/**
 * @file checkpoint.h
 * Handles checkpoints collision detection / effects
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#ifndef DESERT_RACER_CHECKPOINT_H
#define DESERT_RACER_CHECKPOINT_H

#include <TL-Engine.h>
#include "vector.h"
#include "node.h"


namespace desert
{
    /**
    * Checkpoint model wrapper class that handles collision detection &
    * cross indicators
    */
    class DesertCheckpoint : public BoxCollisionModel
    {
    public:
        // Cross indicator state
        enum CheckpointState
        {
            Uncrossed,
            RecentlyCrossed
        };

        static const std::string kDefaultModelName;

        /**
        * @param checkpointModel IModel pointer of checkpoint
        * @param alignment Model axis alignment type
        * @param crossMesh Pointer to mesh of cross indicator
        */
        DesertCheckpoint(tle::IModel* checkpointModel, NodeAlignment alignment, tle::IMesh* crossMesh);
        /**
        * Test collision with another object
        * @param position 2D vector of the other object
        * @param collisionRadius the collision radius of the other object
        */
        Collision::CollisionAxis collision(SVector2D position, const float collisionRadius = 0.0f, bool saveAxis = false);
        /**
        * Test collision with checkpoint box to see if it has been crossed
        * @param position of hover car
        */
        bool checkpointCollision(SVector2D position);
        // Update scene (hide/show cross indicator)
        void updateScene(const float kDeltaTime);
        // Show cross model
        void setCrossed();
        // Hide cross model
        void reset();

    protected:
        // Box collision params
        const float kHalfLength = 10.0f;
        const float kHalfWidth = 3.0f;
        // Strut collision params
        const float kStrutRadius = 1.0f;

        // For how long to show cross
        const float kCrossLifetime = 1.0f;

        // Cross model scale
        const float kCrossScale = 0.5f;

        // Cross Y parking positions
        const float kCrossActiveY = 4.0f, kCrossInactiveY = -8.0f;

        // Cross timer
        float crossElapsed = 0.0f;

        tle::IMesh* crossMesh;
        tle::IModel* crossModel;
        CheckpointState state = Uncrossed;
    };
}

#endif
