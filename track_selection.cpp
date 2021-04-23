/**
 * @file track_selection.cpp
 * Creates track selection menu
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#include <TL-Engine.h>
#include <iostream>
#include "vector.h"
#include "collision.h"
#include "ui.h"
#include "keybinds.h"
#include "track_selection.h"

using namespace tle;
using namespace std;
using namespace desert;

TrackSelection::TrackSelection(I3DEngine* myEngine, EKeyCode trackSelectKey) : kTrackSelectionKey(trackSelectKey), kWindowW(myEngine->GetWidth()), kWindowH(myEngine->GetHeight())
{
	const int rowH = (kWindowH / mRows), colW = (kWindowW / mColumns);
	int i = 0, h = 0, w = 0;
	for (int r = 0; r < mRows; r++)
	{
		w = 0;
		for (int c = 0; c < mColumns; c++)
		{
			if (mTrackAssets.size() > i)
			{
				ISprite* tlSprite = myEngine->CreateSprite(kAssetsFolder + mTrackAssets[i].asset, w + (colW / 2), h + (rowH / 2));
				mTrackSprites.push_back(new DesertSprite(tlSprite, kWindowW, kWindowH, mTrackAssets[i].w, mTrackAssets[i].h,
					DesertSprite::ESpriteHAlignment::Left, DesertSprite::ESpriteVAlignment::Bottom));
			}

			i++;
			w += colW;
		}
		h += rowH;
	}
	
	ISprite* bgSprite = myEngine->CreateSprite(kAssetsFolder + bgAsset.asset, kWindowW / 2, kWindowH / 2, -1);
	mBackground = new DesertSprite(bgSprite, kWindowW, kWindowH, bgAsset.w, bgAsset.h, DesertSprite::ESpriteHAlignment::Left, DesertSprite::ESpriteVAlignment::Bottom);
}

void TrackSelection::remove(I3DEngine* myEngine)
{
	for (DesertSprite* sprite : mTrackSprites)
	{
		myEngine->RemoveSprite(sprite->getISPrite());
	}

	myEngine->RemoveSprite(mBackground->getISPrite());
}

TrackSelection::~TrackSelection()
{
	delete mBackground;
	mBackground = nullptr;

	for (DesertSprite* sprite : mTrackSprites)
	{
		delete sprite;
		sprite = nullptr;
	}
}

int TrackSelection::updateScene(I3DEngine* myEngine)
{
	if (myEngine->KeyHit(Mouse_LButton))
	{
		SVector2D mouseVector = { static_cast<float>(myEngine->GetMouseX()), static_cast<float>(myEngine->GetMouseY()) };
		int i = 0;
		for (DesertSprite* sprite : mTrackSprites)
		{
			if (sprite->collision(mouseVector))
			{
				return i;
			}
			i++;
		}
	}

	return -1;
}

