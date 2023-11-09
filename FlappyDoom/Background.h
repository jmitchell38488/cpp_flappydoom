#pragma once

#include "olcPixelGameEngine.h"

class Background {
private:
	float fScale = 1.0f;
	float fWidth = 1.0f;
	float fHeight = 1.0f;

	olc::vf2d fPosition;
	olc::Sprite* bgSprite;
	olc::Decal* bgDecal;

	float fAnimRate = ANIM_TICK;
	float fLastAnimTime = 0.0f;
	float fScrollRate = 0.0f;

public:
	Background() {}

	void init();
	void update(float fElapsedTime);
	void render(olc::PixelGameEngine* engine, float fElapsedTime);
};

void Background::update(float fElapsedTime) {

}

void Background::render(olc::PixelGameEngine* engine, float fElapsedTime) {
	engine->DrawDecal(fPosition, bgDecal, { fScale, fScale });
	/*if (sBackground != nullptr) {
		DrawDecal({ 0, 0 }, sBackground, { 0.25f, 0.25f });
	}*/
}

void Background::init() {
	fScale = 0.25f;

	bgSprite = new olc::Sprite((std::string)"./assets/images/bg.png");
	bgDecal = new olc::Decal(bgSprite);
	fHeight = bgSprite->height * fScale;
	fWidth = bgSprite->width * fScale;
	fScrollRate = 1.0f / 12 * fWidth;
}