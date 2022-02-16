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
#include <algorithm>

#include "node.h"
#include "racetrack.h"
#include "racecar.h"
#include "files.h"
#include "ui.h"
#include "ai.h"
#include "keybinds.h"
#include "scenery.h"
#include "collision.h"
#include "particle.h"

using namespace std;
using namespace tle;
using namespace desert;


// Set up scene and create objects
DesertRacetrack::DesertRacetrack(I3DEngine* myEngine, string sceneSetupFilename, SControlKeybinding controlKeybind)
{
	// Get setup file lines
	vector<string> setupLines = Files::getLinesFromFile(sceneSetupFilename);

	// Create racecar cameras
	ICamera* followCam = myEngine->CreateCamera();
	ICamera* povCam = myEngine->CreateCamera();
	currentCamera = followCam;

	// Load cross mesh
	crossMesh = myEngine->LoadMesh(kCrossMeshFilename);
	flareMesh = myEngine->LoadMesh(kFlareMeshFilename);

	bool playerLoaded = false;

	// For every model in scene
	for (string setupLine : setupLines)
	{
		vector<string> setupElements = Files::splitLine(setupLine);
		const string meshFilename = setupElements.at(0);
		float initialY = kDefaultY, xRotation = 0, zRotation = 0, scale = 1;

		const float initialX = stof(setupElements.at(XIndex));
		const float initialZ = stof(setupElements.at(ZIndex));
		const float yRotation = stof(setupElements.at(YRotIndex));

		if (setupElements.size() > YIndex)
		{
			initialY = stof(setupElements.at(YIndex));
		}

		if (setupElements.size() > XRotIndex)
		{
			xRotation = stof(setupElements.at(XRotIndex));
		}

		if (setupElements.size() > ZRotIndex)
		{
			zRotation = stof(setupElements.at(ZRotIndex));
		}

		if (setupElements.size() > ScaleIndex)
		{
			scale = stof(setupElements.at(ScaleIndex));
		}

		cout << "Loading model of " << meshFilename << endl;

		// If mesh has already been found, do not load it again
		if (!mMeshes.count(meshFilename))
		{
			mMeshes[meshFilename] = myEngine->LoadMesh(meshFilename + kMeshFilesExtension);
		}

		// Create model in specified location
		IModel* model = mMeshes[meshFilename]->CreateModel(initialX, initialY, initialZ);
		// Rotate if necessary
		model->RotateY(yRotation);
		model->RotateX(xRotation);
		model->RotateZ(zRotation);
		model->Scale(scale);

		// Detect model axis alignment
		const NodeAlignment alignment = SceneNodeContainer::getAlignmentFromRotation(yRotation);

		if (!playerLoaded && meshFilename == "Racecar")
		{
			followCam->AttachToParent(model);
			povCam->AttachToParent(model);

			DesertCamera followDesertCam = DesertCamera(followCam, kFollowCamKey, true, kDefaultCameraBind);
			DesertCamera povDesertCam = DesertCamera(povCam, kPovCamKey);

			followDesertCam.moveLocallyByVector(kFollowCamPosition);
			followDesertCam.rotateLocallyByVector(kFollowCamRotation);

			povDesertCam.moveLocallyByVector(kPovCamPosition);
			povDesertCam.rotateLocallyByVector(kPovCamRotation);

			racecarPtr = new HoverCar(model, controlKeybind, flareMesh);
			racecarPtr->addCamera(followDesertCam);
			racecarPtr->addCamera(povDesertCam);
			mVehicles.push_back(racecarPtr);
			playerLoaded = true;
		}
		else
		{
			handleModel(model, meshFilename, alignment);
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

	for (ParticleSystem* pSystem : mParticles)
	{
		delete pSystem;
		pSystem = nullptr;
	}
}

void DesertRacetrack::remove(I3DEngine* myEngine)
{
	for (auto& it : mMeshes) {
		myEngine->RemoveMesh(it.second);
	}

	myEngine->RemoveMesh(crossMesh);
	myEngine->RemoveMesh(flareMesh);
	
	uiPtr->remove(myEngine);
}

void DesertRacetrack::handleModel(IModel* model, string type, NodeAlignment alignment)
{
	// Model is a non-player hover car
	if (type == HoverCar::kDefaultModelName)
	{
		model->SetSkin(racecarSkins.at(mAI.size()));
		mAI.push_back(new HoverAI(model, "CPU #" + to_string(mAI.size() + 1)));
		mAI.back()->follow(mWaypoints.front());
		mCollisionNodes.push_back(mAI.back());
		mVehicles.push_back(mAI.back());
	}
	// Model is a checkpoint
	else if (type == DesertCheckpoint::kDefaultModelName)
	{
		mCheckpoints.push_back(new DesertCheckpoint(model, alignment, crossMesh));
		// Strut collision detection as collision node
		mCollisionNodes.push_back(mCheckpoints.back());
	}
	// Dummy waypoint for AI
	else if (type == dummyThiccModel)
	{
		mWaypoints.push_back(model);
	}
	// Model is a wall
	else if (type == DesertWall::kDefaultModelName)
	{
		mCollisionNodes.push_back(new DesertWall(model, alignment));
	}
	else if (type == DesertTower::kDefaultModelName)
	{
		mCollisionNodes.push_back(new DesertTower(model, alignment));
	}
	else if (kCustomCollisionRadius.count(type))
	{
		mCollisionNodes.push_back(new CustomSphereCModel(model, kCustomCollisionRadius[type]));
	}
	// Just throw it somewhere
	else
	{
		mScenery.push_back(model);
	}

	if (type == barrelModel)
	{
		SVector3D p = mCollisionNodes.back()->position();
		p += barrelOffset;
		mParticles.push_back(new FireParticleSystem(flareMesh, p));
		mParticles.back()->setup();
	}
}

void DesertRacetrack::updateScene(I3DEngine* myEngine, const float kGameSpeed, const float kDeltaTime)
{
	for (ParticleSystem* p : mParticles)
	{
		p->updateSystem(kDeltaTime);
	}

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
			uiPtr->togglePosition(true);
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
		raceElapsed += kDeltaTime;
		updateOngoingRaceScene(myEngine, kGameSpeed, kDeltaTime);
	}
	// Race has ended
	else if (raceState == Over)
	{
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
		for (DesertVehicle* vehicle : mVehicles)
		{
			if (checkpoint->checkpointCollision(vehicle->position2D()))
			{
				if (vehicle->getCurrentStage() % getStagesNumber() == i)
				{
					// Increment stage state
					vehicle->nextStage();

					if (vehicle->isPlayer())
					{
						// Display cross
						checkpoint->setCrossed();
					}

					// First checkpoint
					if (!i)
					{
						vehicle->nextLap();

						// Player has completed the race
						if (vehicle->getCurrentLap() > kLaps)
						{
							vehicle->resetStage();
							// Update race state
							raceState = Over;
							uiPtr->toggleSummary(true);
							uiPtr->setSummaryTime(to_string(raceElapsed));
							uiPtr->setSummaryWinner(vehicle->getTag());
							raceElapsed = 0.0f;

							if (vehicle->isPlayer())
							{
								// Update UI
								uiPtr->displayText("Race finished", true, 0, "Press R to Restart, Esc to Exit");
								uiPtr->toggleLapSprite(false);
							}
						}
						// Lap completed
						else if (vehicle->isPlayer())
						{
							string lapDisplayText = "Final Lap";

							if (vehicle->getCurrentLap() != kLaps)
							{
								lapDisplayText = "Lap " + to_string(vehicle->getCurrentLap());
							}

							// Update UI
							uiPtr->displayText(lapDisplayText, false, 1.0f);
							updateLapsInUI();
						}
					}

					// Other checkpoint or it's the first lap
					if (vehicle->isPlayer() && (i || vehicle->getCurrentLap() == 1))
					{
						// Show stage completion dialog
						string dispText = "Stage " + to_string(vehicle->getCurrentStage()) + " complete";
						uiPtr->displayText(dispText, false, 1.0f);
					}
				}
			}
			SVector2D nextCheckpoint = mCheckpoints[vehicle->getCurrentStage() % getStagesNumber()]->position2D();
			vehicle->setDistanceToCheckpoint(vehicle->distanceTo(nextCheckpoint));
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
	Collision::CollisionAxis reverseAxis = Collision::CollisionAxis::None;

	// Handle racecar user input
	racecarPtr->control(myEngine, kGameSpeed, kDeltaTime);
	// Update camera
	currentCamera = racecarPtr->getCamera();

	// Update AI movement
	for (HoverAI* hoverAI : mAI)
	{
		// Rubberbanding
		bool ahead = DesertVehicle::compare(hoverAI, racecarPtr);

		// If AI has reached its current target
		if (hoverAI->updateScene(kGameSpeed, kDeltaTime, racecarPtr->distanceTo(hoverAI->position2D()), ahead))
		{
 			// Calculate next waypoint
			unsigned int nextWaypointI = hoverAI->getWaypointIndex();
			if (mWaypoints.size() > nextWaypointI)
			{
				// Set AI's next target
				hoverAI->follow(mWaypoints[nextWaypointI]);
			}
			//// AI has reached the last waypoint
			else
			{
				hoverAI->resetWaypoint();
			}
		}
	}

	// Checkpoint crossing detection & handling
	detectCheckpointCrossings(kDeltaTime);

	// Sort vehicles by their race position
	sort(mVehicles.begin(), mVehicles.end(), &DesertVehicle::compare);

	// Update race position inside vehicle objects
	int i = 1;
	for (DesertVehicle* vehicle : mVehicles)
	{
		vehicle->setRacePosition(i);
		i++;
	}

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
		Collision::CollisionAxis axis = node->collision(racecarPtr->position2D(), racecarPtr->getCollisionRadius(), true);

		// Test for collision
		if (!carHasCollided && axis == Collision::CollisionAxis::Both)
		{
			reverseAxis = node->getNewCollisionAxis();
			carHasCollided = true;

			if (!node->isFixed())
			{
				node->modifyMovementVector(racecarPtr->getMovementVector());
			}
		}

		for (HoverAI* hoverAI : mAI)
		{
			if (node != hoverAI && node->collision(hoverAI->position2D(), hoverAI->getCollisionRadius()) == Collision::CollisionAxis::Both)
			{
				hoverAI->setCollided();
			}
		}
	}

	/**
	* Now, handle collision effects
	*/
	// Only count damages if non contiguous

	if (carHasCollided)
	{
		// Threshold
		if (racecarPtr->speedOverCollisionThreshold() && !carCollidedLastFrame) {
			// Reduce health, update UI
			racecarPtr->reduceHealth();
			updateHealthInUI();

			// If car has no more health, stop race
			if (!racecarPtr->getHealth())
			{
				raceState = Over;
				uiPtr->displayText("Your car is done for :(", false, 0, "Press R to Restart :)");
			}
		}

		
		// Cancel vector out
		racecarPtr->bounce(reverseAxis);
	}

	// After possibly cancelling movement vector out, apply result
	racecarPtr->applyMovementVector(kDeltaTime);

	// Handle AI collisions
	for (HoverAI* hoverAI : mAI)
	{
		if (hoverAI->hasCollided())
		{
			// Cancel AI vector
			hoverAI->bounce();
			hoverAI->reduceHealth();
		}

		// Apply movement vector result
		hoverAI->applyMovementVector(kDeltaTime);
		hoverAI->resetCollided();
	}

	updateUI();
	carCollidedLastFrame = carHasCollided;
}

void DesertRacetrack::reset()
{
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
	for (HoverAI* ai : mAI)
	{
		ai->reset();
		ai->follow(mWaypoints.front());
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
	uiPtr->toggleBoost(false);
	uiPtr->toggleWarning(false);
	uiPtr->toggleOverheat(false);

	// Show only the active one
	switch (racecarPtr->getBoostState())
	{
	case HoverCar::BoostState::Active:
		uiPtr->toggleBoost(true);
		break;
	case HoverCar::BoostState::Warning:
		uiPtr->toggleWarning(true);
		break;
	case HoverCar::BoostState::Penalty:
		uiPtr->toggleOverheat(true);
		break;
	default:
		break;
	}


	// Multiply by the seconds in an hour to get frames in an hour
	// Then by racecar speed
	float speedInKm = (racecarPtr->getSpeed() / kMetersInKm);
	//cout << speedInKm << endl;
	const int kmPerH = static_cast<int>(kSecsInAnHour * speedInKm);

	// Update speed
	uiPtr->setSpeedText(to_string(kmPerH) + "km/h");
	uiPtr->setRacePosition(racecarPtr->getRacePosition());
}

void DesertRacetrack::updateLapsInUI()
{
	uiPtr->setLapText(to_string(racecarPtr->getCurrentLap()) + "/" + to_string(kLaps));
}

void DesertRacetrack::updateHealthInUI()
{
	uiPtr->setHealthText(to_string(racecarPtr->getHealth()));
}

void DesertRacetrack::resetDialog()
{
	uiPtr->displayText("Hit Space to Start", false, 0);
	updateHealthInUI();
	uiPtr->setLapText("1/" + to_string(kLaps));
	uiPtr->togglePosition(false);
	uiPtr->toggleSummary(false);
}
