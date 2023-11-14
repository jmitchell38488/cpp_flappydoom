#pragma once

#define OLC_SOUNDWAVE
#include "olcSoundWaveEngine.h"

#include "Bird.h"
#include "Fire.h"
#include "Pipes.h"
#include "Background.h"
#include "Ground.h"
#include "Ceiling.h"
#include "Pipes.h"

enum class SceneState {
	IDLE, PLAYING, GAMEOVER
};

class Scene {
private:
	//Scene objects;
	Bird sBird;
	Background sBackground;
	Ground sGround;
	Ceiling sCeiling;
	Pipes sPipes;

	float fFirstPipe = GAME_WIDTH / 1.5;
	float fPipeGap = GAME_WIDTH / 3;


	uint16_t gCurScore;
	uint16_t gTopScore;

	SceneState state = SceneState::IDLE;

	olc::sound::WaveEngine sEngine;
	olc::sound::Wave sBgMusic;
	olc::sound::Wave sBirdFlap;
	olc::sound::Wave sBirdDeath;
	olc::sound::Wave sBirdScore;

private:
	void loadAssets();

public:
	Scene();
	~Scene();
	void initScene();
	bool tick(float fElapsedTime);
	void resetScene();
	void render(olc::PixelGameEngine* engine, float fElapsedTime);
	void setSceneState(SceneState newState);
	void jump();
  bool isGameOver();
};

void Scene::loadAssets() {
	sBird.init();
	sBackground.init();
	sGround.init();
	sCeiling.init();
	sPipes.init();

	sBgMusic.LoadAudioWaveform("./assets/sound/theme2.wav");
	sBirdFlap.LoadAudioWaveform("./assets/sound/wing2.wav");
	sBirdDeath.LoadAudioWaveform("./assets/sound/hit2.wav");
	sBirdScore.LoadAudioWaveform("./assets/sound/point2.wav");
}

void Scene::initScene() {
	loadAssets();
	sBird.setIdle();
	sBackground.setIdle();
	sGround.setIdle();
	sCeiling.setIdle();
	sPipes.setIdle();


	// Sounds
	sEngine.InitialiseAudio(88000, 2);
	sEngine.SetOutputVolume(0.25f);
	sEngine.PlayWaveform(&sBgMusic, true);
}

bool Scene::tick(float fElapsedTime) {
	if (state == SceneState::GAMEOVER) return true;

	sBird.update(fElapsedTime);
	sBackground.update(fElapsedTime);
	sGround.update(fElapsedTime);
	sCeiling.update(fElapsedTime);
	sPipes.update(fElapsedTime);

	if (sBird.checkCollision()) {
		if (state != SceneState::GAMEOVER)
			setSceneState(SceneState::GAMEOVER);
	}

	return true;
}

void Scene::render(olc::PixelGameEngine* engine, float fElapsedTime) {
	engine->Clear(olc::BLACK);
	sBackground.render(engine, fElapsedTime);
	sPipes.render(engine, fElapsedTime);
	sGround.render(engine, fElapsedTime);
	sCeiling.render(engine, fElapsedTime);
	

	// Render last z index
	sBird.render(engine, fElapsedTime);
}

void Scene::resetScene() {
	sBird.reset();
	sBackground.reset();
	sGround.reset();
	sCeiling.reset();
	sPipes.reset();
	state = SceneState::IDLE;
}

void Scene::setSceneState(SceneState newState) {
	state = newState;
	switch (state) { 
	case SceneState::IDLE:
		sBird.setIdle();
		sBackground.setIdle();
		sGround.setIdle();
		sCeiling.setIdle();
		sPipes.setIdle();
		break;

	case SceneState::PLAYING:
		sBird.setPlaying();
		sBackground.setScrolling();
		sGround.setScrolling();
		sCeiling.setScrolling();
		sPipes.setScrolling();
		break;

	case SceneState::GAMEOVER:
		sEngine.PlayWaveform(&sBirdDeath);
		sBird.setDead();
		sBackground.setIdle();
		sGround.setIdle();
		sCeiling.setIdle();
		sPipes.setIdle();
		break;
	}
}

void Scene::jump() {
	if (state == SceneState::GAMEOVER) return;
	sBird.flapped();
	sEngine.PlayWaveform(&sBirdFlap);
}

Scene::Scene() {
	
}

Scene::~Scene() {
	sEngine.DestroyAudio();
}

bool Scene::isGameOver() {
  return state == SceneState::GAMEOVER;
}