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

	Scene* scene = nullptr;

public:
	Game()
	{
		// Name your application
		sAppName = "FlappyDOOM";
		scene = new Scene();
	}

public:
	bool OnUserCreate() override
	{
		scene->initScene();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		fAccumulatedTime += fElapsedTime;
		if (fAccumulatedTime > fTickRate) {
			scene->tick(fElapsedTime);
		}

		scene->render(this, fElapsedTime);
		return true;
	}

	void newGame() {

	}

public:
	void run() {
		if (Construct(GAME_WIDTH, GAME_HEIGHT, GAME_PIXEL, GAME_PIXEL)) {
			Start();
		}
	}
};