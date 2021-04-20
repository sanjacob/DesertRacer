/**
 * @file ui.h
 * Handles graphical user interface elements (sprites / text)
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#ifndef DESERT_RACER_UI_H
#define DESERT_RACER_UI_H

#include <TL-engine.h>
#include <string>
#include <vector>
#include "vector.h"


namespace desert
{
    /**
    * Creates a sprite that can be hidden, and hold several fonts associated with it
    */
    class DesertSprite
    {
    public:
        // Horizontal sprite alignment
        enum ESpriteHAlignment
        {
            Left,
            DoubleLeft,
            Right,
            DoubleRight,
            HCentre
        };

        // Vertical sprite alignment
        enum ESpriteVAlignment
        {
            Top,
            DoubleTop,
            Bottom,
            DoubleBottom,
            VCentre
        };

        // Sprite's visibility
        enum ESpriteState
        {
            Shown,
            Hidden
        };

        // TL-Engine font / text
        struct SText
        {
            tle::IFont* font;
            int textXOffset;
            int textYOffset;
            unsigned int colour;
            tle::EHorizAlignment hTextAlign;
            tle::EVertAlignment vTextAlign;
            std::string text;
        };

        /**
        * Create a DesertSprite object and realign if necessary
        * @param s Pointer to TL-Engine ISprite
        * @param windowW Window width
        * @param windowH Window height
        * @param w Sprite width
        * @param h Sprite height
        * @param wAlign Horizontal alignmment
        * @param hAlign Vertical alignment
        */
        DesertSprite(tle::ISprite* s, int windowW, int windowH, int w, int h, ESpriteHAlignment wAlign = HCentre, ESpriteVAlignment hAlign = VCentre);
        // Toggle visibility of sprite (it is moved outside the window)
        void toggle(bool show);
        /**
        * Add font to sprite
        * @param f TL-Engine IFont pointer
        * @param xOffset Offset from the sprite's centre in the X axis
        * @param yOffset Offset from the sprite's centre in the Y axis
        * @param col Colour for font
        * @param h TL-Engine horizontal alignment
        * @param v TL-Engine vertical alignment
        */
        void addFont(tle::IFont* f, const int xOffset, const int yOffset, const unsigned int col, tle::EHorizAlignment h, tle::EVertAlignment v);
        /**
        * Set the text for a sprite
        * @param t Text
        * @param i Font index
        */
        void setText(std::string t, int i);
        // Call draw method on every font on the sprite
        void drawText();

        
    protected:
        const int kWindowH, kWindowW;
        const int h, w;

        tle::ISprite* mSprite;
        std::vector<SText> mTexts;
        std::string mText = "";

        ESpriteState mState = Shown;

        int x, y;
        bool mHasFont = false;
    };

    /**
    * Creates most sprites / text in the UI
    */
    class GameUI
    {
    public:
        /**
        * @param myEngine Pointer to TL-Engine running instance
        */
        GameUI(tle::I3DEngine* myEngine);
        // UI deconstructor
        GameUI::~GameUI();
        /**
        * @param text Text to appear in the main dialog
        * @param goal If true the goal backdrop will be used
        * @param time For how long to show the text
        * @param subText Subtitle to appear in the main dialog
        */
        void displayText(std::string text, bool goal, const float time, std::string subText = "");
        /**
        * @param kDeltaTime Time elapsed since last frame
        */
        void drawGameUI(const float kDeltaTime);
        // Sets car health text
        void setHealthText(std::string text);
        // Sets car speed text
        void setSpeedText(std::string text);
        // Sets lap indicator text
        void setLapText(std::string text);
        // Toggles lap sprite visibility
        void toggleLapSprite(bool on);
        // Toggles boost indicator visibility
        void toggleBoostIndicator(bool on);
        // Toggles boost warning indicator visibility
        void toggleWarnIndicator(bool on);
        // Toggles overheated boost indicator visibility
        void toggleOverheatWarning(bool on);
        // Returns the ordinal part for a number (e.g. 1 -> 'st', 2 -> 'nd', and so on)
        static std::string racePlaceAsString(int place);

        // Struct containing information about a font
        struct SFontData
        {
            std::string fontFamily;
            int fontSize;
        };

        // Data about a sprite
        struct SSprite
        {
            std::string asset;
            int w, h;
        };
    protected:
        // Backdrop folder
        const std::string kAssetsFolder = "backdrops\\";

        // All assets data
        const SSprite tuxAsset = { "tux.png", 700, 60 };
        const SSprite goalAsset = { "goal.png", 700, 60 };
        const SSprite lapAsset = { "lap.png", 158, 55 };
        const SSprite speedAsset  = { "combined.png", 158, 55 };
        const SSprite placeAsset = { "place.png", 180, 180 };
        const SSprite boostAsset  = { "boost.png", 158, 55 };
        const SSprite warnAsset = { "warn.png", 158, 55 };
        const SSprite overheatAsset = { "overheat.png", 158, 55 };

        // Font data
        const SFontData mainFont = { "Comic Sans MS", 32 };
        const SFontData lapFont = { "Consolas", 40 };
        const SFontData statsFont = { "Consolas", 20 };
        const SFontData placeFont = { "Consolas", 120 };
        const SFontData placeSubFont = { "Consolas", 60 };

        const int kPadding = 20;
        const int kWindowH, kWindowW;

        DesertSprite *mTuxSprite, *mGoalSprite, *mLapSprite, *mSpeedSprite, *mPlaceSprite;
        DesertSprite *mBoostSprite, *mWarnSprite, *mOverheatSprite;

        DesertSprite::ESpriteState mState;

        float mShowForTime;
        float mTimeElapsed = 0.0f;
    };
}

#endif