/**
 * @file ui.cpp
 * Handles graphical user interface elements (sprites / text)
 *
 * @author Jacob Sanchez Perez (G20812080) <jsanchez-perez@uclan.ac.uk>
 * Games Concepts (CO1301), University of Central Lancashire
 */

#include <TL-Engine.h>
#include <iostream>
#include "ui.h"
#include "collision.h"

using namespace tle;
using namespace desert;


DesertSprite::DesertSprite(ISprite* s, int windowW, int windowH, int w, int h, ESpriteHAlignment wAlign, ESpriteVAlignment hAlign) : mSprite(s), kWindowW(windowW), kWindowH(windowH), w(w), h(h)
{
	// Realign sprite horizontally 
	switch (wAlign)
	{
	case DoubleLeft:
		s->MoveX(-(w / 2));
	case Left:
		s->MoveX(-(w / 2));
		break;
	case DoubleRight:
		s->MoveX(w / 2);
	case Right:
		s->MoveX(w / 2);
		break;
	}

	// Realign sprite vertically
	switch (hAlign)
	{
	case DoubleTop:
		s->MoveY(h / 2);
	case Top:
		s->MoveY(h / 2);
		break;
	case DoubleBottom:
		s->MoveY(-(h / 2));
	case Bottom:
		s->MoveY(-(h / 2));
		break;
	}

	x = s->GetX() + (w / 2);
	y = s->GetY() + (h / 2);
}



void DesertSprite::toggle(bool show)
{
	if (show)
	{
		mSprite->SetY(y - (h / 2));
		mState = Shown;
		return;
	}

	mSprite->SetY(kWindowH);
	mState = Hidden;
}

void DesertSprite::addFont(IFont* f, const int xOffset, const int yOffset, const unsigned int col, const EHorizAlignment h, const EVertAlignment v)
{
	mHasFont = true;
	mTexts.push_back({ f, xOffset, yOffset, col, h, v });
}

void DesertSprite::setText(string t, int i)
{
	mTexts[i].text = t;
}

void DesertSprite::drawText()
{
	if (mHasFont && mState == Shown)
	{
		for (SText& t : mTexts)
		{
			t.font->Draw(t.text, x + t.textXOffset, y + t.textYOffset, t.colour, t.hTextAlign, t.vTextAlign);
		}
	}
}

SVector2D DesertSprite::position2D()
{
	return { static_cast<float>(x), static_cast<float>(y) };
}

bool DesertSprite::collision(SVector2D point)
{
	return (Collision::pointToBox(point, position2D(), (w / 2), (h / 2)) == Collision::Both);
}

ISprite* DesertSprite::getISPrite()
{
	return mSprite;
}

GameUI::GameUI(I3DEngine* myEngine) : kWindowH(myEngine->GetHeight()), kWindowW(myEngine->GetWidth())
{
	const int smallUnit = 10,  doubleUnit = 20, reallyBigUnit = 60;
	const unsigned int placeColour = 4279060385;
	IFont* defaultIFont = myEngine->LoadFont(mainFont.fontFamily, mainFont.fontSize);
	IFont* lapIFont = myEngine->LoadFont(lapFont.fontFamily, lapFont.fontSize);
	IFont* littleFont = myEngine->LoadFont(statsFont.fontFamily, statsFont.fontSize);
	IFont* bigFont = myEngine->LoadFont(placeSubFont.fontFamily, placeSubFont.fontSize);
	IFont* biggerFont = myEngine->LoadFont(placeFont.fontFamily, placeFont.fontSize);

	ISprite* tuxISprite = myEngine->CreateSprite(kAssetsFolder + tuxAsset.asset, kWindowW / 2, kWindowH - kPadding);
	mTuxSprite = new DesertSprite(tuxISprite, kWindowW, kWindowW, tuxAsset.w, tuxAsset.h, DesertSprite::Left, DesertSprite::DoubleBottom);
	mTuxSprite->addFont(defaultIFont, 0, -smallUnit, kBlack, kCentre, kVCentre);
	mTuxSprite->addFont(littleFont, 0, smallUnit, kBlack, kCentre, kVCentre);

	ISprite* goalISprite = myEngine->CreateSprite(kAssetsFolder + goalAsset.asset, kWindowW / 2, kWindowH - kPadding);
	mGoalSprite = new DesertSprite(goalISprite, kWindowW, kWindowW, goalAsset.w, goalAsset.h, DesertSprite::Left, DesertSprite::DoubleBottom);
	mGoalSprite->addFont(defaultIFont, 0, -smallUnit, kBlack, kCentre, kVCentre);
	mGoalSprite->addFont(littleFont, 0, smallUnit, kBlack, kCentre, kVCentre);

	ISprite* lapISprite = myEngine->CreateSprite(kAssetsFolder + lapAsset.asset, kPadding, kWindowH - kPadding);
	mLapSprite = new DesertSprite(lapISprite, kWindowW, kWindowW, lapAsset.w, lapAsset.h, DesertSprite::HCentre, DesertSprite::DoubleBottom);
	mLapSprite->addFont(lapIFont, doubleUnit, 0, kBlack, kCentre, kVCentre);

	ISprite* boostISprite = myEngine->CreateSprite(kAssetsFolder + boostAsset.asset, kWindowW - kPadding, kWindowH - kPadding);
	mBoostSprite = new DesertSprite(boostISprite, kWindowW, kWindowH, boostAsset.w, boostAsset.h, DesertSprite::DoubleLeft, DesertSprite::DoubleBottom);

	ISprite* warnISprite = myEngine->CreateSprite(kAssetsFolder + warnAsset.asset, kWindowW - kPadding, kWindowH - kPadding);
	mWarnSprite = new DesertSprite(warnISprite, kWindowW, kWindowH, boostAsset.w, boostAsset.h, DesertSprite::DoubleLeft, DesertSprite::DoubleBottom);

	ISprite* overheatISprite = myEngine->CreateSprite(kAssetsFolder + overheatAsset.asset, kWindowW - kPadding, kWindowH - kPadding);
	mOverheatSprite = new DesertSprite(overheatISprite, kWindowW, kWindowH, overheatAsset.w, overheatAsset.h, DesertSprite::DoubleLeft, DesertSprite::DoubleBottom);

	ISprite* speedISprite = myEngine->CreateSprite(kAssetsFolder + speedAsset.asset, kWindowW - kPadding, kWindowH - kPadding);
	mSpeedSprite = new DesertSprite(speedISprite, kWindowW, kWindowH, speedAsset.w, speedAsset.h, DesertSprite::DoubleLeft, DesertSprite::DoubleBottom);
	mSpeedSprite->addFont(littleFont, 0, -smallUnit, kBlack, kRight, kVCentre);
	mSpeedSprite->addFont(littleFont, 0, smallUnit, kBlack, kCentre, kVCentre);
	
	ISprite* placeISprite = myEngine->CreateSprite(kAssetsFolder + placeAsset.asset, kPadding, kPadding);
	mPlaceSprite = new DesertSprite(placeISprite, kWindowW, kWindowH, placeAsset.w, placeAsset.h, DesertSprite::HCentre, DesertSprite::VCentre);
	mPlaceSprite->addFont(biggerFont, -doubleUnit - smallUnit, -reallyBigUnit, placeColour, kCentre, kTop);
	mPlaceSprite->addFont(bigFont, doubleUnit + smallUnit, -reallyBigUnit + smallUnit, placeColour, kCentre, kTop);

	ISprite* summaryISprite = myEngine->CreateSprite(kAssetsFolder + summaryAsset.asset, kWindowW / 2, kWindowH / 2);
	mSummarySprite = new DesertSprite(summaryISprite, kWindowW, kWindowH, summaryAsset.w, summaryAsset.h, DesertSprite::Left, DesertSprite::Bottom);
	mSummarySprite->addFont(defaultIFont, 0, -20, kWhite, kCentre, kVCentre);
	mSummarySprite->addFont(littleFont, 0, +40, kWhite, kCentre, kVCentre);

	mSpeedSprite->setText("88 mi/h", 1);
	mPlaceSprite->toggle(false);
	mGoalSprite->toggle(false);
	mSummarySprite->toggle(false);
}

GameUI::~GameUI()
{
	// Free memory up
	delete mTuxSprite;
	delete mGoalSprite;
	delete mLapSprite;
	delete mSpeedSprite;
	delete mPlaceSprite;
	delete mBoostSprite;
	delete mWarnSprite;
	delete mOverheatSprite;
	delete mSummarySprite;

	mTuxSprite = mGoalSprite = mLapSprite = mSpeedSprite = mPlaceSprite = mBoostSprite = mWarnSprite = mOverheatSprite = mSummarySprite = nullptr;
}

void GameUI::setRacePosition(int position)
{
	mPlaceSprite->setText(to_string(position), 0);
	mPlaceSprite->setText(racePlaceAsString(position), 1);
}

void GameUI::togglePosition(bool on)
{
	mPlaceSprite->toggle(on);
}

string GameUI::racePlaceAsString(int place)
{
	switch (place)
	{
	case 1:
		return "st";
	case 2:
		return "nd";
	case 3:
		return "rd";
	default:
		return "th";
	}
}

void GameUI::displayText(string text, bool goal, const float time, string subText)
{
	mShowForTime = time;

	mTimeElapsed = 0.0f;
	mState = DesertSprite::ESpriteState::Shown;

	if (goal)
	{
		mTuxSprite->toggle(false);
		mGoalSprite->toggle(true);
		mGoalSprite->setText(text, 0);
		mGoalSprite->setText(subText, 1);
	}
	else
	{
		mTuxSprite->toggle(true);
		mGoalSprite->toggle(false);
		mTuxSprite->setText(text, 0);
		mTuxSprite->setText(subText, 1);
	}
}

void GameUI::setLapText(string text)
{
	mLapSprite->setText(text, 0);
}

void GameUI::setHealthText(string text)
{
	mSpeedSprite->setText(text, 0);
}

void GameUI::setSpeedText(string text)
{
	mSpeedSprite->setText(text, 1);
}

void GameUI::setSummaryWinner(string text)
{
	mSummarySprite->setText(text, 0);
}

void GameUI::setSummaryTime(string text)
{
	mSummarySprite->setText(text, 1);
}

void GameUI::drawGameUI(const float kDeltaTime)
{
	if (mState == DesertSprite::ESpriteState::Shown) {
		mTimeElapsed += kDeltaTime;

		if (mShowForTime && mTimeElapsed > mShowForTime)
		{
			mTimeElapsed = 0.0f;
			mTuxSprite->setText("", 0);
			mTuxSprite->setText("", 1);
			mState = DesertSprite::ESpriteState::Hidden;
		}
		else
		{
			mTuxSprite->drawText();
			mGoalSprite->drawText();
		}
	}

	mLapSprite->drawText();
	mSpeedSprite->drawText();
	mPlaceSprite->drawText();
	mSummarySprite->drawText();
}

void GameUI::toggleLapSprite(bool on)
{
	mLapSprite->toggle(on);
}

void GameUI::toggleBoost(bool on)
{
	mBoostSprite->toggle(on);
}

void GameUI::toggleWarning(bool on)
{
	mWarnSprite->toggle(on);
}

void GameUI::toggleOverheat(bool on)
{
	mOverheatSprite->toggle(on);
}

void GameUI::toggleSummary(bool on)
{
	mSummarySprite->toggle(on);
}