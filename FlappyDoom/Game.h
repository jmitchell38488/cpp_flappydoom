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
	bool bPlaying = false;

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

		if (GetKey(olc::Key::SPACE).bPressed) {
			if (!bPlaying) {
				scene->setSceneState(SceneState::PLAYING);
				bPlaying = true;
			}
			scene->jump();
		}

		if (GetKey(olc::Key::ENTER).bPressed) {
			if (bPlaying) {
				scene->setSceneState(SceneState::IDLE);
				bPlaying = false;
			}
		}

		if (GetKey(olc::Key::ESCAPE).bPressed) {
			scene->resetScene();
			bPlaying = false;
		}

		if (GetKey(olc::Key::Q).bPressed) {
			exit(0);
		}

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