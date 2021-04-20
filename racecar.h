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
        void reduceHealth(const int reduction = 1);
        void applyMovementVector();
        void bounce();
        void reset();

        ICamera* getCamera();
        BoostState getBoostState() const;
        const float getCollisionRadius() const;
        int getHealth() const;
        int getSpeed() const;
    protected:
        enum HoverCarState
        {
            Stationary,
            Moving
        };

        enum HoverCarInclination
        {
            NotTurning,
            Turning
        };

        const SControlKeybinding keybind;
        HoverCarState carState = Stationary;
        HoverCarInclination inclinationState = NotTurning;
        BoostState boostState = Inactive;
        tle::ICamera* currentCamera;

        const float kThrust = 5.0f, kBackwardThrustMultiplier = -0.5;
        const float kDrag = 0.96f, kInitialRotation = 200.0f;
        float boostDragMultiplier = 1;
        const float kBounce = 1.0f;
        const float kNerfedRotation = 70.0f;
        float rotationSpeed = kInitialRotation;
        float timeElapsedMoving = 0.0f;
        float boostTimer = 0.0f, boostPenaltyTimer = 0.0f;
        const float kBoostMaxTimeActive = 3.0f, kBoostTimePenalty = 5.0f, kBoostWarningTime = 1.0f, kBoostMinimumHealth = 30.0f;
        const float kBoost = 1.5f;
        
        const float kModelYOffset = 3.0f;
        const float kSinFunctionMultiplier = 5.0f;
        const float kResetYSpeed = 4.0f;

        const float kLeaningSpeed = 40.0f;
        const float kResetLeanSpeed = 30.0f;
        const float kMaxInclination = 20.0f;
        float lean = 0.0f;

        const float kRearLiftSpeed = 5.0f;
        const float kResetRearSpeed = 5.0f;
        const float kMaxRearLift = 10.0f;
        float rearLift = 0.0f;

        const float kCollisionRadius = 3.0f;
        const int kInitialHealth = 100;
        int health = kInitialHealth;
        const int kHealthSteerNerf = 40;

        std::vector<DesertCamera> cameras;
    };
}

#endif
