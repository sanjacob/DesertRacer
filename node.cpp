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

float SceneNodeContainer::distanceTo(SVector2D point) const
{
	return (position2D() - point).length();
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

void SceneNodeContainer::setPositionByVector(SVector3D vector)
{
	node->SetPosition(vector.x, vector.y, vector.z);
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

bool CollisionModel::isFixed()
{
	return mFixed;
}

bool CollisionModel::vectorWasModified()
{
	return mVectorModified;
}

void CollisionModel::modifyMovementVector(SVector2D change) {}

Collision::CollisionAxis CollisionModel::getNewCollisionAxis()
{
	return mNewCollisionAxis;
}

SphereCollisionModel::SphereCollisionModel(IModel* m) : CollisionModel(m) {}

Collision::CollisionAxis SphereCollisionModel::collision(SVector2D position, const float collisionRadius, bool saveAxis)
{
	mVectorModified = false;
	return Collision::circleToCircle(position, position2D(), collisionRadius, mRadius);
}

Collision::CollisionAxis SphereCollisionModel::collision(SphereCollisionModel other)
{
	mVectorModified = false;
	return Collision::circleToCircle(other.position2D(), position2D(), other.getCollisionRadius(), mRadius);
}

int SphereCollisionModel::getCollisionRadius()
{
	return mRadius;
}

BoxCollisionModel::BoxCollisionModel(IModel* m, NodeAlignment a) : CollisionModel(m), mAlignment(a) {}

Collision::CollisionAxis BoxCollisionModel::collision(SVector2D position, const float collisionRadius, bool saveAxis)
{
	mVectorModified = false;
	Collision::CollisionAxis axis;

	if (mAlignment == zAligned)
	{
		axis = Collision::circleToBox(position, collisionRadius, position2D(), mHalfLength, mHalfWidth);
	}
	else
	{
		axis = Collision::circleToBox(position, collisionRadius, position2D(), mHalfWidth, mHalfLength);
	}

	if (saveAxis)
	{
		setNewCollisionAxis(axis);
		mLastCollisionAxis = axis;
	}
	return axis;
}

Collision::CollisionAxis BoxCollisionModel::collision(SphereCollisionModel other)
{
	mVectorModified = false;
	Collision::CollisionAxis axis;
	
	if (mAlignment == zAligned)
	{
		axis = Collision::circleToBox(other.position2D(), other.getCollisionRadius(), position2D(), mHalfLength, mHalfWidth);
	}
	else
	{
		axis = Collision::circleToBox(other.position2D(), other.getCollisionRadius(), position2D(), mHalfWidth, mHalfLength);
	}

	setNewCollisionAxis(axis);
	mLastCollisionAxis = axis;
	return axis;
}

void CollisionModel::setNewCollisionAxis(Collision::CollisionAxis axis)
{
	mNewCollisionAxis = Collision::None;

	if (axis == Collision::Both)
	{
		switch (mLastCollisionAxis)
		{
		case desert::Collision::xAxis:
			mNewCollisionAxis = Collision::yAxis;
			break;
		case desert::Collision::yAxis:
			mNewCollisionAxis = Collision::xAxis;
			break;
		case desert::Collision::None:
			mNewCollisionAxis = Collision::Both;
			break;
		}
	}
}