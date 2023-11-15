#pragma once

#define OLC_PGE_APPLICATION
#include "lib/olcPixelGameEngine.h"
#include "Settings.h"
#include "GameState.h"
#include "GameDifficulty.h"
#include "GameScript.h"
#include "scene/Scene.h"

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
  gScene->setGame(this);
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