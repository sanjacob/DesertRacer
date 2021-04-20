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

DesertTank::DesertTank(IModel* m) : SphereCollisionModel(m)
{
	mRadius = kCollisionRadius;
	cout << "DesertTank created" << endl;
}

DesertTank::~DesertTank()
{
	cout << "DesertTank destroyed" << endl;
}