/**
 * @file node.cpp
 * Container for an ISceneNode* with common operations involving vectors and position resetting
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#include <TL-Engine.h>
#include "vector.h"
#include "collision.h"
#include "node.h"

using namespace tle;
using namespace desert;


SceneNodeContainer::SceneNodeContainer(ISceneNode* sceneNode) :
	node(sceneNode), kInitialPosition({ sceneNode->GetX(), sceneNode->GetY(), sceneNode->GetZ() }),
	kInitialLocalPosition({ sceneNode->GetLocalX(), sceneNode->GetLocalY(), sceneNode->GetLocalZ() })
{
}

SVector3D SceneNodeContainer::position() const
{
	return { node->GetX(), node->GetY(), node->GetZ() };
}

SVector2D SceneNodeContainer::position2D() const
{
	return { node->GetX(), node->GetZ() };
}

SVector3D SceneNodeContainer::getFacingVector() const
{
	float matrix[4][4];
	node->GetMatrix(&matrix[0][0]);
	return  { matrix[2][0], matrix[2][1], matrix[2][2] };
}

SVector2D SceneNodeContainer::getFacingVector2D() const
{
	float matrix[4][4];
	node->GetMatrix(&matrix[0][0]);
	return  { matrix[2][0], matrix[2][2] };
}

void SceneNodeContainer::resetPosition(bool x, bool y, bool z)
{
	if (x) { node->SetX(kInitialPosition.x); }
	if (y) { node->SetY(kInitialPosition.y); }
	if (z) { node->SetZ(kInitialPosition.z); }
}

void SceneNodeContainer::resetLocalPosition(bool x, bool y, bool z)
{
	if (x) { node->SetLocalX(kInitialLocalPosition.x); }
	if (y) { node->SetLocalY(kInitialLocalPosition.y); }
	if (z) { node->SetLocalZ(kInitialLocalPosition.z); }
}

void SceneNodeContainer::moveByVector(SVector3D vector)
{
	node->Move(vector.x, vector.y, vector.z);
}

void SceneNodeContainer::moveByVector(SVector2D vector)
{
	node->Move(vector.x, 0, vector.y);
}

void SceneNodeContainer::moveLocallyByVector(SVector3D vector)
{
	node->MoveLocal(vector.x, vector.y, vector.z);
}

void SceneNodeContainer::moveLocallyByVector(SVector2D vector)
{
	node->MoveLocal(vector.x, 0, vector.y);
}

void SceneNodeContainer::rotateByVector(SVector3D vector)
{
	node->RotateX(vector.x);
	node->RotateY(vector.y);
	node->RotateZ(vector.z);
}

void SceneNodeContainer::rotateByVector(SVector2D vector)
{
	node->RotateX(vector.x);
	node->RotateZ(vector.y);
}

void SceneNodeContainer::rotateLocallyByVector(SVector3D vector)
{
	node->RotateLocalX(vector.x);
	node->RotateLocalY(vector.y);
	node->RotateLocalZ(vector.z);
}

void SceneNodeContainer::rotateLocallyByVector(SVector2D vector)
{
	node->RotateX(vector.x);
	node->RotateZ(vector.y);
}

const int SceneNodeContainer::kAxisDegrees = 180;

NodeAlignment SceneNodeContainer::getAlignmentFromRotation(int rotation)
{
	switch (rotation % kAxisDegrees) 
	{
	case 0:
		return xAligned;
	case (kAxisDegrees / 2):
		return zAligned;
	default:
		return Other;
	};
}

CollisionModel::CollisionModel(IModel* m) : SceneNodeContainer(m) {}
CollisionModel::~CollisionModel()
{
	cout << "Collision Model destroyed" << endl;
}

SphereCollisionModel::SphereCollisionModel(IModel* m) : CollisionModel(m) {}

bool SphereCollisionModel::collision(SVector2D position, const float collisionRadius)
{
	return Collision::circleToCircle(position, position2D(), collisionRadius, mRadius);
}

bool SphereCollisionModel::collision(SphereCollisionModel other)
{
	return Collision::circleToCircle(other.position2D(), position2D(), other.getCollisionRadius(), mRadius);
}

int SphereCollisionModel::getCollisionRadius()
{
	return mRadius;
}

BoxCollisionModel::BoxCollisionModel(IModel* m, NodeAlignment a) : CollisionModel(m), mAlignment(a) {}

bool BoxCollisionModel::collision(SVector2D position, const float collisionRadius)
{
	if (mAlignment == zAligned)
		return Collision::circleToBox(position, collisionRadius, position2D(), mHalfLength, mHalfWidth);
	return Collision::circleToBox(position, collisionRadius, position2D(), mHalfWidth, mHalfLength);
}

bool BoxCollisionModel::collision(SphereCollisionModel other)
{
	if (mAlignment == zAligned)
		return Collision::circleToBox(other.position2D(), other.getCollisionRadius(), position2D(), mHalfLength, mHalfWidth);
	return Collision::circleToBox(other.position2D(), other.getCollisionRadius(), position2D(), mHalfWidth, mHalfLength);
}