#pragma once

#include "Bird.h";
#include "Fire.h";
#include "Pipes.h";
#include "Background.h"

class Scene {
private:

	//Scene objects;
	Bird sBird;
	Background sBg;
	//Background sBackground;
	olc::Decal* sBackground = nullptr;

	void loadAssets() {
		sBird.init();
	}

public:
	void initScene() {
		loadAssets();
	}

	bool tick(float fElapsedTime) {
		sBird.update(fElapsedTime);
		return true;
	}

	void render(olc::PixelGameEngine* engine, float fElapsedTime) {
		engine->Clear(olc::BLACK);
		engine->DrawString({ 20, 20 }, "fElapsedTime: " + std::to_string(fElapsedTime), olc::Pixel(255, 255, 255), 1);
		sBird.render(engine, fElapsedTime);
	}

};