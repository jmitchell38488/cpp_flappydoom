#pragma once

// #define DEBUG_MODE false
// #define FULL_SCREEN false

#include "Settings.h"

struct GameSettings {
  bool DEBUG_MODE;
  bool VSYNC_MODE;
  bool FS_MODE;

  public:
  GameSettings() {
    DEBUG_MODE = false;
    VSYNC_MODE = true;
    FS_MODE = false;
  }
};

GameSettings gSettings;