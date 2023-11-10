#pragma once

#include "Bird.h";
#include "Fire.h";
#include "Pipes.h";
#include "Background.h"
#include "Ground.h"

enum class SceneState {
	IDLE, PLAYING, GAMEOVER
};

class Scene {
private:
	//Scene objects;
	Bird sBird;
	Background sBackground;
	Ground sGround;

	uint16_t gCurScore;
	uint16_t gTopScore;

	SceneState state = SceneState::IDLE;

private:
	void loadAssets();

public:
	void initScene();
	bool tick(float fElapsedTime);
	void resetScene();
	void render(olc::PixelGameEngine* engine, float fElapsedTime);
	void setSceneState(SceneState newState);
	void jump();
};

void Scene::loadAssets() {
	sBird.init();
	sBackground.init();
	sGround.init();
}

void Scene::initScene() {
	loadAssets();
	sBird.setIdle();
	sBackground.setIdle();
	sGround.setIdle();
}

bool Scene::tick(float fElapsedTime) {
	sBird.update(fElapsedTime);
	sBackground.update(fElapsedTime);
	sGround.update(fElapsedTime);

	if (sBird.checkCollision()) {
		setSceneState(SceneState::GAMEOVER);
	}

	return true;
}

void Scene::render(olc::PixelGameEngine* engine, float fElapsedTime) {
	engine->Clear(olc::BLACK);
	sBackground.render(engine, fElapsedTime);
	sGround.render(engine, fElapsedTime);

	// Render last z index
	sBird.render(engine, fElapsedTime);
}

void Scene::resetScene() {
	sBird.reset();
	sBackground.reset();
	sGround.reset();
	state = SceneState::IDLE;
}

void Scene::setSceneState(SceneState newState) {
	state = newState;
	switch (state) {
	case SceneState::IDLE:
		sBird.setIdle();
		sBackground.setIdle();
		sGround.setIdle();
		break;

	case SceneState::PLAYING:
		sBird.setPlaying();
		sBackground.setScrolling();
		sGround.setScrolling();
		break;

	case SceneState::GAMEOVER:
		sBird.setDead();
		sBackground.setIdle();
		sGround.setIdle();
	}
}

void Scene::jump() {
	sBird.flapped();
}