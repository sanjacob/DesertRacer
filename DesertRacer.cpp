/**
 * @file DesertRacer.cpp
 * A Racing game using the TL-Engine
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 * @date April, 2021
 */

#include <TL-Engine.h>

#include <iostream> // cout
#include <vector> // 
#include <unordered_map> // Mesh storage

#include "keybinds.h" // Default game keybinds
#include "racetrack.h" // Racetrack class
#include "startup.h" // Startup screen

// Standard library
using namespace std;
// TL-Engine namespace
using namespace tle;
// Desert Racer namespace
using namespace desert;


enum GameState
{
	Playing,
	Paused
};

void main()
{
	// Determines the speed of all elements in the game
	const float kGameSpeed = 1;
	// Folder containing meshes and other assets for the game
	const string mediaFolder = "media";
	// Text file containing the names of models used and their initial position
	const string sceneSetupFilename = mediaFolder + "\\SceneSetup.txt";

	GameState state = Playing;

	// Create a 3D engine
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add folder for meshes and other media
	myEngine->AddMediaFolder(mediaFolder);

	// Create startup screen
	TLtendoStartup startupScreen = TLtendoStartup(myEngine);
	// Pointer to a racetrack
	DesertRacetrack* defaultTrack = nullptr;

	// Pointer to currently active camera
	ICamera* currentCamera = startupScreen.camera;
	bool startupScreenEnded = false;

	// Game pause indicator
	IFont* defaultFont = myEngine->LoadFont("Comic Sans MS");
	const int kPausedTextOffset = 50;

	bool mouseCaptureOn = true;
	myEngine->StartMouseCapture();
	
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Frame timing
		const float kDeltaTime = myEngine->Timer();
		// Draw the scene
		myEngine->DrawScene(currentCamera);

		// STARTUP SCREEN
		if (!startupScreenEnded)
		{
			startupScreenEnded = startupScreen.setupFrame(myEngine, kGameSpeed, kDeltaTime);

			if (startupScreenEnded)
			{
				// Clean up startup screen
				startupScreen.remove(myEngine);
				// Create racetrack scene
				defaultTrack = new DesertRacetrack(myEngine, sceneSetupFilename);
			}
		}
		else if (state == Playing)
		{
			// Setup the current frame
			defaultTrack->updateScene(myEngine, kGameSpeed, kDeltaTime);
			// Update camera pointer
			currentCamera = defaultTrack->getCamera();
		}
		else
		{
			defaultFont->Draw("PAUSED", myEngine->GetWidth() / 2, myEngine->GetHeight() - kPausedTextOffset, kBlack, kCentre, kVCentre);
		}

		// Toggle mouse capture
		if (myEngine->KeyHit(kDefaultMetaBind.kToggleMouseCapture))
		{
			if (mouseCaptureOn)
			{
				myEngine->StopMouseCapture();
			}
			else
			{
				myEngine->StartMouseCapture();
			}

			mouseCaptureOn = !mouseCaptureOn;
		}

		// Toggle pause
		if (myEngine->KeyHit(kDefaultMetaBind.kPauseGame))
		{
			if (state == Paused)
			{
				state = Playing;
			}
			else if (state == Playing)
			{
				state = Paused;
			}
		}

		// Quit game
		if (myEngine->KeyHit(kDefaultMetaBind.kQuitGame))
		{
			myEngine->Stop();
		}
	}

	// Free up memory
	delete defaultTrack;
	defaultTrack = nullptr;

	// Not sure if this is even necessary tbh
	myEngine->StopMouseCapture();

	// Delete the 3D engine
	myEngine->Delete();
}
