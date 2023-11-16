#pragma once

#include "../lib/olcPixelGameEngine.h"
#include "../Settings.h"
#include "../definitions.h"

enum class BackgroundState {
	IDLE, SCROLLING
};

class Background {
private:
	float fScale = 1.0f;
	float fWidth = 1.0f;
	float fHeight = 1.0f;

	olc::vf2d fPosition;
	olc::Sprite* sBackground;
	olc::Decal* dBackground;

	float fAnimRate = ANIM_TICK;
	float fLastAnimTime = 0.0f;
	float fScrollRate = 0.0f;
	float fY = 25.0f;

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

	fLastAnimTime += fElapsedTime;
	if (fLastAnimTime < fAnimRate) return;

	fLastAnimTime = 0.0f;

	fPosition = { fPosition.x -= fScrollRate, fY };

	// reposition to 0,fY
	if (0 > fPosition.x + fWidth) fPosition.x = fPosition.x + fWidth;
}

void Background::render(olc::PixelGameEngine* engine, float fElapsedTime) {
  #ifndef DEBUG_MODE
	// Draw frame 1
	engine->DrawDecal(fPosition, dBackground, { fScale, fScale });
	
	// Draw frame 2?
	olc::vf2d newPos = { fPosition.x + fWidth, fPosition.y };
	if (fPosition.x + fWidth < engine->ScreenWidth()) {
		// Draw partial decal, not the full decal
		engine->DrawDecal(newPos, dBackground, { fScale, fScale });

	}

	// Draw frame 3?
	if (fPosition.x + fWidth + fWidth < engine->ScreenWidth()) {
		olc::vf2d newPos2 = { fPosition.x + fWidth + fWidth, fPosition.y };
		// Draw partial decal, not the full decal
		engine->DrawDecal(newPos2, dBackground, { fScale, fScale });

	}
  
  #endif // !DEBUG_MODE
}

void Background::init() {
	fScale = 0.5f;

	sBackground = new olc::Sprite((std::string)"./assets/images/bg.png");
	dBackground = new olc::Decal(sBackground);
	fHeight = sBackground->height * fScale;
	fWidth = sBackground->width * fScale;
	fScrollRate = (GAME_TICK) * fWidth/8;
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