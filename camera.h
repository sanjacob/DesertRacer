/**
 * @file camera.h
 * Handle user input / switching between cameras
 * 
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#ifndef DESERT_RACER_CAMERA_H
#define DESERT_RACER_CAMERA_H

#include <TL-Engine.h>
#include "vector.h"
#include "node.h"
#include "keybinds.h"


namespace desert
{
    /**
    * Hold an ICamera* and its controls
    * Then, call control() to handle user input
    */
    class DesertCamera : public SceneNodeContainer
    {
    public:
        /**
        * @param cam Pointer to TL-Engine ICamera
        * @param switchToKey Key that switches view to this camera
        * @param controlEnabled If false, camera will be fixed
        * @param camKeybinding If control is enabled, these keybind will be used
        */
        DesertCamera(tle::ICamera* cam, tle::EKeyCode switchToKey, const bool controlEnabled = false, const SCameraKeybinding camKeybinding = kDefaultCameraBind);
        /**
        * Handle user input in the current frame
        * @param myEngine TL-Engine running instance
        * @param kGameSpeed Universal game speed constant
        * @param kDeltaTime The time elapsed since last frame
        */
        void control(tle::I3DEngine* myEngine, const float kGameSpeed = 1.0f, const float kDeltaTime = 1.0f);
        void setActive(bool active = true);
        bool isActive();
        // True if camera trigger key has been hit in the current frame
        bool isTriggered();
    protected:
        enum CameraState
        {
            Active,
            Inactive
        };

        // Movement and rotation speeds, respectively
        const float kCamSpeed = 40.0f, kRotationSpeed = 0.2f;
        const float kXRotationLimit = 20, kYRotationLimit = 40;

        // Camera movement / rotation controls
        SCameraKeybinding mKeybind;
        // Key to switch to this camera
        tle::EKeyCode mSwitchToKey;
        // Whether camera is currently being used
        CameraState mState = Inactive;

        float mRotationX = 0, mRotationY = 0;
        bool mRotationEnabled = false;
        bool mWasTriggered = false;
        bool mControllable;
    };
}
#endif