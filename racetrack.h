/**
 * @file racetrack.h
 * Contains all elements that make up the racetrack and handles interactions between them
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#ifndef DESERT_RACER_RACETRACK_H
#define DESERT_RACER_RACETRACK_H

#include <TL-Engine.h>
#include <string>
#include "checkpoint.h"
#include "scenery.h"
#include "racecar.h"
#include "ui.h"
#include "ai.h"


namespace desert
{
    /**
    * Holds all objects that make up the racetrack
    * (player, scenery, ai, checkpoints, etc)
    */
    class DesertRacetrack
    {
    public:
        // State of race
        enum RaceState
        {
            NotStarted,
            Starting,
            Transcurring,
            Over
        };

        /**
        * @param myEngine Pointer to TL-Engine running instance
        * @param sceneSetupFilename File containing models / positioning
        */
        DesertRacetrack(tle::I3DEngine* myEngine, string sceneSetupFilename);
        // Destroy racetrack, free memory
        ~DesertRacetrack();
        // Choose the correct object type for each model loaded
        void handleModel(tle::IModel* model, std::string type, NodeAlignment alignment, tle::IMesh* crossMesh);
        /** 
        * Setup next frame of scene
        * This includes handling user input, detecting collisions, etc
        * @param myEngine Pointer to running instance of TL-Engine
        * @param kGameSpeed Global game speed
        * @param kDeltaTime Time elapsed since last frame
        */
        void updateScene(tle::I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime);

        // Detect and handle checkpoint crossings (collisions)
        void detectCheckpointCrossings(const float kDeltaTime);

        // Reset scene to initial setup
        void reset();
        // Reset UI to initial layout
        void resetDialog();

        // Camera getter
        tle::ICamera* getCamera();

        // Returns number of checkpoints loaded (or stages per lap)
        int getStagesNumber() const;

        HoverCar* racecarPtr;
        RaceState raceState = Transcurring;

    protected:
        // Detect collisions between objects, handle user input, etc
        void updateOngoingRaceScene(tle::I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime);

        // Update UI based on current racecar status (boost indicators, speed)
        void updateUI();
        // Update health (only called when car is damaged)
        void updateHealthInUI();
        // Update laps (only called when car completes a lap)
        void updateLapsInUI();

        const SVector3D kFollowCamPosition;
        const SVector3D kFollowCamRotation = { 10, 0, 0 };

        const SVector3D kPovCamPosition = { 0, -10, 30 };
        const SVector3D kPovCamRotation;

        // Default DirectX mesh file extension
        const std::string kMeshFilesExtension = ".x";

        // Cross mesh filename (used for checkpoints)
        const std::string crossMeshFilename = "Cross" + kMeshFilesExtension;

        // Since text file does not contain Y positions, we just hold a couple of them
        // Default Y axis position for all models
        const float kDefaultY = 0;
        // Skybox Y axis position
        const float kSkyboxInitialY = -960;
        // Easter egg initial Y
        const float kCubeInitialY = 5;

        // Number of seconds to countdown before race starts
        const int kSecondsBeforeRace = 3;
        
        // Total number of laps
        const int kLaps = 1;
 
        // Don't load a mesh twice
        std::unordered_map<std::string, tle::IMesh*> mMeshes;
        // Checkpoints
        std::vector<DesertCheckpoint*> mCheckpoints;
        // Collision-enabled scenery (polymorphism ftw!)
        std::vector<CollisionModel*> mCollisionNodes;
        // Other scenery
        std::vector<tle::IModel*> mScenery;
        // AI & waypoints
        std::vector<HoverAI> mAI;
        std::vector<tle::IModel*> mWaypoints;

        // User interface (sprites / dialog)
        GameUI* uiPtr = nullptr;

        // Current camera
        tle::ICamera* currentCamera;

        // Start timer
        float timeElapsed = 0.0f;
        // Start countdown
        int secondsElapsed = 0;

        // Current lap
        int currentLap = 0;
        // Current checkpoint crossed
        int currentStage = 0;

        // Whether race ended because of car damage
        bool raceOverDueToDamage = false;
        // The car receives no further damage for any
        // additional frames it is in collision with something
        bool carCollidedLastFrame = false;
    };
}

#endif
