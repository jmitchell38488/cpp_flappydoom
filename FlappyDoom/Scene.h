#pragma once

#include "Bird.h";
#include "Fire.h";
#include "Pipes.h";
#include "Background.h"

class Scene {
private:

	//Scene objects;
	Bird sBird;
	Background sBackground;

	void loadAssets() {
		sBird.init();
		sBackground.init();
	}

	uint16_t gCurScore;
	uint16_t gTopScore;

public:
	void initScene() {
		loadAssets();
	}

	bool tick(float fElapsedTime) {
		sBird.update(fElapsedTime);
		sBackground.update(fElapsedTime);
		return true;
	}

	void resetScene() {

	}

	void render(olc::PixelGameEngine* engine, float fElapsedTime) {
		engine->Clear(olc::BLACK);
		sBackground.render(engine, fElapsedTime);
		sBird.render(engine, fElapsedTime);
		engine->DrawString({ 20, 20 }, "fElapsedTime: " + std::to_string(fElapsedTime), olc::Pixel(255, 255, 255), 1);
	}

};