/**
 * Setup for the TL-tendo logo startup screen
 *
 * Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * University of Central Lancashire
 *
 * startup.cpp
 * 2021-04-14
 */

#include <TL-Engine.h>
#include "startup.h"
#include <iostream>

using namespace tle;
using namespace desert;


TLtendoStartup::TLtendoStartup(tle::I3DEngine* myEngine)
{
	camera = myEngine->CreateCamera(kManual, 0, kCameraYOffset, kCameraZOffset);
	camera->RotateY(kCameraYRotation);

	if (!testing)
	{
		skyboxMesh = myEngine->LoadMesh(kDomeMeshFilename);
		skyboxModel = skyboxMesh->CreateModel(0, kDomeYOffset, 0);

		logoMesh = myEngine->LoadMesh(kMeshFilename);
		ringMesh = myEngine->LoadMesh(kRingMeshFilename);

		mainModel = logoMesh->CreateModel(0, kLogoYOffset, kLogoZOffset);
		mainModel->RotateX(kInitialXRotation);
		mainModel->Scale(kLogoScale);

		for (int i = 0; i < kAfterimagesNo; i++)
		{
			IModel* afterimg = ringMesh->CreateModel();
			afterimg->AttachToParent(mainModel);
			//afterimg->Scale(kLogoScale);
			afterimages.push_back(afterimg);
		}

		PlaySound(kStartUpSound, NULL, SND_FILENAME | SND_ASYNC);

		mainModel->RotateY(kInitialYRotation);
		modelRotation += kInitialYRotation;
	}
}

void TLtendoStartup::remove(I3DEngine* myEngine)
{

	if (!testing)
	{
		for (IModel* afterImg : afterimages)
		{
			ringMesh->RemoveModel(afterImg);
		}

		myEngine->RemoveMesh(ringMesh);

		logoMesh->RemoveModel(mainModel);
		myEngine->RemoveMesh(logoMesh);

		skyboxMesh->RemoveModel(skyboxModel);
		myEngine->RemoveMesh(skyboxMesh);
	}

	cout << "Removed startup screen scene" << endl;
};

bool TLtendoStartup::setupFrame(I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime)
{
	if (testing)
		return true;

	timeElapsed += kDeltaTime;

	float currentRotationSpeed = kInitialSpeed;

	// Accelerate first, apply later
	rotationSpeed += deltaRotation;

	if (timeElapsed >= kStartAccelerationTime)
	{
		currentRotationSpeed = rotationSpeed;

		// Increase acceleration, increase rotation
		deltaRotation += kJerkRotation;
		modelRotation += (currentRotationSpeed * kDeltaTime);

		int i = 0;
		for (IModel* &afterImg : afterimages)
		{
			if (currentFrame == (i * kStaggeredRelease))
			{
				afterImg->RotateZ(-cumulativeOffset);
				cumulativeOffset += kAfterImgOffset;
			}

			++i;
		}

		++currentFrame;
	}
	else
	{
		modelRotation += (currentRotationSpeed * kDeltaTime);
	}

	mainModel->RotateY(currentRotationSpeed * kDeltaTime);
	return (timeElapsed > kScreenDuration);
}
