/**
 * @file particle.cpp
 * Particle systems
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */
#include <TL-Engine.h>
#include <random>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "vector.h"
#include "particle.h"

using namespace std;
using namespace tle;
using namespace desert;

ParticleSystem::ParticleSystem(IMesh* mesh) : mMesh(mesh) {};

float ParticleSystem::randMToN(double M, double N)
{
	// https://stackoverflow.com/a/17798317
	return M + (rand() / (RAND_MAX / (N - M)));
}

void ParticleSystem::setup()
{
	// Feed current timestamp as random seed
	srand(time(nullptr));
	createParticles(mInitialParticles);
}

void ParticleSystem::createParticles(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		SVector3D velocity = mInitialVelocity;
		SVector3D randomVector = { randMToN(-mRangeX, mRangeX), randMToN(-mRangeY, mRangeY), randMToN(-mRangeZ, mRangeZ) };

		IModel* model = mMesh->CreateModel(mInitialPosition.x, mInitialPosition.y, mInitialPosition.z);
		model->Scale(mModelScale);

		mParticles.push_back({ SceneNodeContainer(model), velocity + randomVector, randomVector, 0.0f });
	}
}

void ParticleSystem::resume()
{
	mActive = true;
}

void ParticleSystem::stop()
{
	mActive = false;
}

void ParticleSystem::updateSystem(const float kDeltaTime, const SVector3D newPosition)
{
	for (Particle& p : mParticles)
	{

		if (!mActive && p.reset)
		{
			if (newPosition.isZero())
			{
				p.container.resetPosition();
			}
			else
			{
				p.container.setPositionByVector(newPosition);
			}
		}
		else
		{
			p.container.moveByVector(p.velocity * kDeltaTime);
			p.lifespan += kDeltaTime;
			p.velocity += (mGravity * kDeltaTime);
			if (p.reset)
			{
				p.reset = false;
			}

			if (p.lifespan > mLifespan)
			{
				p.lifespan = 0.0f;
				p.reset = true;
				p.velocity = mInitialVelocity + p.randomisation;
				if (newPosition.isZero())
				{
					p.container.resetPosition();
				}
				else
				{
					p.container.setPositionByVector(newPosition);
				}
			}
		}
		
		/*if (p.container.position2D().y < minY)
		{
			p.velocity = mInitialVelocity + p.randomisation;
		}*/
	}
	
	if (mParticles.size() < mNumParticles)
	{
		createParticles(mParticleSteps);
	}
}

FireParticleSystem::FireParticleSystem(IMesh* fireMesh, SVector3D position) : ParticleSystem(fireMesh), kInitialPosition(position)
{
	mNumParticles = kNumParticles;
	mInitialParticles = kInitialParticles;
	mParticleSteps = kParticleSteps;
	mLifespan = kLifespan;
	mModelScale = kModelScale;
	mRangeX = kRangeX;
	mRangeY = kRangeY;
	mRangeZ = kRangeZ;
	mInitialPosition = kInitialPosition;
	mInitialVelocity = kInitialVelocity;
}