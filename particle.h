/**
 * @file particle.h
 * Particle systems
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#ifndef DESERT_RACER_PARTICLES_H
#define DESERT_RACER_PARTICLES_H

#include <TL-engine.h>
#include <string>
#include <vector>
#include "vector.h"
#include "node.h"


namespace desert
{
	class ParticleSystem
	{
	public:
		struct Particle
		{
			SceneNodeContainer container;
			SVector3D velocity;
			SVector3D randomisation;
			float lifespan;
			bool reset = false;
		};
		static float randMToN(double M, double N);
		ParticleSystem(tle::IMesh* mesh);
		void createParticles(int amount);
		void setup();
		void stop();
		void resume();
		void updateSystem(const float kDeltaTime, const SVector3D newPosition = { 0, 0, 0 });
	protected:
		int mNumParticles;
		int mInitialParticles;
		int mParticleSteps;
		float mLifespan = 1.0f;
		float mModelScale = 1.0f;

		vector<Particle> mParticles;
		SVector3D mGravity;
		SVector3D mInitialPosition;
		SVector3D mInitialVelocity;

		float mRangeX = 1, mRangeY = 1, mRangeZ = 1;
		const float minY = 9;
		tle::IMesh* mMesh;
		bool mActive = true;
	};

	class FireParticleSystem : public ParticleSystem
	{
	public:
		FireParticleSystem(tle::IMesh* fireMesh, SVector3D position);
	protected:
		const SVector3D kInitialPosition;
		const SVector3D kInitialVelocity = { 0.0f, 3.0f, 0.0f };
		const SVector3D kGravity = { 0.0f, -20.0f, 0.0f };
		const float kLifespan = 1.0f;
		const float kModelScale = 0.08f;
		const float kRangeX = 1, kRangeY = 1, kRangeZ = 1;
		const int kNumParticles = 120;
		const int kInitialParticles = 10;
		const int kParticleSteps = 10;
	};
};

#endif
