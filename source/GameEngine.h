#pragma once

#define OLC_SOUNDWAVE
#define OLC_PGEX_MINIAUDIO
#define OLC_IMAGE_STB
#define OLC_PGE_APPLICATION
#define OLC_PGEX_FONT

#include "lib/miniaudio.h"
#include "lib/olcPGEX_MiniAudio.h"
#include "lib/olcPixelGameEngine.h"
#include "lib/olcPGEX_Font.h"

#include "definitions.h"
#include "scene/Bird.h"
#include "scene/Fire.h"
#include "scene/Background.h"
#include "scene/Ground.h"
#include "scene/Ceiling.h"
#include "scene/Pipes.h"
#include "scene/Pipe.h"
#include "scene/SplashBird.h"
#include "GameState.h"
#include "GameDifficulty.h"
#include "GameScript.h"
#include "Settings.h"
#include "Score.h"
#include "Util.h"

#include <list>
#include <chrono>
#include <ctime>

enum class GameScreenState {
  SPLASHSCREEN,
  MAINSCREEN,
  DIFFICULTYSELECT,
  HIGHSCORES,
  GAME,
  RESUME
};

class GameSoundManager
{
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
  void adjustVol(int sound, float vol = 1.0f)
  {
    if (vol != 1.0f)
    {
      float nVol = std::fmax(0.0f, std::fmin(1.0f, vol));
      ma.SetVolume(sound, nVol);
    }
  }

public:
  void loadResources()
  {
    aBgMusic = ma.LoadSound("./assets/sound/theme.mp3");
    aBirdFlap = ma.LoadSound("./assets/sound/wing2.wav");
    aBirdDeath = ma.LoadSound("./assets/sound/hit2.wav");
    aBirdScore = ma.LoadSound("./assets/sound/point2.wav");
  }

  void startGame()
  {
    if (!ma.IsPlaying(aBgMusic)) {
      ma.SetVolume(aBgMusic, aVol);
      ma.Play(aBgMusic, true);
    }
  }

  void stopGame() {
    ma.Stop(aBgMusic);
  }

  void setGlobalVolume(float vol)
  {
    aVol = vol;
    ma.SetVolume(aBgMusic, aVol);
    ma.SetVolume(aBirdFlap, aVol);
    ma.SetVolume(aBirdDeath, aVol);
    ma.SetVolume(aBirdScore, aVol);
  }

  void playFlap(float vol = 1.0f)
  {
    adjustVol(aBirdFlap, vol);

    if (!ma.IsPlaying(aBirdFlap))
      ma.Stop(aBirdFlap);
    ma.Play(aBirdFlap);
  }

  void playDeath(float vol = 1.0f)
  {
    adjustVol(aBirdDeath, vol);
    if (!ma.IsPlaying(aBirdDeath))
      ma.Stop(aBirdDeath);
    ma.Play(aBirdDeath);
  }

  void playScore(float vol = 1.0f)
  {
    adjustVol(aBirdScore, vol);
    if (!ma.IsPlaying(aBirdScore))
      ma.Stop(aBirdScore);
    ma.Play(aBirdScore);
  }

  void pauseGame()
  {
    bBgMusicPlaying = ma.IsPlaying(aBgMusic);
    bBirdFlapPlaying = ma.IsPlaying(aBirdFlap);
    bBirdDeathPlaying = ma.IsPlaying(aBirdDeath);
    bBirdScorePlaying = ma.IsPlaying(aBirdScore);

    ma.Toggle(aBgMusic);
    if (bBirdFlapPlaying)
      ma.Toggle(aBirdFlap);
    if (bBirdDeathPlaying)
      ma.Toggle(aBirdDeath);
    if (bBirdScorePlaying)
      ma.Toggle(aBirdScore);
  }

  void resumeGame()
  {
    ma.Toggle(aBgMusic);
    if (bBirdFlapPlaying)
      ma.Toggle(aBirdFlap);
    if (bBirdDeathPlaying)
      ma.Toggle(aBirdDeath);
    if (bBirdScorePlaying)
      ma.Toggle(aBirdScore);

    bBgMusicPlaying = false;
    bBirdFlapPlaying = false;
    bBirdDeathPlaying = false;
    bBirdScorePlaying = false;
  }

  void cleanup()
  {
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
  olc::Sprite *sPaused;
  olc::Decal *dPaused;

  uint16_t gCurScore;
  uint16_t gTopScore;

  GameEngine *gEngine = nullptr;

  std::unique_ptr<olc::Font> doomFont;

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
  void startGame();
};

class GameEngine;

struct MenuOptions {
  uint8_t iVal;
  std::string sTitle;
  GameScreenState gCur;
  GameScreenState gNext;
  bool bSelected = false;
  const std::function<bool()> fEval;
  olc::vf2d vPos;
  DifficultyMode dMode;
};

class MenuScreen
{
private:
  std::unique_ptr<olc::Font> doomFont24;
  bool bTransLogo = true;
  olc::vf2d vPosition;
  float fTransFrame = 0.0f;
  float fLastFlapT = 0.0f;
  uint8_t fCurMenuIdx = 0;

  GameEngine *gEngine = nullptr;

  olc::Sprite * sLogo = nullptr;
  olc::Sprite * sBg = nullptr;
  olc::Sprite * sTitle = nullptr;
  olc::Decal * dLogo = nullptr;
  olc::Decal * dBg = nullptr;
  olc::Decal * dTitle = nullptr;
  
  GameScore *scoreRef = nullptr;

  SplashBird * sBird = nullptr;
  SplashBird * sBird2 = nullptr;
  std::vector<MenuOptions> vOptions;
  std::vector<MenuOptions> vMenuOptions;

public:
  MenuScreen();
  MenuScreen(GameEngine *engine);
  ~MenuScreen();

private:
  void loadAssets();

public:
  void init();
  void tick(float fElapsedTime);
  void handleInput(float fElapsedTime);
  void render(olc::PixelGameEngine *engine, float fElapsedTime);
  void updateScreen(GameScreenState screen);
};

// enum class MenuDirection {
//   HORZ, VERT
// };

// class GameEngine;

// class GameMenuScreen : MenuScreen {
// protected:
//   std::vector<MenuOptions<void(GameEngine*)>> vOptions;
//   MenuDirection mDir;

// public:
//   GameMenuScreen();
//   GameMenuScreen(GameEngine *engine);
// };

// class MainMenuScreen : GameMenuScreen {
// public:
//   void init();
// };

// class DifficultyMenuScreen : GameMenuScreen {
// public:
//   void init();
// };

// class HighScoreMenuScreen : GameMenuScreen {
// public:
//   void init();
// };


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
  MenuScreen *gMenu;

public:
  float fGameScore = 0.0f;
  bool bPlaying = false;
  GameState gState;
  GameScreenState gScreen;
  GameScreenState gPrevScreen;
  GameDifficulty *gDifficulty = nullptr;
  Scene *gScene = nullptr;
  GameScriptProcessor *gScriptProcessor = nullptr;
  std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;
  GameSoundManager * gSoundMan;

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
  bool isGamePlaying();
  void setPlaying();
  void resumeGame();
  void quit();
};

bool GameEngine::isGamePlaying() {
  return gScreen == GameScreenState::GAME && gState != GameState::GAMEOVER;
}

GameEngine::GameEngine()
{
  // Name your application
  sAppName = "FlappyDOOM";

  gDifficulty = new GameDifficulty();
  gScriptProcessor = new GameScriptProcessor();
  gScene = new Scene(this);
  gMenu = new MenuScreen(this);
  gSoundMan = new GameSoundManager();
  m_tp1 = std::chrono::system_clock::now();
  m_tp2 = std::chrono::system_clock::now();
}

GameEngine::~GameEngine()
{
}

bool GameEngine::initialise()
{
  gMenu->init();
  gScene->initScene();
  gDifficulty->setDifficulty(DifficultyMode::EASY);
  gSoundMan->loadResources();
  if (ENABLE_DEBUG_MODE) setPlaying();
  else {
    gState = GameState::MENU;
    gScreen = GameScreenState::SPLASHSCREEN;
  }
  bPlaying = false;
  return true;
}

void GameEngine::setGameState(GameState state)
{
  gState = state;
  gScene->setGameState(gState);

  switch (state)
  {
  case GameState::MENU:
    bPlaying = false;
    if (gState != GameState::GAMEOVER)
      gScene->setGameState(GameState::IDLE);
    gScreen = GameScreenState::SPLASHSCREEN;
    break;

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
  if (!gSettings.CLAMP_DIFFICULTY && gDifficulty->canChangeDifficulty())
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
  if (gState == GameState::MENU || gScreen == GameScreenState::RESUME) {
    gMenu->handleInput(fElapsedTime);
    return;
  }

  if (GetKey(olc::Key::SPACE).bPressed)
  {
    if (gState == GameState::GAMEOVER)
    {
      if (gScore.score > gScore.topScore)
        gScore.topScore = gScore.score;
      gScore.runs++;

      if (gScore.score > 0)
        gScore.pushScore({gScore.score, gScore.runs, gDifficulty->getMode()});
      
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
    if (gState != GameState::PAUSED)
    {
      gLastState = gState;
      setGameState(GameState::PAUSED);
      gScene->pauseGame();
      gSettings.GAME_PAUSED = true;
    }
    else
    {
      setGameState(gLastState);
      gScene->resumeGame();
      fAccumulatedTime = fElapsedTime;
      gSettings.GAME_PAUSED = false;
    }
  }

  if (GetKey(olc::Key::ESCAPE).bPressed)
  {
    if (ENABLE_DEBUG_MODE) {
      resetGame();
    }

    gMenu->handleInput(fElapsedTime);
  }

  if (GetKey(olc::Key::Q).bPressed)
  {
    if (ENABLE_DEBUG_MODE) {
      quit();
    }
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

    if (gState == GameState::MENU) {
      gMenu->tick(fElapsedTime);
    } else {
      float fGameDistance = gScene->tick(fElapsedTime, gDifficulty);
      gDifficulty->gameUpdate(fGameDistance);
    }
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

  // Always render the game in the background for the RESUME screen
  if (gState != GameState::MENU || gScreen == GameScreenState::RESUME)
    gScene->render(this, fElapsedTime);
  
  if (gState == GameState::MENU || gScreen == GameScreenState::RESUME)
    gMenu->render(this, fElapsedTime);

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

float GameEngine::getRunTime()
{
  m_tp2 = std::chrono::system_clock::now();
  std::chrono::duration<float> elapsedTime = m_tp2 - m_tp1;
  return elapsedTime.count();
}

void GameEngine::setPlaying() {
  gScreen = GameScreenState::GAME;
  if (gState != GameState::GAMEOVER) {
    setGameState(GameState::IDLE);
    gScene->startGame();
  }
}

void GameEngine::resumeGame() {
  gScreen = GameScreenState::GAME;
  if (gState != GameState::GAMEOVER) {
    setGameState(GameState::PLAYING);
  }
}

void GameEngine::quit() {
  // if (gScene != nullptr) gScene->~Scene();
  // if (gMenu != nullptr) gMenu->~MenuScreen();
  // if (gDifficulty != nullptr) gDifficulty->~GameDifficulty();
  // if (gScriptProcessor != nullptr) gScriptProcessor->~GameScriptProcessor();
  exit(0);
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

  sPaused = new olc::Sprite((std::string) "./assets/i-paused.png");
  dPaused = new olc::Decal(sPaused);

  doomFont = std::make_unique<olc::Font>("./assets/doomfont40.png");
}

void Scene::initScene()
{
  loadAssets();
  sBird.setIdle();
  sBackground.setIdle();
  sGround.setIdle();
  sCeiling.setIdle();
  sPipes.setIdle();
}

float Scene::tick(float fElapsedTime, GameDifficulty *difficulty)
{
  if (gEngine->gState == GameState::PLAYING)
  {
    fGameDistance += difficulty->gameSpeed();
    sBackground.update(fElapsedTime);
    sGround.update(fElapsedTime, difficulty->gameSpeed() * 1 + fElapsedTime / 2);
    sCeiling.update(fElapsedTime, difficulty->gameSpeed() * 1 + fElapsedTime / 2);
    sPipes.update(fElapsedTime, difficulty->gameSpeed() * 1 + fElapsedTime / 2, gEngine->gDifficulty->getPipeGap(), gEngine->gDifficulty->getPipeVar());
  }

  sBird.update(fElapsedTime, gEngine->getRunTime(), gEngine->isGamePlaying());

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
  engine->Clear(olc::BLACK);

  sBackground.render(engine, fElapsedTime);
  sPipes.render(engine, fElapsedTime);
  sGround.render(engine, fElapsedTime);
  sCeiling.render(engine, fElapsedTime);

  if (ENABLE_DEBUG_MODE)
  {
    engine->DrawStringDecal({20, 20}, "Score: " + std::to_string((int)std::round(gScore.score)));
    engine->DrawStringDecal({20, 35}, "Difficulty: " + gEngine->gDifficulty->getMode() + (gSettings.CLAMP_DIFFICULTY ? " [clamped!]" : "") + " [vel: " + std::to_string(gEngine->gDifficulty->gameSpeed()) + ", fac: " + std::to_string(gEngine->gDifficulty->getScale()) + "]");
    engine->DrawStringDecal({20, 50}, "Runs: " + std::to_string((int)gScore.runs));
    if (gScore.score > gScore.topScore)
      engine->DrawStringDecal({20, 65}, "New top score!");
    else if (gScore.topScore > 0 && gScore.runs > 0)
      engine->DrawStringDecal({20, 65}, "Top score: " + std::to_string((int)std::round(gScore.topScore)));

    engine->DrawStringDecal({20, 100}, "Pipe gap: " + std::to_string((int)std::round(gSettings.P_GAP)));
  }

  olc::vi2d fSize;
  float offX;
  std::string str = "Score: " + std::to_string((int)std::round(gScore.score));
  string_to_lower(str);

  // Draw score
  doomFont->DrawStringDecal({20, GAME_HEIGHT - 15}, str, {0.3, 0.3});
  if (gScore.score > gScore.topScore)
    str = "New top score!";
  else if (gScore.topScore > 0 && gScore.runs > 0)
    str = "Top score: " + std::to_string((int)std::round(gScore.topScore));
  string_to_lower(str);

  fSize = doomFont->GetTextSizeProp(str);
  offX = GAME_WIDTH / 2 - fSize.x / 2;
  doomFont->DrawStringDecal({offX, GAME_HEIGHT - 15}, str, {0.3, 0.3});

  // Draw difficulty
  str = gEngine->gDifficulty->getMode();
  string_to_lower(str);
  fSize = doomFont->GetTextSizeProp(gEngine->gDifficulty->getMode());
  offX = GAME_WIDTH - fSize.x + 50;
  doomFont->DrawStringDecal({offX, GAME_HEIGHT - 15}, str, {0.3, 0.3});

  // Render last z index
  sBird.render(engine, fElapsedTime);

  if (gEngine->gState == GameState::PAUSED)
  {
    auto c_time = std::chrono::system_clock::now();
    float time = c_time.time_since_epoch().count() / 1000;

    uint8_t alpha = (int)std::round(sine_between(gEngine->getRunTime(), 4, 0.65, 0.85) * 255) & 0xFF;
    float offX = GAME_WIDTH / 2 - sPaused->width / 2;
    float offY = GAME_HEIGHT / 2 - sPaused->height / 2;
    engine->DrawDecal({offX, offY}, dPaused, {1.0f, 1.0f}, {alpha, alpha, alpha, alpha});
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
    gEngine->gSoundMan->playDeath();
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
  gEngine->gSoundMan->playFlap();
  sBird.flapped();
}

void Scene::score()
{
  gEngine->gSoundMan->playScore();
}

Scene::Scene() {}

Scene::Scene(GameEngine *engine)
{
  gEngine = engine;
}

Scene::~Scene()
{
  gEngine->gSoundMan->cleanup();
}

void Scene::resumeGame()
{
  gEngine->gSoundMan->resumeGame();
}

void Scene::pauseGame()
{
  gEngine->gSoundMan->pauseGame();
}

void Scene::startGame() {
  gEngine->gSoundMan->startGame();
}

MenuScreen::MenuScreen() {}

MenuScreen::MenuScreen(GameEngine *engine) {
  gEngine = engine;
}

MenuScreen::~MenuScreen() {}

void MenuScreen::init() {
  loadAssets();
  sBird = new SplashBird(SB_ANIM_DIR::LEFT);
  sBird2 = new SplashBird(SB_ANIM_DIR::RIGHT);
  sBird->init();
  sBird2->init();
  sBird2->setPosition({sBird2->getPosition().x, sBird2->getPosition().y + 100});

  scoreRef = &gScore;

  auto fnHs = [this](){return this->scoreRef->scores.size() > 0;};
  auto fnRs = [this](){return this->gEngine->gPrevScreen == GameScreenState::RESUME;};
  auto fnMn = [this](){return this->gEngine->gPrevScreen == GameScreenState::MAINSCREEN;};

  // Main menu
  vOptions.push_back({1, "New game", GameScreenState::MAINSCREEN, GameScreenState::DIFFICULTYSELECT, true});
  vOptions.push_back({2, "High scores", GameScreenState::MAINSCREEN, GameScreenState::HIGHSCORES, false, fnHs});
  vOptions.push_back({3, "Quit", GameScreenState::MAINSCREEN});

  // Difficulty menu
  vOptions.push_back({1, DifficultyModeToString(DifficultyMode::CHALLENGE), GameScreenState::DIFFICULTYSELECT, GameScreenState::GAME, true});
  vOptions.push_back({2, DifficultyModeToString(DifficultyMode::EASY), GameScreenState::DIFFICULTYSELECT, GameScreenState::GAME});
  vOptions.push_back({3, DifficultyModeToString(DifficultyMode::MEDIUM), GameScreenState::DIFFICULTYSELECT, GameScreenState::GAME});
  vOptions.push_back({4, DifficultyModeToString(DifficultyMode::HARD), GameScreenState::DIFFICULTYSELECT, GameScreenState::GAME});
  vOptions.push_back({5, DifficultyModeToString(DifficultyMode::NIGHTMARE), GameScreenState::DIFFICULTYSELECT, GameScreenState::GAME});
  vOptions.push_back({6, DifficultyModeToString(DifficultyMode::IMPOSSIBLE), GameScreenState::DIFFICULTYSELECT, GameScreenState::GAME});
  vOptions.push_back({7, "Main menu", GameScreenState::DIFFICULTYSELECT, GameScreenState::MAINSCREEN});

  // Resume menu
  vOptions.push_back({1, "Resume game", GameScreenState::RESUME, GameScreenState::GAME, true});
  vOptions.push_back({2, "High scores", GameScreenState::RESUME, GameScreenState::HIGHSCORES, false, fnHs});
  vOptions.push_back({3, "Main menu", GameScreenState::RESUME, GameScreenState::MAINSCREEN});
  vOptions.push_back({4, "Quit", GameScreenState::RESUME});

  // High scores menu
  vOptions.push_back({1, "Back", GameScreenState::HIGHSCORES, GameScreenState::RESUME, true, fnRs});
  vOptions.push_back({1, "Back", GameScreenState::HIGHSCORES, GameScreenState::MAINSCREEN, true, fnMn});
}

void MenuScreen::updateScreen(GameScreenState screen) {
  if (gEngine->gScreen == GameScreenState::RESUME && screen == GameScreenState::MAINSCREEN)
    gEngine->gSoundMan->stopGame();

  gEngine->gPrevScreen = gEngine->gScreen;
  gEngine->gScreen = screen;
  if (gEngine->gState != GameState::GAMEOVER)
    gEngine->setGameState(GameState::IDLE);

  if (vMenuOptions.size() > 0) vMenuOptions.clear();
  if (screen != GameScreenState::GAME) {
    std::copy_if(vOptions.begin(), vOptions.end(), std::back_inserter(vMenuOptions),
                 [screen](const MenuOptions &opt){ 
                    if (opt.fEval != nullptr)
                      return opt.gCur == screen && opt.fEval();
                    else
                      return opt.gCur == screen; 
                  });

    // Set selected menu item
    fCurMenuIdx = 0;
    for (int i = 0; i < vMenuOptions.size(); i++) {
      if (vMenuOptions[i].bSelected) {
        fCurMenuIdx = i;
        break;
      }
    }
    
    if (screen != GameScreenState::RESUME)
      gEngine->gState = GameState::MENU;
  }

  if (screen == GameScreenState::GAME && gEngine->gState == GameState::GAMEOVER) gEngine->resumeGame();
  else if (screen == GameScreenState::GAME) gEngine->setPlaying();
  if (screen == GameScreenState::MAINSCREEN) bTransLogo = false;
  // if (screen == GameScreenState::RESUME) {
  //   gEngine->gState = GameState::MENU;
  // }
  
}

void MenuScreen::tick(float fElapsedTime) {
  float fRt = gEngine->getRunTime();
  
  if (gEngine->gScreen == GameScreenState::SPLASHSCREEN) {
    float fY = GAME_HEIGHT / 2 - sLogo->height / 2;
    float fDuration = FPS * 1.5; // 1.5 seconds
    // Bird movement
    sBird->update(fElapsedTime, fRt);

    if ((int)vPosition.y > (int)fY) {
      float fNormalizedTime = fTransFrame > 0 ? fTransFrame / fDuration : 0;
      float fEasedT = ease_in_ease_out(fNormalizedTime);
      fTransFrame++;
      
      vPosition.y = GAME_HEIGHT - ((GAME_HEIGHT - fY) * fEasedT);
      return;
    }

    // Transition bird from the left
    if (1.25f < fRt) {
      sBird2->update(fElapsedTime, fRt);
      if (!sBird2->hasDied()) {
        auto data = sBird2->getBirdColMask();

        // Bird has collided
        if (data.cx + data.r >= GAME_WIDTH / 2 - sLogo->width / 2) {
          sBird2->died();
          gEngine->gSoundMan->playDeath();
        }
      }
    }

    // Show splash screen for 4s
    if (3.8f < fRt) updateScreen(GameScreenState::MAINSCREEN);
  }

  if (gEngine->gScreen == GameScreenState::MAINSCREEN) {
    
  }

  if (gEngine->gScreen == GameScreenState::DIFFICULTYSELECT) {

  }
}

void MenuScreen::handleInput(float fElapsedTime) {
  if (bTransLogo) return; // Block all input until logo displays
  
  if (gEngine->GetKey(olc::Key::ESCAPE).bPressed) {
    for (int i = 0; i < vMenuOptions.size(); i++) {
      if (vMenuOptions[i].sTitle == "Quit" || vMenuOptions[i].gNext == GameScreenState::MAINSCREEN) {
        vMenuOptions[fCurMenuIdx].bSelected = false;
        vMenuOptions[i].bSelected = true;
        fCurMenuIdx = i;
        if (vMenuOptions[i].sTitle == "Quit")
          break;
      }
    }
    
    if (gEngine->gScreen == GameScreenState::GAME)
      updateScreen(GameScreenState::RESUME);
  }

  // Handle keyboard menu nav
  if (gEngine->gScreen != GameScreenState::SPLASHSCREEN &&
      gEngine->gScreen != GameScreenState::GAME) {
    if (gEngine->GetKey(olc::Key::DOWN).bPressed) {
      vMenuOptions[fCurMenuIdx].bSelected = false;
      if (fCurMenuIdx + 1 < vMenuOptions.size()) fCurMenuIdx++;
      else fCurMenuIdx = 0;
      vMenuOptions[fCurMenuIdx].bSelected = true;
    }

    if (gEngine->GetKey(olc::Key::UP).bPressed) {
      vMenuOptions[fCurMenuIdx].bSelected = false;
      if (fCurMenuIdx - 1 >= 0) fCurMenuIdx--;
      else fCurMenuIdx = vMenuOptions.size() - 1;
      vMenuOptions[fCurMenuIdx].bSelected = true;
    }

    // Handle menu selection
    if (gEngine->GetKey(olc::Key::ENTER).bPressed) {
      if (gEngine->gScreen == GameScreenState::DIFFICULTYSELECT) {
        // Set selected difficulty
        auto dm = StringToDifficultyMode(vMenuOptions[fCurMenuIdx].sTitle);
        if (dm == DifficultyMode::CHALLENGE) {
          gEngine->gDifficulty->setDifficulty(DifficultyMode::EASY);
        }

        gEngine->gDifficulty->setDifficulty(dm);

        // Make sure the difficulty doesn't change until the user returns to the home screen
        if (dm != DifficultyMode::CHALLENGE)
          gEngine->gDifficulty->setDifficulty(DifficultyMode::NOCHANGE);
      }

      if (vMenuOptions[fCurMenuIdx].sTitle == "Quit") {
        gEngine->quit();
      }

      updateScreen(vMenuOptions[fCurMenuIdx].gNext);
    }
  }

  
}

void MenuScreen::render(olc::PixelGameEngine *engine, float fElapsedTime) {
  switch (gEngine->gScreen) {
    case GameScreenState::SPLASHSCREEN:
    case GameScreenState::MAINSCREEN:
      engine->Clear(olc::BLACK);
      break;
    
    default: // do nothing
      break;
  }

  if (gEngine->gScreen == GameScreenState::SPLASHSCREEN) {
    sBird->render(engine, fElapsedTime);
    sBird2->render(engine, fElapsedTime);
    engine->DrawDecal(vPosition, dLogo);
    if (ENABLE_DEBUG_MODE)
      engine->DrawStringDecal({20, 20}, "Time: " + std::to_string(gEngine->getRunTime()));
    return;
  }

  if (gEngine->gScreen != GameScreenState::RESUME || (
    gEngine->gScreen == GameScreenState::HIGHSCORES &&
    gEngine->gPrevScreen != GameScreenState::RESUME
  )) {
    engine->DrawDecal({0,0}, dBg);
  }

  if (gEngine->gScreen == GameScreenState::RESUME) {
    
  }

  float offX = GAME_WIDTH / 2 - sTitle->width / 2;
  engine->DrawDecal({offX, 20}, dTitle);

  float gapY = 35;
  float offY = 20 + sTitle->height + gapY * 1.5;
  olc::Pixel px = olc::WHITE;

  if (gEngine->gScreen == GameScreenState::HIGHSCORES) {
    std::string str = "HIGHSCORES";
    offX = (float)doomFont24->GetTextSize(str).x * 0.75f;
    doomFont24->DrawStringDecal({offX, offY}, str, {0.75f, 0.75f}, px);
    offY += gapY;

    float gapY = 18.5f;
    str = "Score - Difficulty - Run";
    offX = (float)doomFont24->GetTextSize(str).x * 0.35f;
    offX = GAME_WIDTH / 2 - offX / 2;
    doomFont24->DrawStringDecal({offX, offY}, str, {0.35f, 0.35f}, px);
    offY += gapY;

    for (auto score : gScore.highestScores(7)) {
      str = std::to_string((int)score.score) + " - " + score.mode + " - " + std::to_string((int)score.run);
      offX = (float)doomFont24->GetTextSize(str).x * 0.35f;
      offX = GAME_WIDTH / 2 - offX / 2;
      doomFont24->DrawStringDecal({offX, offY}, str, {0.35f, 0.35}, px);
      offY += gapY;
    }
    offY += gapY;
    gapY = 35;
  }

  for (auto opt : vMenuOptions) {
    offX = (float)doomFont24->GetTextSize(opt.sTitle).x * 0.75f;
    offX = GAME_WIDTH / 2 - offX / 2;
    px = olc::WHITE;
    if (opt.bSelected) px = olc::RED;
    doomFont24->DrawStringDecal({offX, offY}, opt.sTitle, {0.75f, 0.75f}, px);
    offY += gapY;
  }
  
}

void MenuScreen::loadAssets() {
  doomFont24 = std::make_unique<olc::Font>("./assets/doomfont40.png");
  sLogo = new olc::Sprite((std::string) "./assets/i-logo.png");
  dLogo = new olc::Decal(sLogo);

  sBg = new olc::Sprite((std::string) "./assets/i-menubg.jpg");
  dBg = new olc::Decal(sBg);

  sTitle = new olc::Sprite((std::string) "./assets/i-title.png");
  dTitle = new olc::Decal(sTitle);

  vPosition = {(float)(GAME_WIDTH / 2 - sLogo->width / 2), (float)GAME_HEIGHT};
}