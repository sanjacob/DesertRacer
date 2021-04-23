/**
 * @file track_selection.h
 * Creates track selection menu
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#ifndef DESERT_RACER_TRACK_SELECTION_H
#define DESERT_RACER_TRACK_SELECTION_H

#include <TL-Engine.h>
#include "vector.h"
#include "collision.h"


namespace desert
{
	class TrackSelection
	{
	public:
		TrackSelection(tle::I3DEngine* myEngine, tle::EKeyCode trackSelectKey);
		~TrackSelection();
		void remove(tle::I3DEngine* myEngine);
		int updateScene(tle::I3DEngine* myEngine);
		static const int kNoTrackSelected = -1;
	protected:
		const std::string kAssetsFolder = "backdrops\\";
		const int mRows = 2, mColumns = 2;
		const int kWindowW, kWindowH;
		const DesertSprite::SSprite bgAsset = { "selection_bg.png", 1920, 1080 };
		vector<DesertSprite::SSprite> mTrackAssets {
			{ "track_one.png", 428, 223 },
			{ "track_two.png", 428, 223 },
			{ "track_three.png", 428, 223 }
		};

		vector<DesertSprite*> mTrackSprites;
		const tle::EKeyCode kTrackSelectionKey;
		DesertSprite *mBackground;
	};
};

#endif