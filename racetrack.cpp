/**
 * @file racetrack.cpp
 * Contains all elements that make up the racetrack and handles interactions between them
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#include <TL-Engine.h>
#include <iostream>
#include <vector>
#include <unordered_map> // Mesh storage

#include "node.h"
#include "racetrack.h"
#include "racecar.h"
#include "files.h"
#include "ui.h"
#include "ai.h"
#include "keybinds.h"
#include "scenery.h"

using namespace std;
using namespace tle;
using namespace desert;


// Set up scene and create objects
DesertRacetrack::DesertRacetrack(I3DEngine* myEngine, string sceneSetupFilename)
{
	// Choose keybinds
	const SControlKeybinding controlKeybind = kDefaultDvorakBind.kControlKeybind;

	// Get setup file lines
	vector<string> setupLines = Files::getLinesFromFile(sceneSetupFilename);

	// Create racecar cameras
	ICamera* followCam = myEngine->CreateCamera();
	ICamera* povCam = myEngine->CreateCamera();
	currentCamera = followCam;

	// Load cross mesh
	IMesh* crossMesh = myEngine->LoadMesh(crossMeshFilename);

	bool playerLoaded = false;
	
	// For every model in scene
	for (string setupLine : setupLines)
	{
		vector<string> setupElements = Files::splitLine(setupLine);
		const string meshFilename = setupElements[0];

		const float initialX = stof(setupElements[1]), initialZ = stof(setupElements[2]), rotation = stof(setupElements[3]);
		float initialY = kDefaultY;

		cout << "Loading model of " << meshFilename << endl;

		// If mesh has already been found, do not load it again
		if (!mMeshes.count(meshFilename))
		{
			mMeshes[meshFilename] = myEngine->LoadMesh(meshFilename + kMeshFilesExtension);
		}

		// Set special cases Y axis
		if (meshFilename == "Skybox")
		{
			initialY = kSkyboxInitialY;
		}
		else if (meshFilename == "Cube")
		{
			initialY = kCubeInitialY;
		}

		// Create model in specified location
		IModel* model = mMeshes[meshFilename]->CreateModel(initialX, initialY, initialZ);
		// Rotate if necessary
		model->RotateY(rotation);

		// Detect model axis alignment
		NodeAlignment alignment = SceneNodeContainer::getAlignmentFromRotation(rotation);

		if (!playerLoaded && meshFilename == "race2")
		{
			followCam->AttachToParent(model);
			povCam->AttachToParent(model);

			DesertCamera followDesertCam = DesertCamera(followCam, Key_1, true, kDefaultCameraBind);
			DesertCamera povDesertCam = DesertCamera(povCam, Key_2);

			followDesertCam.moveLocallyByVector(kFollowCamPosition);
			followDesertCam.rotateLocallyByVector(kFollowCamRotation);

			povDesertCam.moveLocallyByVector(kPovCamPosition);
			povDesertCam.rotateLocallyByVector(kPovCamRotation);

			racecarPtr = new HoverCar(model, controlKeybind);
			racecarPtr->AddCamera(followDesertCam);
			racecarPtr->AddCamera(povDesertCam);
			playerLoaded = true;
		}
		else
		{
			handleModel(model, meshFilename, alignment, crossMesh);
		}
	}

	// Create UI
	uiPtr = new GameUI(myEngine);
	
	// Initialise UI elements
	resetDialog();
	updateUI();

	cout << "Racetrack created" << endl;
}

DesertRacetrack::~DesertRacetrack()
{
	// Delete racecar
	delete racecarPtr;
	racecarPtr = nullptr;

	// Delete UI
	delete uiPtr;
	uiPtr = nullptr;

	// Delete all collision model pointers
	for (CollisionModel* node : mCollisionNodes)
	{
		delete node;
		node = nullptr;
	}

	cout << "Racetrack destroyed" << endl;
}

void DesertRacetrack::handleModel(IModel* model, string type, NodeAlignment alignment, IMesh* crossMesh)
{
	// Model is a non-player hover car
	if (type == "race2")
	{
		mAI.push_back(HoverAI(model));
		mAI.back().follow(mWaypoints.front());
	}
	// Model is a checkpoint
	else if (type == "Checkpoint")
	{
		mCheckpoints.push_back(new DesertCheckpoint(model, alignment, crossMesh));
		// Strut collision detection as collision node
		mCollisionNodes.push_back(mCheckpoints.back());
	}
	// Dummy waypoint for AI
	else if (type == "Dummy")
	{
		mWaypoints.push_back(model);
	}
	// Model is a wall
	else if (type == "Wall")
	{
		mCollisionNodes.push_back(new DesertWall(model, alignment));
	}
	// Model is a tank
	else if (type == "TankSmall1" || type == "TankSmall2")
	{
		mCollisionNodes.push_back(new DesertTank(model));
	}
	// Just throw it somewhere
	else
	{
		mScenery.push_back(model);
	}
}

void DesertRacetrack::updateScene(I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime)
{
	// Race has not started
	if (raceState == NotStarted)
	{
		// Wait until user presses start key
		if (myEngine->KeyHit(kDefaultMetaBind.kStartGame))
		{
			raceState = Starting;
		}
	}
	// User pressed start key
	else if (raceState == Starting)
	{
		// Add a second every time the timer passes one second, reset timer
		if (timeElapsed > 1)
		{
			++secondsElapsed;
			timeElapsed = 0.0f;
		}

		// If countdown still on
		if (secondsElapsed < kSecondsBeforeRace)
		{
			// If no time has elapsed
			if (!timeElapsed)
			{
				// Display seconds remaining for a second
				uiPtr->displayText(to_string(kSecondsBeforeRace - secondsElapsed), false, 1);
			}
		}
		// Countdown ended
		else
		{
			// Display go indefinitely
			uiPtr->displayText("Go!", false, 0);
			// Race has started, update state
			raceState = Transcurring;
			// Reset seconds elapsed for next cycle
			secondsElapsed = 0;
			// Since time is added later, in order for
			// timeElapsed to be zero this subtraction is needed
			timeElapsed = -kDeltaTime;
		}
		
		// Add time to counter
		timeElapsed += kDeltaTime;
	}
	else if (raceState == Transcurring)
	{
		updateOngoingRaceScene(myEngine, kGameSpeed, kDeltaTime);
	}
	// Race has ended
	else if (raceState == Over)
	{
		// Check cause of race ending 
		if (raceOverDueToDamage)
		{
			// Update dialog
			uiPtr->displayText("Your car is done for :(", false, 0, "Press R to Restart :)");
		}

		// Wait until user presses restart key
		if (myEngine->KeyHit(kDefaultMetaBind.kRestartGame))
		{
			// Restart race
			raceState = NotStarted;
			reset();
		}
	}

	// Draw UI (text)
	uiPtr->drawGameUI(kDeltaTime);
}

void DesertRacetrack::detectCheckpointCrossings(const float kDeltaTime)
{
	// We still need the index
	unsigned int i = 0;

	for (DesertCheckpoint* checkpoint : mCheckpoints)
	{
		// Box collision
		if (checkpoint->checkpointCollision(racecarPtr->position2D()))
		{
			// If checkpoint crossed is the next one
			if (currentStage % getStagesNumber() == i)
			{
				// Display cross
				checkpoint->setCrossed();
				// Increment stage state
				currentStage++;

				// First checkpoint
				if (!i)
				{
					currentLap++;

					// Player has completed the race
					if (currentLap > kLaps)
					{
						// Update race state
						raceState = Over;

						// Update UI
						uiPtr->displayText("Race finished", true, 0, "Press R to Restart, Esc to Exit");
						uiPtr->toggleLapSprite(false);
					}
					// Lap completed
					else
					{
						string lapDisplayText = "Final Lap";

						if (currentLap != kLaps)
						{
							lapDisplayText = "Lap " + to_string(currentLap);
						}

						// Update UI
						uiPtr->displayText(lapDisplayText, false, 1.0f);
						updateLapsInUI();
					}
				}

				// Other checkpoint or it's the first lap
				if (i || currentLap == 1)
				{
					// Show stage completion dialog
					string dispText = "Stage " + to_string(currentStage) + " complete";
					uiPtr->displayText(dispText, false, 1.0f);
				}
			}
		}

		// Update cross position
		checkpoint->updateScene(kDeltaTime);
		i++;
	}
}

void DesertRacetrack::updateOngoingRaceScene(I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime)
{
	// To avoid cancelling the car's vector twice, therefore not cancelling it at all,
	// only count one collision per frame
	bool carHasCollided = false;

	// Handle racecar user input
	racecarPtr->control(myEngine, kGameSpeed, kDeltaTime);
	// Update camera
	currentCamera = racecarPtr->currentCamera;

	// Update AI movement
	for (HoverAI& hoverAI : mAI)
	{
		// If AI has reached its current target
		if (hoverAI.updateScene(kGameSpeed, kDeltaTime))
		{
			// Calculate next waypoint
			unsigned int nextWaypointI = hoverAI.getWaypointIndex();

			if (mWaypoints.size() > nextWaypointI)
			{
				// Set AI's next target
				hoverAI.follow(mWaypoints[nextWaypointI]);
			}
			// AI has reached the last waypoint
			else
			{
				hoverAI.stop();
			}
		}
	}

	// Checkpoint crossing detection & handling
	detectCheckpointCrossings(kDeltaTime);

	/** First, detect collision for
	* - Player
	* - AI
	* 
	* Against
	* 
	* - Walls
	* - Tanks
	* - Struts of checkpoints
	* - AI (yes, AI against AI too)
	*/
	for (CollisionModel* node : mCollisionNodes)
	{
		// Test for collision
		if (!carHasCollided && node->collision(racecarPtr->position2D(), racecarPtr->getCollisionRadius()))
		{
			carHasCollided = true;
		}

		for (HoverAI& hoverAI : mAI)
		{
			if (node->collision(hoverAI.position2D(), hoverAI.getCollisionRadius()))
			{
				hoverAI.setCollided();
			}
		}
	}

	/**
	* Now, handle collision effects
	*/
	// Only count damages if non contiguous
	if (carHasCollided && !carCollidedLastFrame)
	{
		// Cancel vector out
		racecarPtr->bounce();
		// Reduce health, update UI
		racecarPtr->reduceHealth();
		updateHealthInUI();

		// If car has no more health, stop race
		if (!racecarPtr->getHealth())
		{
			raceOverDueToDamage = true;
			raceState = Over;
		}
	}

	// After possibly cancelling movement vector out, apply result
	racecarPtr->applyMovementVector();

	// Handle AI collisions
	for (HoverAI& hoverAI : mAI)
	{
		if (hoverAI.hasCollided())
		{
			// Cancel AI vector
			hoverAI.bounce();
			hoverAI.resetCollided();
		}

		// Apply movement vector result
		hoverAI.applyMovementVector();

		// Recalculate route after collision
		if (hoverAI.hasCollided())
		{
			hoverAI.follow(mWaypoints[hoverAI.getWaypointIndex() - 1], false);
		}
	}

	updateUI();
	carCollidedLastFrame = carHasCollided;
}

void DesertRacetrack::reset()
{
	// Reset race progress info
	currentStage = 0;
	currentLap = 0;
	// Reset car position and movement
	racecarPtr->reset();
	// Reset UI
	resetDialog();

	// Hide all crosses
	for (DesertCheckpoint* checkpoint : mCheckpoints)
	{
		checkpoint->reset();
	}

	// Reset AI to original position and waypoint
	for (HoverAI& ai : mAI)
	{
		//ai.reset();
	}
}

ICamera* DesertRacetrack::getCamera()
{
	return currentCamera;
}

int DesertRacetrack::getStagesNumber() const
{
	return mCheckpoints.size();
}

////////////////////
// UI
////////////////////

void DesertRacetrack::updateUI()
{
	// Hide all boost indicators
	uiPtr->toggleBoostIndicator(false);
	uiPtr->toggleWarnIndicator(false);
	uiPtr->toggleOverheatWarning(false);

	// Show only the active one
	switch (racecarPtr->getBoostState())
	{
	case HoverCar::BoostState::Active:
		uiPtr->toggleBoostIndicator(true);
		break;
	case HoverCar::BoostState::Warning:
		uiPtr->toggleWarnIndicator(true);
		break;
	case HoverCar::BoostState::Penalty:
		uiPtr->toggleOverheatWarning(true);
		break;
	default:
		break;
	}

	// Update speed
	uiPtr->setSpeedText(to_string(racecarPtr->getSpeed()));
}

void DesertRacetrack::updateLapsInUI()
{
	uiPtr->setLapText(to_string(currentLap) + "/" + to_string(kLaps));
}

void DesertRacetrack::updateHealthInUI()
{
	uiPtr->setHealthText(to_string(racecarPtr->getHealth()));
}

void DesertRacetrack::resetDialog()
{
	uiPtr->displayText("Hit Space to Start", false, 0);
}