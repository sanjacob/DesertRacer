/**
 * @file racecar.h
 * Player hover car controls
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#ifndef DESERT_RACER_CAR_H
#define DESERT_RACER_CAR_H

#include <TL-Engine.h>
#include <math.h>
#include <vector>
#include "keybinds.h"
#include "vector.h"
#include "node.h"
#include "camera.h"


namespace desert
{
    class HoverCar : public SceneNodeContainer, public VectorBasedMovement
    {
    public:
        static enum BoostState
        {
            Inactive,
            Active,
            Warning,
            Penalty
        };

        HoverCar(tle::IModel* model, const SControlKeybinding carKeybinding);
        ~HoverCar();
        void addCamera(DesertCamera cam);
        void control(tle::I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime);
        float processBoost(tle::I3DEngine* myEngine, const float kDeltaTime);
        void processThrust(tle::I3DEngine* myEngine, const float frameSpeed, const float boostMultiplier);
        int processTurn(tle::I3DEngine* myEngine, const float frameSpeed);
        void processLean(const float frameSpeed, const int turnMultiplier);
        void processBobble(const float kGameSpeed, const float kDeltaTime);
        void controlCameras(tle::I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime);
        void reduceHealth(const int reduction = 1);
        void applyMovementVector();
        void bounce();
        void reset();

        tle::ICamera* getCamera();
        BoostState getBoostState() const;
        const float getCollisionRadius() const;
        int getHealth() const;
        int getSpeed() const;
    protected:
        // Car forwards / backwards movement state
        enum HoverCarState
        {
            Stationary,
            Moving
        };

        // Hover car L/R turning state
        enum HoverCarInclination
        {
            NotTurning,
            Turning
        };

        // Control keybinds
        const SControlKeybinding mKeybind;

        // MOVEMENT //

        // Thrust speed
        const float kThrust = 5.0f;
        // Multiplier when using backwards thrust
        const float kBackwardThrustMultiplier = -0.5;
        // Rotation speed
        const float kInitialRotation = 200.0f;

        // Movement vector is multiplied by drag every frame
        const float kDrag = 0.96f;
        
        // Bouncing multiplier
        const float kBounce = 1.0f;

        // BOOST //

        // Thrust will be multiplied by this when boost is active
        const float kBoost = 1.5f;

        // Boost maximum continuous use, time penalty and warning period
        const float kBoostMaxTimeActive = 3.0f, kBoostTimePenalty = 5.0f, kBoostWarningTime = 1.0f;
        
        // Minimum health to use boost
        const float kBoostMinimumHealth = 30.0f;
        
        // SIN WAVE //

        // Initial Y Offset
        const float kModelYOffset = 3.0f;
        // Speed to reset Y position
        const float kResetYSpeed = 4.0f;
        // Modifies sin wave amplitude (aka bumpiness)
        const float kSinFunctionMultiplier = 5.0f;

        // LEAN //

        // Speed at which car leans into the bends
        const float kLeaningSpeed = 40.0f;
        // Speed at which to reset lean
        const float kResetLeanSpeed = 30.0f;
        // Max leaning angle
        const float kMaxInclination = 20.0f;
        
        // REAR LIFT //
    
        // Speed at which rear is lifted
        const float kRearLiftSpeed = 5.0f;
        // Speed at which rear lift is reset (resetted?)
        const float kResetRearSpeed = 5.0f;
        // Max rear lift angle
        const float kMaxRearLift = 10.0f;
        
        // HEALTH //

        // Initial health
        const int kInitialHealth = 100;
        // Rotation speed after a certain damage
        const float kNerfedRotation = 70.0f;
        // Health at which rotation nerf will take effect
        const int kHealthSteerNerf = 40;


        // Collision radius
        const float kCollisionRadius = 3.0f;

        // Cameras that move with the car
        std::vector<DesertCamera> cameras;
        // Pointer to currently active camera
        tle::ICamera* mCurrentCamera;

        HoverCarState carState = Stationary;
        HoverCarInclination inclinationState = NotTurning;
        BoostState boostState = Inactive;

        float timeElapsedMoving = 0.0f;
        float rotationSpeed = kInitialRotation;
        float boostTimer = 0.0f, boostPenaltyTimer = 0.0f;
        float boostDragMultiplier = 1;
        float rearLift = 0.0f;
        float lean = 0.0f;
        int health = kInitialHealth;
    };
}

#endif
