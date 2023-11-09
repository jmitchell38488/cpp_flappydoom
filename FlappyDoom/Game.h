#pragma once

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Settings.h";
#include "Scene.h";


// Override base class with your custom functionality
class Game : public olc::PixelGameEngine
{
private:
	float fTickRate = GAME_TICK;
	float fAnimRate = ANIM_TICK;
	float fAccumulatedTime = 0.0f;
	float fLastAnimTime = 0.0f;

	//Scene* scene;

	//Scene objects;
	Bird sBird;
	//Background sBackground;
	olc::Decal* sBackground = nullptr;

public:
	Game()
	{
		// Name your application
		sAppName = "FlappyDOOM";
	}

public:
	bool OnUserCreate() override
	{
		loadAssets();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		fAccumulatedTime += fElapsedTime;
		if (fAccumulatedTime > fTickRate) {
			//scene->tick(fElapsedTime);
			sBird.update(fElapsedTime);
			//fScene.tick(fElapsedTime);
		}

		Clear(olc::BLACK);
		DrawString({ 20, 20 }, "fElapsedTime: " + std::to_string(fElapsedTime), olc::Pixel(255, 255, 255), 1);
		if (sBackground != nullptr) {
			DrawDecal({ 0, 0 }, sBackground, { 0.25f, 0.25f });
		}
		
		sBird.render(this, fElapsedTime);
		return true;
	}

	void newGame() {

	}

	void loadAssets() {
		sBird.init();
		olc::Sprite* bgSprite = new olc::Sprite((std::string)"./assets/images/bg.png");
		sBackground = new olc::Decal(bgSprite);
	}

public:
	void run() {
		if (Construct(GAME_WIDTH, GAME_HEIGHT, GAME_PIXEL, GAME_PIXEL)) {
			Start();
		}
	}
};