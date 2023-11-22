#ifndef GAME_DEFINITIONS
#define GAME_DEFINITIONS

#pragma once

// #define DEBUG_MODE false
// #define FULL_SCREEN false

#include "Settings.h"

#include <chrono>
#include <ctime>

struct GameSettings {
  bool DEBUG_MODE;
  bool VSYNC_MODE;
  bool FS_MODE;
  bool CLAMP_DIFFICULTY;
  bool GAME_PAUSED;
  float P_GAP;

  public:
  GameSettings() {
    DEBUG_MODE = false;
    VSYNC_MODE = true;
    FS_MODE = false;
    CLAMP_DIFFICULTY = false;
    GAME_PAUSED = false;
    P_GAP = PIPE_GAP;
  }
};

#endif // GAME_DEFINITIONS