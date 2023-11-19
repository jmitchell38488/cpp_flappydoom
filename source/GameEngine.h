#pragma once

#define OLC_SOUNDWAVE
#define OLC_PGEX_MINIAUDIO
#define OLC_IMAGE_STB
#define OLC_PGE_APPLICATION

#include "lib/miniaudio.h"
#include "lib/olcPGEX_MiniAudio.h"
#include "lib/olcPixelGameEngine.h"

#include "definitions.h"
#include "scene/Bird.h"
#include "scene/Fire.h"
#include "scene/Background.h"
#include "scene/Ground.h"
#include "scene/Ceiling.h"
#include "scene/Pipes.h"
#include "scene/Pipe.h"
#include "GameState.h"
#include "GameDifficulty.h"
#include "GameScript.h"
#include "Settings.h"
#include "Score.h"
#include "Util.h"

#include <list>
#include <chrono>
#include <ctime>

class GameSoundManager {
private:
  int aBgMusic;
  int aBirdFlap;
  int aBirdDeath;
  int aBirdScore;

  bool bBgMusicPlaying;
  bool bBirdFlapPlaying;
  bool bBirdDeathPlaying;
  bool bBirdScorePlaying;

  olc::MiniAudio ma;

  float aVol = 0.25f;

private:
  void AdjustVol(int sound, float vol = 1.0f) {
    if (vol != 1.0f) {
      float nVol = std::fmax(0.0f, std::fmin(1.0f, vol));
      ma.SetVolume(sound, nVol);
    }
  }

public:
  void LoadResources() {
    aBgMusic = ma.LoadSound("./assets/sound/theme.mp3");
    aBirdFlap = ma.LoadSound("./assets/sound/wing2.wav");
    aBirdDeath = ma.LoadSound("./assets/sound/hit2.wav");
    aBirdScore = ma.LoadSound("./assets/sound/point2.wav");
  }

  void StartGame() {
    ma.SetVolume(aBgMusic, aVol);
    ma.Play(aBgMusic, true);
  }

  void SetGlobalVolume(float vol) {
    aVol = vol;
    ma.SetVolume(aBgMusic, aVol);
    ma.SetVolume(aBirdFlap, aVol);
    ma.SetVolume(aBirdDeath, aVol);
    ma.SetVolume(aBirdScore, aVol);
  }

  void PlayFlap(float vol = 1.0f) {
    AdjustVol(aBirdFlap, vol);

  if (!ma.IsPlaying(aBirdFlap))
      ma.Stop(aBirdFlap);
    ma.Play(aBirdFlap);
  }

  void PlayDeath(float vol = 1.0f) {
    AdjustVol(aBirdDeath, vol);
    if (!ma.IsPlaying(aBirdDeath))
      ma.Stop(aBirdDeath);
    ma.Play(aBirdDeath);
  }

  void PlayScore(float vol = 1.0f) {
    AdjustVol(aBirdScore, vol);
    if (!ma.IsPlaying(aBirdScore))
      ma.Stop(aBirdScore);
    ma.Play(aBirdScore);
  }

  void PauseGame() {
    bBgMusicPlaying = ma.IsPlaying(aBgMusic);
    bBirdFlapPlaying = ma.IsPlaying(aBirdFlap);
    bBirdDeathPlaying = ma.IsPlaying(aBirdDeath);
    bBirdScorePlaying = ma.IsPlaying(aBirdScore);

    ma.Toggle(aBgMusic);
    if (bBirdFlapPlaying) ma.Toggle(aBirdFlap);
    if (bBirdDeathPlaying) ma.Toggle(aBirdDeath);
    if (bBirdScorePlaying) ma.Toggle(aBirdScore);
  }

  void ResumeGame() {
    ma.Toggle(aBgMusic);
    if (bBirdFlapPlaying) ma.Toggle(aBirdFlap);
    if (bBirdDeathPlaying) ma.Toggle(aBirdDeath);
    if (bBirdScorePlaying) ma.Toggle(aBirdScore);

    bBgMusicPlaying = false;
    bBirdFlapPlaying = false;
    bBirdDeathPlaying = false;
    bBirdScorePlaying = false;
  }

  void Cleanup() {
    if (ma.IsPlaying(aBgMusic))
      ma.Stop(aBgMusic);
    if (ma.IsPlaying(aBirdFlap))
      ma.Stop(aBirdFlap);
    if (ma.IsPlaying(aBirdDeath))
      ma.Stop(aBirdDeath);
    if (ma.IsPlaying(aBirdScore))
      ma.Stop(aBirdScore);

    ma.UnloadSound(aBgMusic);
    ma.UnloadSound(aBirdFlap);
    ma.UnloadSound(aBirdDeath);
    ma.UnloadSound(aBirdScore);
    ma.~MiniAudio();
  }
};

class GameEngine;
class GameSoundManager;

class Scene
{
public:
  Scene();
  ~Scene();
  Scene(GameEngine *engine);

private:
  // Scene objects;
  Bird sBird;
  Background sBackground;
  Ground sGround;
  Ceiling sCeiling;
  Pipes sPipes;

  float fFirstPipe = GAME_WIDTH / 1.5;
  float fPipeGap = GAME_WIDTH / 3;
  float fGameDistance = 0.0f;
  float fRenders = 0.0f;
  olc::Sprite * sPaused;
  olc::Decal * dPaused;

  uint16_t gCurScore;
  uint16_t gTopScore;

  GameEngine *gEngine = nullptr;
  GameSoundManager gSoundMan;

private:
  void loadAssets();

public:
  void initScene();
  float tick(float fElapsedTime, GameDifficulty *difficulty);
  void resetScene();
  void render(olc::PixelGameEngine *engine, float fElapsedTime);
  void setGameState(GameState state);
  bool checkCollisions();
  void jump();
  void score();
  void resumeGame();
  void pauseGame();
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
  float fLag = 0.0f;
  GameState gLastState;

public:
  float fGameScore = 0.0f;
  bool bPlaying = false;
  GameState gState;
  GameDifficulty *gDifficulty = nullptr;
  Scene *gScene = nullptr;
  GameScriptProcessor *gScriptProcessor = nullptr;
  std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;

protected:
  void handleInput(float fElapsedTime);
  void doGameUpdate(float fElapsedTime);

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
  float getRunTime();
};

GameEngine::GameEngine()
{
  // Name your application
  sAppName = "FlappyDOOM";

  gDifficulty = new GameDifficulty();
  gScriptProcessor = new GameScriptProcessor();
  gScene = new Scene(this);
  m_tp1 = std::chrono::system_clock::now();
  m_tp2 = std::chrono::system_clock::now();
}

GameEngine::~GameEngine()
{
}

bool GameEngine::initialise()
{
  gScene->initScene();
  gDifficulty->setDifficulty(DifficultyMode::EASY);
  gState = GameState::IDLE;
  bPlaying = false;

  Pipe::gEngine = this;
  return true;
}

void GameEngine::setGameState(GameState state)
{
  gState = state;
  gScene->setGameState(gState);

  switch (state)
  {
  case GameState::PLAYING:
    bPlaying = true;
    break;

  case GameState::IDLE:
    bPlaying = false;
    gScene->setGameState(GameState::IDLE);
    break;

  case GameState::GAMEOVER:
    bPlaying = false;
    break;

  case GameState::PAUSED:
    gScene->setGameState(GameState::PAUSED);
    break;
  }
}

void GameEngine::resetGame()
{
  fGameScore = 0.0f;
  if (!gSettings.CLAMP_DIFFICULTY)
    gDifficulty->setDifficulty(DifficultyMode::EASY);

  gSettings.GAME_PAUSED = false;
  gScene->resetScene();
  setGameState(GameState::IDLE);
  bPlaying = false;
  gScore.score = 0;
}

void GameEngine::addScore()
{
  if (gState == GameState::GAMEOVER)
    return;
  gScene->score();
  fGameScore += gDifficulty->gameScore();
}

void GameEngine::jump()
{
  if (gState == GameState::GAMEOVER)
    return;
}

void GameEngine::handleInput(float fElapsedTime)
{
  if (GetKey(olc::Key::SPACE).bPressed)
  {
    if (gState == GameState::GAMEOVER)
    {
      if (gScore.score > gScore.topScore)
        gScore.topScore = gScore.score;
      gScore.runs++;
      resetGame();
    }
    else if (!bPlaying && gState != GameState::PAUSED)
    {
      setGameState(GameState::PLAYING);
      gScene->jump();
    }
    else if (gState != GameState::PAUSED)
    {
      gScene->jump();
    }
  }

  if (GetKey(olc::Key::ENTER).bPressed)
  {
    if (gState != GameState::PAUSED) {
      gLastState = gState;
      setGameState(GameState::PAUSED);
      gScene->pauseGame();
      gSettings.GAME_PAUSED = true;
    } else {
      setGameState(gLastState);
      gScene->resumeGame();
      fAccumulatedTime = fElapsedTime;
      gSettings.GAME_PAUSED = false;
    }
  }

  if (GetKey(olc::Key::ESCAPE).bPressed)
  {
    if (gState == GameState::GAMEOVER)
    {
      if (gScore.score > gScore.topScore)
        gScore.topScore = gScore.score;
      gScore.runs++;
    }
    resetGame();
  }

  if (GetKey(olc::Key::Q).bPressed)
  {
    exit(0);
  }

  if (GetKey(olc::Key::D).bPressed)
  {
    if (ENABLE_DEBUG_MODE)
      gSettings.DEBUG_MODE = !gSettings.DEBUG_MODE;
  }

  if (GetKey(olc::Key::P).bPressed)
  {
    if (ENABLE_DEBUG_MODE)
    {
      gSettings.CLAMP_DIFFICULTY = !gSettings.CLAMP_DIFFICULTY;
      gDifficulty->toggleNoChange();
    }
  }

  if (GetKey(olc::Key::O).bPressed)
  {
    if (ENABLE_DEBUG_MODE)
    {
      if (gSettings.CLAMP_DIFFICULTY)
      {
        gDifficulty->incDifficulty();
      }
    }
  }

  if (GetKey(olc::Key::I).bPressed)
  {
    if (ENABLE_DEBUG_MODE)
    {
      if (gSettings.CLAMP_DIFFICULTY)
      {
        gDifficulty->decDifficulty();
      }
    }
  }

  if (GetKey(olc::Key::T).bPressed)
  {
    if (ENABLE_DEBUG_MODE)
    {
      gDifficulty->decScale();
    }
  }

  if (GetKey(olc::Key::Y).bPressed)
  {
    if (ENABLE_DEBUG_MODE)
    {
      gDifficulty->incScale();
    }
  }

  if (GetKey(olc::Key::G).bPressed)
  {
    if (ENABLE_DEBUG_MODE)
    {
      gSettings.P_GAP -= 5.0f;
      if (gSettings.P_GAP < BIRD_SH)
        gSettings.P_GAP = BIRD_SH;
    }
  }

  if (GetKey(olc::Key::H).bPressed)
  {
    if (ENABLE_DEBUG_MODE)
    {
      gSettings.P_GAP += 5.0f;

      if (gSettings.P_GAP > PIPE_GAP * 2)
        gSettings.P_GAP = PIPE_GAP * 2;
    }
  }
}

void GameEngine::doGameUpdate(float fElapsedTime)
{
  while (fAccumulatedTime >= fElapsedTime)
  {
    gScriptProcessor->processCommands(fElapsedTime);

    if (gScore.newScore)
    {
      gScore.newScore = false;
      gScore.score += gDifficulty->gameScore();
      gScene->score();
    }

    float fGameDistance = gScene->tick(fElapsedTime, gDifficulty);
    gDifficulty->gameUpdate(fGameDistance);
    fAccumulatedTime -= fElapsedTime;
  }
}

bool GameEngine::update(float fElapsedTime)
{
  fLag += fElapsedTime;
  fAccumulatedTime += fElapsedTime;
  float tick = fAccumulatedTime;

  if (fAccumulatedTime < 0)
    fAccumulatedTime = 0.0f;
  if (fLag < 0)
    fLag = 0.0f;

  handleInput(fAccumulatedTime);
  doGameUpdate(GAME_TICK);

  gScene->render(this, fElapsedTime);

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

float GameEngine::getRunTime() {
  m_tp2 = std::chrono::system_clock::now();
  std::chrono::duration<float> elapsedTime = m_tp2 - m_tp1;
  return elapsedTime.count();
}

void GameEngine::run()
{
  if (Construct(GAME_WIDTH, GAME_HEIGHT, GAME_PIXEL, GAME_PIXEL, gSettings.FS_MODE, gSettings.VSYNC_MODE, true))
  {
    Start();
  }
}

void Scene::loadAssets()
{
  sBird.init();
  sBackground.init();
  sGround.init();
  sCeiling.init();
  sPipes.init(BIRD_X + BIRD_SW);

  sPaused = new olc::Sprite((std::string)"./assets/i-paused.png");
  dPaused = new olc::Decal(sPaused);

  gSoundMan.LoadResources();
}

void Scene::initScene()
{
  loadAssets();
  sBird.setIdle();
  sBackground.setIdle();
  sGround.setIdle();
  sCeiling.setIdle();
  sPipes.setIdle();
  
  gSoundMan.StartGame();
}

float Scene::tick(float fElapsedTime, GameDifficulty *difficulty)
{
  if (gEngine->gState == GameState::PLAYING)
  {
    fGameDistance += difficulty->gameSpeed();
    sBackground.update(fElapsedTime);
    sGround.update(fElapsedTime, difficulty->gameSpeed() * 1 + fElapsedTime / 2);
    sCeiling.update(fElapsedTime, difficulty->gameSpeed() * 1 + fElapsedTime / 2);
    sPipes.update(fElapsedTime, difficulty->gameSpeed() * 1 + fElapsedTime / 2);
  }

  sBird.update(fElapsedTime, gEngine->getRunTime());

  if (checkCollisions() && gEngine->gState == GameState::PLAYING)
  {
    gEngine->setGameState(GameState::GAMEOVER);
  }

  return fGameDistance;
}

bool Scene::checkCollisions()
{
  return sBird.checkCollisionBounds() || sPipes.checkCollision(&sBird, fGameDistance);
}

void Scene::render(olc::PixelGameEngine *engine, float fElapsedTime)
{
  fRenders++;
  engine->Clear(olc::BLACK);

  sBackground.render(engine, fElapsedTime);
  sPipes.render(engine, fElapsedTime);
  sGround.render(engine, fElapsedTime);
  sCeiling.render(engine, fElapsedTime);
  
  if (ENABLE_DEBUG_MODE) {
    engine->DrawStringDecal({20, 20}, "Score: " + std::to_string((int)std::round(gScore.score)));
    engine->DrawStringDecal({20, 35}, "Difficulty: " + gEngine->gDifficulty->getMode() + (gSettings.CLAMP_DIFFICULTY ? " [clamped!]" : "") + " [vel: " + std::to_string(gEngine->gDifficulty->gameSpeed()) + ", fac: " + std::to_string(gEngine->gDifficulty->getScale()) + "]");
    engine->DrawStringDecal({20, 50}, "Runs: " + std::to_string((int)gScore.runs));
    if (gScore.score > gScore.topScore)
      engine->DrawStringDecal({20, 65}, "New top score!");
    else if (gScore.topScore > 0 && gScore.runs > 0)
      engine->DrawStringDecal({20, 65}, "Top score: " + std::to_string((int)std::round(gScore.topScore)));

    engine->DrawStringDecal({20, 100}, "Pipe gap: " + std::to_string((int)std::round(gSettings.P_GAP)));
  }

  // Render last z index
  sBird.render(engine, fElapsedTime);

  if (gEngine->gState == GameState::PAUSED) {
    auto c_time = std::chrono::system_clock::now();
    float time = c_time.time_since_epoch().count() / 1000;

    uint8_t alpha = (int) std::round(sine_between(gEngine->getRunTime(), 2, 0.25, 0.85) * 255) & 0xFF;
    float offX = GAME_WIDTH / 2 - sPaused->width / 2;
    float offY = GAME_HEIGHT / 2 - sPaused->height / 2;
    engine->DrawDecal({offX, offY}, dPaused, {1.0f, 1.0f}, {255,255,255,alpha});
  }
}

void Scene::resetScene()
{
  sBird.reset();
  sBackground.reset();
  sGround.reset();
  sCeiling.reset();
  sPipes.reset();
  fGameDistance = 0.0f;
}

void Scene::setGameState(GameState state)
{
  switch (state)
  {
  case GameState::PAUSED:
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
    gSoundMan.PlayDeath();
    sBird.setDead();
    sBackground.setIdle();
    sGround.setIdle();
    sCeiling.setIdle();
    sPipes.setIdle();
    break;
  }
}

void Scene::jump()
{
  gSoundMan.PlayFlap();
  sBird.flapped();
}

void Scene::score()
{
  gSoundMan.PlayScore();
}

Scene::Scene() {}

Scene::Scene(GameEngine *engine)
{
  gEngine = engine;
}

Scene::~Scene()
{
  gSoundMan.Cleanup();
}

void Scene::resumeGame() {
  gSoundMan.ResumeGame();
}

void Scene::pauseGame() {
  gSoundMan.PauseGame();
}