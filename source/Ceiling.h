#pragma once
#pragma once

#include "olcPixelGameEngine.h"
#include "Settings.h"

enum class CeilingState {
	IDLE, SCROLLING
};

class Ceiling {
private:
	float fScale = 1.0f;
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
	// Draw frame 1
	engine->DrawDecal(fPosition, dCeiling, { fScale, fScale });

	// Draw frame 2?
	olc::vf2d newPos = { fPosition.x + fWidth, fPosition.y };
	if (fPosition.x + fWidth < engine->ScreenWidth()) {
		// Draw partial decal, not the full decal
		engine->DrawDecal(newPos, dCeiling, { fScale, fScale });

	}

	// Draw frame 3?
	if (fPosition.x + fWidth + fWidth < engine->ScreenWidth()) {
		olc::vf2d newPos2 = { fPosition.x + fWidth + fWidth, fPosition.y };
		// Draw partial decal, not the full decal
		engine->DrawDecal(newPos2, dCeiling, { fScale, fScale });

	}
}

void Ceiling::init() {
	fScale = 0.5f;

	sCeiling = new olc::Sprite((std::string)"./assets/images/ceiling.png");
	dCeiling = new olc::Decal(sCeiling);
	fHeight = sCeiling->height * fScale;
	fWidth = sCeiling->width * fScale;
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
