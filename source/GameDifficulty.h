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

  // Minimum game distance travelled for difficulty level
  float fGameDistance;

  // Increment of game score per pipe
  float fGameScore;

  bool bDiffChange = true;

public:
  void setDifficulty(DifficultyMode mode)
  {
    switch (mode)
    {
    case DifficultyMode::EASY:
      bDiffChange = true;
      fGameSpeed = 3.0f;
      fGameDistance = GAME_WIDTH * 2;
      fGameScore = 1.0f;
      mPrevMode = mMode;
      mMode = mode;
      sMode = "I'm too young to die";
      break;

    case DifficultyMode::MEDIUM:
      bDiffChange = true;
      fGameSpeed = 3.75f;
      fGameDistance = GAME_WIDTH * 4;
      fGameScore = 1.0f;
      mPrevMode = mMode;
      mMode = mode;
      sMode = "Hey, not too rough";
      break;

    case DifficultyMode::HARD:
      bDiffChange = true;
      fGameSpeed = 4.5f;
      fGameDistance = GAME_WIDTH * 6;
      fGameScore = 1.5f;
      mPrevMode = mMode;
      mMode = mode;
      sMode = "Hurt me plenty";
      break;

    case DifficultyMode::NIGHTMARE:
      bDiffChange = true;
      fGameSpeed = 5.75f;
      fGameDistance = GAME_WIDTH * 8;
      fGameScore = 4.0f;
      mPrevMode = mMode;
      mMode = mode;
      sMode = "Nightmare";
      break;

    case DifficultyMode::IMPOSSIBLE:
      bDiffChange = true;
      fGameSpeed = 8.0f;
      fGameDistance = GAME_WIDTH * 10;
      fGameScore = 10.0f;
      mPrevMode = mMode;
      mMode = mode;
      sMode = "Watch me die!";
      break;

    case DifficultyMode::NOCHANGE:
      bDiffChange = false;
      mPrevMode = mMode;
      mMode = DifficultyMode::NOCHANGE;
      break;
    }
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
    return fGameSpeed;
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
  
};

GameDifficulty::GameDifficulty()
{
  this->setDifficulty(DifficultyMode::EASY);
}

GameDifficulty::~GameDifficulty() {}