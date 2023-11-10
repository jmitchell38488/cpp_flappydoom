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
	bool bCanScroll = true;

private:
	void drawDebugOutline(olc::PixelGameEngine* engine, olc::vf2d pos, olc::Decal* decal, olc::vf2d scale);

public:
	Background() {
		fAnimRate = 1.0f / 55;
	}

	void init();
	void setCanScroll(bool scroll);
	void update(float fElapsedTime);
	void render(olc::PixelGameEngine* engine, float fElapsedTime);
};

void Background::update(float fElapsedTime) {
	if (!bCanScroll) {
		fLastAnimTime = 0.0f;
		return;
	}

	fLastAnimTime += fElapsedTime;
	if (fLastAnimTime < fAnimRate) return;

	fLastAnimTime = 0.0f;

	fPosition = { fPosition.x -= fScrollRate, 0 };

	// reposition to 0,0
	if (0 > fPosition.x + fWidth) fPosition.x = fPosition.x + fWidth;
}

void Background::render(olc::PixelGameEngine* engine, float fElapsedTime) {
	// Draw frame 1
	engine->DrawDecal(fPosition, bgDecal, { fScale, fScale });
	drawDebugOutline(engine, fPosition, bgDecal, { fScale, fScale });
	
	// Draw frame 2?
	olc::vf2d newPos = { fPosition.x + fWidth, 0 };
	if (fPosition.x + fWidth < engine->ScreenWidth()) {
		// Draw partial decal, not the full decal
		engine->DrawDecal(newPos, bgDecal, { fScale, fScale });

	}

	// Draw frame 3?
	if (fPosition.x + fWidth + fWidth < engine->ScreenWidth()) {
		olc::vf2d newPos2 = { fPosition.x + fWidth + fWidth, 0 };
		// Draw partial decal, not the full decal
		engine->DrawDecal(newPos2, bgDecal, { fScale, fScale });

	}
}

void Background::drawDebugOutline(olc::PixelGameEngine* engine, olc::vf2d pos, olc::Decal* decal, olc::vf2d scale) {
	int minX = fPosition.x, maxX = bgSprite->width * fScale;
	int minY = fPosition.y, maxY = bgSprite->height * fScale;
	// {0,0} -> {1,0} Top left - Top right
	engine->DrawLine({minX, minY }, { maxX, minY }, olc::RED);
	// {0,1} -> {1,1} Bottom left - Bottom right
	engine->DrawLine({ minX, maxY }, { maxX, maxY }, olc::RED);
	// {0,0} -> {0,1} Top left - Bottom Left
	engine->DrawLine({ minX, minY }, { minX, maxY }, olc::RED);
	// {1,0} -> {1,1} Top right - Bottom right
	engine->DrawLine({ maxX, minY }, { maxX, maxY }, olc::RED);
}

void Background::init() {
	fScale = 0.5f;

	bgSprite = new olc::Sprite((std::string)"./assets/images/bg.png");
	bgDecal = new olc::Decal(bgSprite);
	fHeight = bgSprite->height * fScale;
	fWidth = bgSprite->width * fScale;
	fScrollRate = (GAME_TICK) * fWidth/8;
}

void Background::setCanScroll(bool scroll) {
	bCanScroll = scroll;
}