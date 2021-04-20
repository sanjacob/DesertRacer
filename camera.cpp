/**
 * @file camera.cpp
 * Handle user input / switching between cameras
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#include <TL-Engine.h>
#include <iostream>
#include "vector.h"
#include "node.h"
#include "camera.h"
#include "keybinds.h"

using namespace tle;
using namespace desert;


DesertCamera::DesertCamera(ICamera* cam, EKeyCode switchToKey = Key_0, const bool controlEnabled, const SCameraKeybinding camKeybinding) : 
	SceneNodeContainer(cam), camera(cam), mSwitchToKey(switchToKey), mControllable(controlEnabled), mKeybind(camKeybinding)
{
	cout << "Camera created" << endl;
}

ICamera* DesertCamera::getCamera()
{
	return camera;
}

void DesertCamera::control(I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime)
{
	// Reset triggered flag
	mWasTriggered = false;

	// If camera is active and controls are enabled
	if ((mState == Active) && mControllable)
	{
		const float kFrameSpeed = kGameSpeed * kDeltaTime;
		float cameraSpeed = kCamSpeed * kFrameSpeed;

		// Forwards / Backwards
		if (myEngine->KeyHeld(mKeybind.kCamForward))
		{
			node->MoveZ(cameraSpeed);
		}
		else if (myEngine->KeyHeld(mKeybind.kCamBackward))
		{
			node->MoveZ(-cameraSpeed);
		}

		// Left / Right
		if (myEngine->KeyHeld(mKeybind.kCamLeft))
		{
			node->MoveX(-cameraSpeed);
		}
		else if (myEngine->KeyHeld(mKeybind.kCamRight))
		{
			node->MoveX(cameraSpeed);
		}

		// Reset camera to original position / orientation
		if (myEngine->KeyHit(mKeybind.kCamReset))
		{
			// Reset local position
			resetLocalPosition();
			// Reset rotation
			node->RotateLocalX(-mRotationX);
			node->RotateY(-mRotationY);
			mRotationY = mRotationX = 0;
		}

		if (myEngine->KeyHit(mKeybind.kToggleRotation))
		{
			mRotationEnabled = !mRotationEnabled;
		}

		// Camera rotation with mouse
		if (mRotationEnabled) {
			int mouseMovementX = myEngine->GetMouseMovementX();
			int mouseMovementY = myEngine->GetMouseMovementY();

			float currentRotationX = mouseMovementY * kRotationSpeed * kFrameSpeed;
			float currentRotationY = mouseMovementX * kRotationSpeed * kFrameSpeed;

			float futureXRotation = mRotationX + currentRotationX;
			float futureYRotation = mRotationY + currentRotationY;

			if (futureXRotation < kXRotationLimit && futureXRotation > -kXRotationLimit)
			{
				mRotationX += currentRotationX;
				node->RotateLocalX(currentRotationX);
			}

			if (futureYRotation < kYRotationLimit && futureYRotation > -kYRotationLimit)
			{
				mRotationY += currentRotationY;
				node->RotateY(currentRotationY);
			}
		}
	}
	else if (mState != Active)
	{
		if (myEngine->KeyHeld(mSwitchToKey))
		{
			mState = Active;
			mWasTriggered = true;
		}
	}
}

void DesertCamera::setActive(bool active)
{
	if (!mWasTriggered)
	{
		mState = (active) ? Active : Inactive;
	}
}

bool DesertCamera::isActive()
{
	return mState == Active;
}

bool DesertCamera::isTriggered()
{
	return mWasTriggered;
}