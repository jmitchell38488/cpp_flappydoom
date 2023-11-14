#pragma once

#include "olcPixelGameEngine.h"
#include "Settings.h"

enum class GroundState {
	IDLE, SCROLLING
};

class Ground {
private:
	float fScale = 1.0f;
	float fWidth = 1.0f;
	float fHeight = 1.0f;

	olc::vf2d fPosition;
	olc::Sprite* sGround;
	olc::Decal* dGround;

	float fAnimRate = ANIM_TICK;
	float fScrollRate = 0.0f;
	float fGroundY = GAME_HEIGHT - GROUND_HEIGHT;

	GroundState state = GroundState::IDLE;

public:
	Ground() {
		fAnimRate = GAME_SPEED;
	}

	void init();
	void update(float fElapsedTime);
	void render(olc::PixelGameEngine* engine, float fElapsedTime);
	void setScrolling();
	void setIdle();
	void reset();
};

void Ground::update(float fElapsedTime) {
	if (state == GroundState::IDLE) {
		return;
	}

	fPosition = { fPosition.x -= fScrollRate, fGroundY };

	// reposition to 0,0
	if (0 > fPosition.x + fWidth) fPosition.x = fPosition.x + fWidth;
}

void Ground::render(olc::PixelGameEngine* engine, float fElapsedTime) {
	// Draw frame 1
	engine->DrawDecal(fPosition, dGround, { fScale, fScale });

	// Draw frame 2?
	olc::vf2d newPos = { fPosition.x + fWidth, fPosition.y };
	if (fPosition.x + fWidth < engine->ScreenWidth()) {
		// Draw partial decal, not the full decal
		engine->DrawDecal(newPos, dGround, { fScale, fScale });

	}

	// Draw frame 3?
	if (fPosition.x + fWidth + fWidth < engine->ScreenWidth()) {
		olc::vf2d newPos2 = { fPosition.x + fWidth + fWidth, fPosition.y };
		// Draw partial decal, not the full decal
		engine->DrawDecal(newPos2, dGround, { fScale, fScale });

	}
}

void Ground::init() {
	fScale = 0.5f;

	sGround = new olc::Sprite((std::string)"./assets/images/ground.png");
	dGround = new olc::Decal(sGround);
	fHeight = sGround->height * fScale;
	fWidth = sGround->width * fScale;
	fScrollRate = SCROLL_SPEED;
	fPosition = { 0, fGroundY };
}

void Ground::setScrolling() {
	state = GroundState::SCROLLING;
}

void Ground::setIdle() {
	state = GroundState::IDLE;
}

void Ground::reset() {
	state = GroundState::IDLE;
	fPosition = { 0, fPosition.y };
}
