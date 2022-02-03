/**
 * Startup screen header file
 *
 * Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * University of Central Lancashire
 *
 * startup.h
 * 2021-04-14
 */

#ifndef DESERT_RACER_STARTUP_H
#define DESERT_RACER_STARTUP_H

#include <TL-Engine.h> // 3D engine
#include <windows.h> // Windows API (PlaySound)
#include <mmsystem.h> // PlaySound
#include <string>
#include <vector> // Hold afterimages

namespace desert
{
    class TLtendoStartup
    {
    public:
        // Set scene up, create afterimages
        TLtendoStartup(tle::I3DEngine* myEngine);
        // Removes all models, meshes, cameras
        void remove(tle::I3DEngine* myEngine);
        // Setup of the next frame
        bool setupFrame(tle::I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime);
        // Holds the camera created for this screen
        tle::ICamera* camera;

    protected:
        // Testing
        const bool testing = false;

        // Meshes Filenames //
        const string kMeshFilename = "TLtendo.x";
        const string kRingMeshFilename = "TLtendoRing.x";
        const string kDomeMeshFilename = "BlackDome.x";


        // Dome Setup //
        const float kDomeYOffset = -960.0f;


        // Camera Setup //
        // Needed cause shaders
        const float kCameraYOffset = 12;
        const float kCameraZOffset = 70;
        const float kCameraYRotation = 180;


        // Logo Setup //

        const float kLogoYOffset = 15.0f, kLogoZOffset = 20.0f;
        const float kLogoScale = 0.05f;

        const float kInitialXRotation = -90;
        const float kInitialYRotation = 60.0f;


        // Afterimages Setup //

        // Rotation offset
        const float kAfterImgOffset = 5.0f;

        // Number of after images to produce
        const int kAfterimagesNo = 10;

        const int kStaggeredRelease = 3;
        float cumulativeOffset = kAfterImgOffset;


        // Movement Variables //

        // Keep track of current model rotation
        float modelRotation = 0;

        // Initial speed
        const float kInitialSpeed = 100;

        // Built up speed
        float rotationSpeed = kInitialSpeed;

        // Acceleration
        float deltaRotation = 1;
        // Increase in Acceleration (aka Jerk)
        const float kJerkRotation = 0.0015f;


        // Timing Variables //

        // Time after which the logo rotation starts to accelerate
        const float kStartAccelerationTime = 4;

        float timeElapsed = 0.0f;

        // Frame after which the screen is considered over
        const int kScreenDuration = 12;

        int currentFrame = 0;


        // Motor audio name
        const LPCSTR kStartUpSound = "media\\audio\\roar.wav";


        // Meshes and models used //
        tle::IMesh* skyboxMesh;
        tle::IModel* skyboxModel;

        tle::IMesh* logoMesh;
        tle::IModel* mainModel;

        tle::IMesh* ringMesh;

        std::vector<tle::IModel*> afterimages;
    };
}

#endif
