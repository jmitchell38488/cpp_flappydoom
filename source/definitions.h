#pragma once

// #define DEBUG_MODE false
// #define FULL_SCREEN false

struct GameSettings {
  bool DEBUG_MODE;
  bool VSYNC_MODE;
  bool FS_MODE;

  public:
  GameSettings() {
    DEBUG_MODE = false;
    VSYNC_MODE = false;
    FS_MODE = false;
  }
};

GameSettings gSettings;