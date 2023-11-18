#pragma once
#pragma once

#include "../lib/olcPixelGameEngine.h"
#include "../Settings.h"
#include "../definitions.h"

enum class CeilingState {
	IDLE, SCROLLING
};

class Ceiling {
private:
	float fScaleFactor = 1.0f;
	float fWidth = 1.0f;
	float fHeight = 1.0f;

	olc::vf2d fPosition;
	olc::Sprite* sCeiling;
	olc::Decal* dCeiling;

	float fAnimRate = ANIM_TICK;
	float fCeilingY = 0;

	CeilingState state = CeilingState::IDLE;

public:
	Ceiling() {
		fAnimRate = GAME_SPEED;
	}

	void init();
	void update(float fElapsedTime, float gameSpeed);
	void render(olc::PixelGameEngine* engine, float fElapsedTime);
	void setScrolling();
	void setIdle();
	void reset();
};

void Ceiling::update(float fElapsedTime, float gameSpeed) {
	if (state == CeilingState::IDLE) {
		return;
	}

	fPosition = { fPosition.x -= gameSpeed, fCeilingY };

	// reposition to 0,0
	if (0 > fPosition.x + fWidth) fPosition.x = fPosition.x + fWidth;
}

void Ceiling::render(olc::PixelGameEngine* engine, float fElapsedTime) {
  if (!gSettings.DEBUG_MODE) {
    // Draw frame 1
    engine->DrawDecal(fPosition, dCeiling, { fScaleFactor, fScaleFactor });

    // Draw frame 2?
    olc::vf2d newPos = { fPosition.x + fWidth, fPosition.y };
    if (fPosition.x + fWidth < engine->ScreenWidth()) {
      // Draw partial decal, not the full decal
      engine->DrawDecal(newPos, dCeiling, { fScaleFactor, fScaleFactor });

    }

    // Draw frame 3?
    if (fPosition.x + fWidth + fWidth < engine->ScreenWidth()) {
      olc::vf2d newPos2 = { fPosition.x + fWidth + fWidth, fPosition.y };
      // Draw partial decal, not the full decal
      engine->DrawDecal(newPos2, dCeiling, { fScaleFactor, fScaleFactor });

    }
  }

  if (gSettings.DEBUG_MODE) {
    engine->DrawRect({0, 0}, {GAME_WIDTH, (int)(dCeiling->sprite->height * fScaleFactor)}, {0, 255, 0});
  }
}

void Ceiling::init() {
	sCeiling = new olc::Sprite((std::string)"./assets/i-cl.jpg");
	dCeiling = new olc::Decal(sCeiling);
	fHeight = sCeiling->height * fScaleFactor;
	fWidth = sCeiling->width * fScaleFactor;
	fPosition = { 0, fCeilingY };
}

void Ceiling::setScrolling() {
	state = CeilingState::SCROLLING;
}

void Ceiling::setIdle() {
	state = CeilingState::IDLE;
}

void Ceiling::reset() {
	state = CeilingState::IDLE;
	fPosition = { 0, fPosition.y };
}
