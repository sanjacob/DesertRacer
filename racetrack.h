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
#include "vehicle.h"
#include "particle.h"


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
        void handleModel(tle::IModel* model, std::string type, NodeAlignment alignment);
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
        RaceState raceState = NotStarted;

    protected:
        // Detect collisions between objects, handle user input, etc
        void updateOngoingRaceScene(tle::I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime);

        // Update UI based on current racecar status (boost indicators, speed)
        void updateUI(float kDeltaTime = 1.0f);
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
        const std::string kCrossMeshFilename = "Cross" + kMeshFilesExtension;
        const std::string kFlareMeshFilename = "Flare" + kMeshFilesExtension;

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
        const int kLaps = 3;

        const int kSecsInAnHour = 60 * 60;
        const int kMetersInKm = 1000;

        const tle::EKeyCode kFollowCamKey = Key_1;
        const tle::EKeyCode kPovCamKey = Key_2;

        const int kSetupXIndex = 1;
        const int kSetupZIndex = 2;
        const int kSetupYRotIndex = 3;
        const int kSetupYIndex = 4;
        const int kSetupXRotIndex = 5;
        const int kSetupZRotIndex = 6;
        const int kSetupScaleIndex = 7;

        const std::string barrelModel = "Barrel";
        const std::string dummyThiccModel = "Dummy";

        const SVector3D barrelOffset = { 0, 8, 0 };

        // Collision Radiuses
        std::unordered_map<std::string, float> kCustomCollisionRadius
        {
            { "Snowman", 8.0f },
            { "Moon", 180.0f },
            { "Barrel", 6.0f },
            { "TankSmall1", 3.0f },
            { "TankSmall2", 3.0f }
        };

        std::vector<std::string> racecarSkins
        {
            "ai_red.png",
            "ai_blue.png",
            "ai_purple.png",
            "ai_teal.png"
        };
 
        // Don't load a mesh twice
        std::unordered_map<std::string, tle::IMesh*> mMeshes;
        // Checkpoints
        std::vector<DesertCheckpoint*> mCheckpoints;
        // Collision-enabled scenery (polymorphism ftw!)
        std::vector<CollisionModel*> mCollisionNodes;
        // Other scenery
        std::vector<tle::IModel*> mScenery;
        // AI & waypoints
        std::vector<HoverAI*> mAI;
        std::vector<tle::IModel*> mWaypoints;
        // Numbering
        std::vector<int> mOrdinals;
        std::vector<DesertVehicle*> mVehicles;

        // Particle Systems
        std::vector<ParticleSystem*> mParticles;

        // User interface (sprites / dialog)
        GameUI* uiPtr = nullptr;

        // Current camera
        tle::ICamera* currentCamera;

        tle::IMesh* crossMesh;
        tle::IMesh* flareMesh;

        Collision::CollisionAxis mLastCollisionAxis;

        // Start timer
        float timeElapsed = 0.0f;
        // Start countdown
        int secondsElapsed = 0;

        // Race timer
        float raceElapsed = 0.0f;

        // Whether race ended because of car damage
        //bool raceOverDueToDamage = false;
        // The car receives no further damage for any
        // additional frames it is in collision with something
        bool carCollidedLastFrame = false;
    };
}

#endif
