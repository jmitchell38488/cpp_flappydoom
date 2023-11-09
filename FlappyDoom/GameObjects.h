#pragma once
#include "Animation.h";

class GameObjects {
private:
	std::vector<Animation> gameObjs;
	size_t objsCreated = 0;

public:
	GameObjects();

	void addGameObject(Animation gameObj) {
		gameObjs.push_back(gameObj);
		objsCreated++;
	}

	void updateState(float fElapsedTime) {
		for (Animation gObj : gameObjs) {
			gObj.update(fElapsedTime);
		}
	}

	void render(olc::PixelGameEngine* engine, float fElapsedTime) {
		if (gameObjs.empty()) {
			return;
		}

		for (Animation gObj : gameObjs) {
			gObj.render(engine, fElapsedTime);
		}
	}
};

GameObjects::GameObjects() {

}