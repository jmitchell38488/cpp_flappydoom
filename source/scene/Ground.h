#pragma once

#include "../lib/olcPixelGameEngine.h"
#include "../Settings.h"
#include "../definitions.h"

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
	float fGroundY = GAME_HEIGHT - GROUND_HEIGHT;

	GroundState state = GroundState::IDLE;

public:
	Ground() {
		fAnimRate = GAME_SPEED;
	}

	void init();
	void update(float fElapsedTime, float gameSpeed);
	void render(olc::PixelGameEngine* engine, float fElapsedTime);
	void setScrolling();
	void setIdle();
	void reset();
};

void Ground::update(float fElapsedTime, float gameSpeed) {
	if (state == GroundState::IDLE) {
		return;
	}

	fPosition = { fPosition.x -= gameSpeed, fGroundY };

	// reposition to 0,0
	if (0 > fPosition.x + fWidth) fPosition.x = fPosition.x + fWidth;
}

void Ground::render(olc::PixelGameEngine* engine, float fElapsedTime) {
  #ifndef DEBUG_MODE
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
  #endif // !DEBUG_MODE

  #ifdef DEBUG_MODE
  engine->DrawRect({0, (int)(GAME_HEIGHT - dGround->sprite->height * fScale)}, {GAME_WIDTH, (int)(dGround->sprite->height * fScale)}, {0, 255, 0});
  #endif // DEBUG_MODE
}

void Ground::init() {
	fScale = 0.5f;

	sGround = new olc::Sprite((std::string)"./assets/images/ground.png");
	dGround = new olc::Decal(sGround);
  fScale = (float)GROUND_HEIGHT / (float)sGround->height;
	fHeight = sGround->height * fScale;
	fWidth = sGround->width * fScale;
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
