#pragma once

#include "definitions.h"
#include "Settings.h"

enum class DifficultyMode
{
  EASY,
  MEDIUM,
  HARD,
  NIGHTMARE,
  IMPOSSIBLE,
  NOCHANGE
};

class GameDifficulty
{

public:
  GameDifficulty();
  ~GameDifficulty();

private:
  DifficultyMode mMode;
  DifficultyMode mPrevMode;

  std::string sMode;

  // Scroll speed of the game
  float fGameSpeed;
  float fGameSpeed_sf;

  // Minimum game distance travelled for difficulty level
  float fGameDistance;

  // Increment of game score per pipe
  float fGameScore;

  bool bDiffChange = true;

  float fScaleFactor = 1.0f;
  float fScaleFactorStep = 0.15f;
  float fPipeGap = PIPE_GAP;

public:
  void setDifficulty(DifficultyMode mode)
  {
    switch (mode)
    {
    case DifficultyMode::EASY:
      bDiffChange = true;
      fGameSpeed = 3.0f;
      fGameDistance = GAME_WIDTH * 3;
      fGameScore = 1.0f;
      mPrevMode = mMode;
      mMode = mode;
      sMode = "I'm too young to die";
      fPipeGap = PIPE_GAP;
      break;

    case DifficultyMode::MEDIUM:
      bDiffChange = true;
      fGameSpeed = 3.80f;
      fGameDistance = GAME_WIDTH * 6;
      fGameScore = 1.0f;
      mPrevMode = mMode;
      mMode = mode;
      sMode = "Hey, not too rough";
      fPipeGap = PIPE_GAP;
      break;

    case DifficultyMode::HARD:
      bDiffChange = true;
      fGameSpeed = 4.80f;
      fGameDistance = GAME_WIDTH * 10;
      fGameScore = 1.5f;
      mPrevMode = mMode;
      mMode = mode;
      sMode = "Hurt me plenty";
      fPipeGap = PIPE_GAP * 1.025;
      break;

    case DifficultyMode::NIGHTMARE:
      bDiffChange = true;
      fGameSpeed = 6.0f;
      fGameDistance = GAME_WIDTH * 15;
      fGameScore = 2.0f;
      mPrevMode = mMode;
      mMode = mode;
      sMode = "Nightmare";
      fPipeGap = PIPE_GAP * 1.05;
      break;

    case DifficultyMode::IMPOSSIBLE:
      bDiffChange = true;
      fGameSpeed = 9.0f;
      fGameDistance = GAME_WIDTH * 20;
      fGameScore = 5.0f;
      mPrevMode = mMode;
      mMode = mode;
      sMode = "Watch me die!";
      fPipeGap = PIPE_GAP * 1.075;
      break;

    case DifficultyMode::NOCHANGE:
      bDiffChange = false;
      mPrevMode = mMode;
      mMode = DifficultyMode::NOCHANGE;
      break;
    }

    fGameSpeed_sf = fGameSpeed;
  }

  void gameUpdate(float distanceX)
  {
    // No scaling difficulty change allowed
    if (!bDiffChange)
      return;

    // Highest difficulty
    if (mMode == DifficultyMode::IMPOSSIBLE || mMode == DifficultyMode::NOCHANGE)
      return;

    if (distanceX > fGameDistance)
    {
      switch (mMode)
      {
      case DifficultyMode::EASY:
        if (mMode != DifficultyMode::EASY)
          break;
        setDifficulty(DifficultyMode::MEDIUM);
        break;

      case DifficultyMode::MEDIUM:
        if (mMode != DifficultyMode::MEDIUM)
          break;
        setDifficulty(DifficultyMode::HARD);
        break;

      case DifficultyMode::HARD:
        if (mMode != DifficultyMode::HARD)
          break;
        setDifficulty(DifficultyMode::NIGHTMARE);
        break;

      case DifficultyMode::NIGHTMARE:
        if (mMode != DifficultyMode::NIGHTMARE)
          break;
        setDifficulty(DifficultyMode::IMPOSSIBLE);
        break;
      }
    }
  }

  float gameSpeed()
  {
    return fGameSpeed_sf;
  }

  std::string getMode()
  {
    return sMode;
  }

  float gameScore()
  {
    return fGameScore;
  }

  void decDifficulty() {
    DifficultyMode mode = mMode == DifficultyMode::NOCHANGE ? mPrevMode : mMode;
    switch (mode)
    {
    case DifficultyMode::MEDIUM:
      setDifficulty(DifficultyMode::EASY);
      bDiffChange = false;
      break;

    case DifficultyMode::HARD:
      setDifficulty(DifficultyMode::MEDIUM);
      bDiffChange = false;
      break;

    case DifficultyMode::NIGHTMARE:
      setDifficulty(DifficultyMode::HARD);
      bDiffChange = false;
      break;

    case DifficultyMode::IMPOSSIBLE:
      setDifficulty(DifficultyMode::NIGHTMARE);
      bDiffChange = false;
      break;
      
    default:
      break;
    }
  }

  void incDifficulty() {
    DifficultyMode mode = mMode == DifficultyMode::NOCHANGE ? mPrevMode : mMode;
    switch (mode)
    {
    case DifficultyMode::EASY:
      setDifficulty(DifficultyMode::MEDIUM);
      bDiffChange = false;
      break;

    case DifficultyMode::MEDIUM:
      setDifficulty(DifficultyMode::HARD);
      bDiffChange = false;
      break;

    case DifficultyMode::HARD:
      setDifficulty(DifficultyMode::NIGHTMARE);
      bDiffChange = false;
      break;

    case DifficultyMode::NIGHTMARE:
      setDifficulty(DifficultyMode::IMPOSSIBLE);
      bDiffChange = false;
      break;
      
    default:
      break;
    }
  }

  void toggleNoChange() {
    if (mMode != DifficultyMode::NOCHANGE) {
      setDifficulty(DifficultyMode::NOCHANGE);
      bDiffChange = false;
    } else {
      setDifficulty(mPrevMode);
      bDiffChange = true;
    }
  }

  void incScale() {
    fScaleFactor += fScaleFactorStep;
    fGameSpeed_sf = fGameSpeed * fScaleFactor;
  }

  void decScale() {
    if (fScaleFactor > 0) fScaleFactor -= fScaleFactorStep;
    if (fScaleFactor < 0) fScaleFactor = 0;
    fGameSpeed_sf = fGameSpeed * fScaleFactor;
  }

  float getScale() {
    return fScaleFactor;
  }

  float getPipeGap() {
    return fPipeGap;
  }
};

GameDifficulty::GameDifficulty()
{
  this->setDifficulty(DifficultyMode::EASY);
}

GameDifficulty::~GameDifficulty() {}