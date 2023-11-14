#pragma once

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Settings.h"
#include "GameState.h"
#include "GameDifficulty.h"
#include "GameScript.h"
#include "Scene.h"

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

  GameScriptProcessor gScriptProcessor;

public:
  float fGameScore = 0.0f;
  bool bPlaying = false;
  GameState gState;
  GameDifficulty *gDifficulty = nullptr;

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
}

GameEngine::~GameEngine() {}

bool GameEngine::initialise()
{
  gCommand::gEngine = this;
  Scene::get().initScene();
  gDifficulty->setDifficulty(DifficultyMode::EASY);
  return true;
}

void GameEngine::setGameState(GameState state)
{
  gState = state;
  Scene::get().setGameState(gState);

  if (gState == GameState::GAMEOVER)
    gDifficulty->setDifficulty(DifficultyMode::EASY);
}

void GameEngine::resetGame()
{
  fGameScore = 0.0f;
  Scene::get().resetScene();
  bPlaying = false;
  gState = GameState::IDLE;
}

void GameEngine::addScore()
{
  if (gState == GameState::GAMEOVER) return;
  Scene::get().score();
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
      Scene::get().setGameState(GameState::PLAYING);
      bPlaying = true;
      Scene::get().jump();
    }
    else
    {
      Scene::get().jump();
    }
  }

  if (GetKey(olc::Key::ENTER).bPressed)
  {
    if (bPlaying)
    {
      Scene::get().setGameState(GameState::IDLE);
      bPlaying = false;
    }
  }

  if (GetKey(olc::Key::ESCAPE).bPressed)
  {
    Scene::get().resetScene();
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
      float fGameDistance = Scene::get().tick(fElapsedTime, gDifficulty);
      gDifficulty->gameUpdate(fGameDistance);
    }
  }

  Scene::get().render(this, fElapsedTime);

  DrawStringDecal({20, 20}, "Score: " + std::to_string((int)std::round(fGameScore)));
  DrawStringDecal({20, 35}, "Difficulty: " + gDifficulty->getMode());
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