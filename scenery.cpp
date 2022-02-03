#include <TL-Engine.h>
#include <iostream>

#include "vector.h"
#include "node.h"
#include "scenery.h"
#include "collision.h"

using namespace tle;
using namespace std;
using namespace desert;


DesertWall::DesertWall(IModel* m, NodeAlignment alignment) : BoxCollisionModel(m, alignment)
{
	mHalfWidth = kHalfWidth;
	mHalfLength = kHalfLength;
	cout << "DesertWall created" << endl;
}

DesertWall::~DesertWall()
{
	cout << "DesertWall destroyed at ";
	cout << position2D().asString() << endl;
}

const string DesertWall::kDefaultModelName = "Wall";

DesertTower::DesertTower(IModel* m, NodeAlignment alignment) : BoxCollisionModel(m, alignment)
{
	mHalfWidth = kHalfWidth;
	mHalfLength = kHalfLength;
	cout << "DesertTower created" << endl;
}

DesertTower::~DesertTower()
{
	cout << "DesertTower destroyed at ";
	cout << position2D().asString() << endl;
}

const string DesertTower::kDefaultModelName = "Tower";

CustomSphereCModel::CustomSphereCModel(IModel* m, const float radius) : SphereCollisionModel(m)
{
	mRadius = radius;
	cout << "DesertTank created" << endl;
}

CustomSphereCModel::~CustomSphereCModel()
{
	cout << "Snowman destroyed" << endl;
}
