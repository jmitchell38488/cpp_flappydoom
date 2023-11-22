#pragma once

#include "definitions.h"
#include "Settings.h"
#include "DifficultyMode.h"

using namespace std;

std::string DifficultyModeToString(DifficultyMode mode)
{
  switch (mode)
  {
  case DifficultyMode::EASY:
    return "I'm too young to die";
  case DifficultyMode::MEDIUM:
    return "Hey, not too rough";
  case DifficultyMode::HARD:
    return "Hurt me plenty";
  case DifficultyMode::NIGHTMARE:
    return "Nightmare";
  case DifficultyMode::IMPOSSIBLE:
    return "Watch me die!";
  case DifficultyMode::CHALLENGE:
    return "Challenge me!";
  default:
    break;
  }
}

DifficultyMode StringToDifficultyMode(std::string str)
{
  if (str == "I'm too young to die") return DifficultyMode::EASY;
  if (str == "Hey, not too rough") return DifficultyMode::MEDIUM;
  if (str == "Hurt me plenty") return DifficultyMode::HARD;
  if (str == "Nightmare") return DifficultyMode::NIGHTMARE;
  if (str == "Watch me die!") return DifficultyMode::IMPOSSIBLE;
  if (str == "Challenge me!") return DifficultyMode::CHALLENGE;
}

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
  uint8_t fPipeVar = 3;

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
      fPipeGap = PIPE_GAP;
      fPipeVar = 3;
      break;

    case DifficultyMode::MEDIUM:
      bDiffChange = true;
      fGameSpeed = 3.80f;
      fGameDistance = GAME_WIDTH * 6;
      fGameScore = 1.0f;
      mPrevMode = mMode;
      mMode = mode;
      fPipeGap = PIPE_GAP;
      fPipeVar = 4;
      break;

    case DifficultyMode::HARD:
      bDiffChange = true;
      fGameSpeed = 4.80f;
      fGameDistance = GAME_WIDTH * 10;
      fGameScore = 1.5f;
      mPrevMode = mMode;
      mMode = mode;
      fPipeGap = PIPE_GAP * 1.015;
      fPipeVar = 5;
      break;

    case DifficultyMode::NIGHTMARE:
      bDiffChange = true;
      fGameSpeed = 6.0f;
      fGameDistance = GAME_WIDTH * 15;
      fGameScore = 3.0f;
      mPrevMode = mMode;
      mMode = mode;
      fPipeGap = PIPE_GAP * 1.025;
      fPipeVar = 6;
      break;

    case DifficultyMode::IMPOSSIBLE:
      bDiffChange = false;
      fGameSpeed = 9.0f;
      fGameDistance = GAME_WIDTH * 20;
      fGameScore = 3.5f;
      mPrevMode = mMode;
      mMode = mode;
      fPipeGap = PIPE_GAP * 1.035;
      fPipeVar = 7;
      break;

    case DifficultyMode::NOCHANGE:
      bDiffChange = false;
      mPrevMode = mMode;
      break;

    case DifficultyMode::CHALLENGE:
      bDiffChange = true;
      mPrevMode = mMode;
    }

    fGameSpeed_sf = fGameSpeed;
    sMode = DifficultyModeToString(mMode);
  }

  void gameUpdate(float distanceX)
  {
    // No scaling difficulty change allowed
    if (!bDiffChange)
      return;

    // Highest difficulty
    if (!bDiffChange)
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
    DifficultyMode mode = !bDiffChange ? mPrevMode : mMode;
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
    DifficultyMode mode = !bDiffChange ? mPrevMode : mMode;
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
    if (bDiffChange) {
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

  uint8_t getPipeVar() {
    return fPipeVar;
  }

  bool canChangeDifficulty() {
    return bDiffChange;
  }
};

GameDifficulty::GameDifficulty()
{
  this->setDifficulty(DifficultyMode::EASY);
}

GameDifficulty::~GameDifficulty() {}