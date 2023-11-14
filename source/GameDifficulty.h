#pragma once

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
      fGameSpeed = 3.0f * ANIM_TICK;
      fGameDistance = GAME_WIDTH * 2;
      fGameScore = 1.0f;
      mMode = mode;
      sMode = "I'm too young to die";
      break;

    case DifficultyMode::MEDIUM:
      bDiffChange = true;
      fGameSpeed = 3.75f * ANIM_TICK;
      fGameDistance = GAME_WIDTH * 4;
      fGameScore = 1.0f;
      mMode = mode;
      sMode = "Hey, not too rough";
      break;

    case DifficultyMode::HARD:
      bDiffChange = true;
      fGameSpeed = 4.5f * ANIM_TICK;
      fGameDistance = GAME_WIDTH * 6;
      fGameScore = 1.5f;
      mMode = mode;
      sMode = "Hurt me plenty";
      break;

    case DifficultyMode::NIGHTMARE:
      bDiffChange = true;
      fGameSpeed = 5.75f * ANIM_TICK;
      fGameDistance = GAME_WIDTH * 8;
      fGameScore = 4.0f;
      mMode = mode;
      sMode = "Nightmare";
      break;

    case DifficultyMode::IMPOSSIBLE:
      bDiffChange = true;
      fGameSpeed = 8.0f * ANIM_TICK;
      fGameDistance = GAME_WIDTH * 10;
      fGameScore = 10.0f;
      mMode = mode;
      sMode = "Watch me die!";
      break;

    case DifficultyMode::NOCHANGE:
      bDiffChange = false;
      mMode = DifficultyMode::EASY;
      break;
    }
  }

  void gameUpdate(float distanceX)
  {
    // No scaling difficulty change allowed
    if (!bDiffChange)
      return;

    // Highest difficulty
    if (mMode == DifficultyMode::IMPOSSIBLE)
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
};

GameDifficulty::GameDifficulty()
{
  this->setDifficulty(DifficultyMode::EASY);
}

GameDifficulty::~GameDifficulty() {}