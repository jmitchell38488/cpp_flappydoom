#pragma once

#include "../lib/olcPixelGameEngine.h"
#include "../Settings.h"
#include "../definitions.h"

enum class BackgroundState {
	IDLE, SCROLLING
};

class Background {
private:
	float fScaleFactor = 1.0f;
	float fWidth = 1.0f;
	float fHeight = 1.0f;

	olc::vf2d fPosition;
	olc::Sprite* sBackground;
	olc::Decal* dBackground;

	float fAnimRate = ANIM_TICK;
	float fLastAnimTime = 0.0f;
	float fScrollRate = 0.0f;
	float fY = (float)CEILING_HEIGHT;

	BackgroundState state = BackgroundState::IDLE;

public:
	Background() {
		fAnimRate = BG_SPEED;
	}

	void init();
	void update(float fElapsedTime);
	void render(olc::PixelGameEngine* engine, float fElapsedTime);
	void setScrolling();
	void setIdle();
	void reset();
};

void Background::update(float fElapsedTime) {
	if (state == BackgroundState::IDLE) {
		return;
	}

	fPosition = { (fPosition.x -= fScrollRate) * 1 + fElapsedTime, fY };

	// reposition to 0,fY
	if (0 > fPosition.x + fWidth) fPosition.x = fPosition.x + fWidth;
}

void Background::render(olc::PixelGameEngine* engine, float fElapsedTime) {
  if (!gSettings.DEBUG_MODE) {
    // Draw frame 1
    engine->DrawDecal(fPosition, dBackground, { fScaleFactor, fScaleFactor });
    
    // Draw frame 2?
    olc::vf2d newPos = { fPosition.x + fWidth, fPosition.y };
    if (fPosition.x + fWidth < engine->ScreenWidth()) {
      // Draw partial decal, not the full decal
      engine->DrawDecal(newPos, dBackground, { fScaleFactor, fScaleFactor });
    }

    // Draw frame 3?
    if (fPosition.x + fWidth + fWidth < engine->ScreenWidth()) {
      olc::vf2d newPos2 = { fPosition.x + fWidth + fWidth, fPosition.y };
      // Draw partial decal, not the full decal
      engine->DrawDecal(newPos2, dBackground, { fScaleFactor, fScaleFactor });

    }
  }
}

void Background::init() {
	sBackground = new olc::Sprite((std::string)"./assets/i-bg.jpg");
	dBackground = new olc::Decal(sBackground);
	fHeight = sBackground->height * fScaleFactor;
	fWidth = sBackground->width * fScaleFactor;
	fScrollRate = (GAME_TICK) * fWidth / 10.0f;
	fPosition = { 0, fY };
}

void Background::setScrolling() {
	state = BackgroundState::SCROLLING;
}

void Background::setIdle() {
	state = BackgroundState::IDLE;
}

void Background::reset() {
	fPosition = { 0, fY };
	fLastAnimTime = 0.0f;
	state = BackgroundState::IDLE;
}