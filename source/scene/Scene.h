#pragma once

#define OLC_SOUNDWAVE
#include "../lib/olcSoundWaveEngine.h"

#include "Bird.h"
#include "Fire.h"
#include "Pipes.h"
#include "Background.h"
#include "Ground.h"
#include "Ceiling.h"
#include "Pipes.h"
#include "../GameState.h"
#include "../GameDifficulty.h"
#include "../Settings.h"

class GameEngine;

class Scene {
public:
  Scene();
  ~Scene();
	Scene(GameEngine * engine);

private:
	//Scene objects;
	Bird sBird;
	Background sBackground;
	Ground sGround;
	Ceiling sCeiling;
	Pipes sPipes;

	float fFirstPipe = GAME_WIDTH / 1.5;
	float fPipeGap = GAME_WIDTH / 3;
  float fGameDistance = 0.0f;

	uint16_t gCurScore;
	uint16_t gTopScore;

	olc::sound::WaveEngine sEngine;
	olc::sound::Wave sBgMusic;
	olc::sound::Wave sBirdFlap;
	olc::sound::Wave sBirdDeath;
	olc::sound::Wave sBirdScore;

	GameEngine * gEngine = nullptr;

private:
	void loadAssets();

public:
	void initScene();
	float tick(float fElapsedTime, GameDifficulty * difficulty);
	void resetScene();
	void render(olc::PixelGameEngine* engine, float fElapsedTime);
	void setGameState(GameState state);
  bool checkCollisions();
	void jump();
  void score();
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

  sPipes.setScoreOffX(BIRD_X + sBird.getWidth());

	// Sounds
	sEngine.InitialiseAudio(88000, 2);
	sEngine.SetOutputVolume(0.25f);
	sEngine.PlayWaveform(&sBgMusic, true);
}

float Scene::tick(float fElapsedTime, GameDifficulty * difficulty) {
  fGameDistance += difficulty->gameSpeed();

	sBird.update(fElapsedTime);
	sBackground.update(fElapsedTime);
	sGround.update(fElapsedTime, difficulty->gameSpeed());
	sCeiling.update(fElapsedTime, difficulty->gameSpeed());
	sPipes.update(fElapsedTime, difficulty->gameSpeed());

	if (checkCollisions()) {
		gEngine->setGameState(GameState::GAMEOVER);
	}

  return fGameDistance;
}

bool Scene::checkCollisions() {
  // Bounds check
  if (sBird.checkCollisionBounds()) {
    return true;
  }

  // Pipes check
  for (auto * pipe : vPipes) {
    // Skip!
    if (pipe->bTraversed || !pipe->isVisible()) continue;
      
  }

  return false;
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
  fGameDistance = 0.0f;
}

void Scene::setGameState(GameState state) {
	switch (state) { 
	case GameState::IDLE:
		sBird.setIdle();
		sBackground.setIdle();
		sGround.setIdle();
		sCeiling.setIdle();
		sPipes.setIdle();
		break;

	case GameState::PLAYING:
		sBird.setPlaying();
		sBackground.setScrolling();
		sGround.setScrolling();
		sCeiling.setScrolling();
		sPipes.setScrolling();
		break;

	case GameState::GAMEOVER:
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
	sBird.flapped();
	sEngine.PlayWaveform(&sBirdFlap);
}

void Scene::score() {
  sEngine.PlayWaveform(&sBirdScore);
}

Scene::Scene() {}

Scene::~Scene() {
	sEngine.DestroyAudio();
}

Scene::Scene(GameEngine * engine) {
	gEngine = engine;
}