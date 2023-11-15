#pragma once

#define OLC_SOUNDWAVE
#define OLC_PGE_APPLICATION

#include "lib/olcSoundWaveEngine.h"
#include "lib/olcPixelGameEngine.h"

#include "scene/Bird.h"
#include "scene/Fire.h"
#include "scene/Background.h"
#include "scene/Ground.h"
#include "scene/Ceiling.h"
#include "scene/Pipes.h"
#include "GameState.h"
#include "GameDifficulty.h"
#include "GameScript.h"

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

class GameEngine : public olc::PixelGameEngine
{

public:
  GameEngine();
  ~GameEngine();

private:
  float fTickRate = GAME_TICK;
  float fAnimRate = ANIM_TICK;
  float fAccumulatedTime = 0.0f;
  float fLastAnimTime = 0.0f;

public:
  float fGameScore = 0.0f;
  bool bPlaying = false;
  GameState gState;
  GameDifficulty *gDifficulty = nullptr;
  Scene * gScene = nullptr;
  GameScriptProcessor * gScriptProcessor = nullptr;

public:
  bool initialise();
  void setGameState(GameState state);
  void jump();
  void addScore();
  void resetGame();
  bool update(float fElapsedTime);
  void setDifficultyMode(DifficultyMode mode);
  void run();
  bool OnUserCreate() override;
  bool OnUserUpdate(float fElapsedTime) override;
};

GameEngine::GameEngine()
{
  // Name your application
  sAppName = "FlappyDOOM";

  gDifficulty = new GameDifficulty();
  gScriptProcessor = new GameScriptProcessor();
  gScene = new Scene();
}

GameEngine::~GameEngine() {}

bool GameEngine::initialise()
{
  gScene->initScene();
  gDifficulty->setDifficulty(DifficultyMode::EASY);
  return true;
}

void GameEngine::setGameState(GameState state)
{
  gState = state;
  gScene->setGameState(gState);

  if (gState == GameState::GAMEOVER)
    gDifficulty->setDifficulty(DifficultyMode::EASY);
}

void GameEngine::resetGame()
{
  fGameScore = 0.0f;
  gScene->resetScene();
  bPlaying = false;
  gState = GameState::IDLE;
}

void GameEngine::addScore()
{
  if (gState == GameState::GAMEOVER) return;
  gScene->score();
  fGameScore += gDifficulty->gameScore();
}

void GameEngine::jump() {
  if (gState == GameState::GAMEOVER) return;
}

bool GameEngine::update(float fElapsedTime)
{
  fAccumulatedTime += fElapsedTime;
  if (GetKey(olc::Key::SPACE).bPressed)
  {
    if (gState == GameState::GAMEOVER)
    {
      resetGame();
    }
    else if (!bPlaying)
    {
      gScene->setGameState(GameState::PLAYING);
      bPlaying = true;
      gScene->jump();
    }
    else
    {
      gScene->jump();
    }
  }

  if (GetKey(olc::Key::ENTER).bPressed)
  {
    if (bPlaying)
    {
      gScene->setGameState(GameState::IDLE);
      bPlaying = false;
    }
  }

  if (GetKey(olc::Key::ESCAPE).bPressed)
  {
    gScene->resetScene();
    bPlaying = false;
  }

  if (GetKey(olc::Key::Q).bPressed)
  {
    exit(0);
  }

  // Update game state 1 / 60
  if (fAccumulatedTime > fTickRate)
  {
    if (gState == GameState::PLAYING) {
      float fGameDistance = gScene->tick(fElapsedTime, gDifficulty);
      gDifficulty->gameUpdate(fGameDistance);
    }
  }

  gScene->render(this, fElapsedTime);

  DrawStringDecal({20, 20}, "Score: " + std::to_string((int)std::round(fGameScore)));
  DrawStringDecal({20, 35}, "Difficulty: " + gDifficulty->getMode());

  return true;
}

void GameEngine::setDifficultyMode(DifficultyMode mode)
{
  gDifficulty->setDifficulty(mode);
}

bool GameEngine::OnUserCreate()
{
  return initialise();
}

bool GameEngine::OnUserUpdate(float fElapsedTime)
{
  return update(fElapsedTime);
}

void GameEngine::run()
{
  if (Construct(GAME_WIDTH, GAME_HEIGHT, GAME_PIXEL, GAME_PIXEL))
  {
    Start();
  }
}

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
  if (sBird.checkCollision()) {
    return true;
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