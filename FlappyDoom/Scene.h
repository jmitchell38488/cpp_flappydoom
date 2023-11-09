#pragma once

#include "Bird.h";
#include "GameObjects.h";
#include "GameObj.h";
#include "Fire.h";
#include "GameObjects.h";
#include "Pipes.h";

class Scene {
private:
	GameObjects gameObjs;

	//Scene objects;
	Bird sBird;
	//Background sBackground;
	olc::Decal* sBackground = nullptr;

	void loadAssets() {
		Bird sBird;
		sBird.init();
		gameObjs.addGameObject(sBird);
	}

public:
	void initScene() {
		loadAssets();
	}

	bool tick(float fElapsedTime) {
		gameObjs.updateState(fElapsedTime);
		return true;
	}

	void render(olc::PixelGameEngine* engine, float fElapsedTime) {
		engine->Clear(olc::BLACK);
		engine->DrawString({ 20, 20 }, "fElapsedTime: " + std::to_string(fElapsedTime), olc::Pixel(255, 255, 255), 1);
		gameObjs.render(engine, fElapsedTime);
	}

};