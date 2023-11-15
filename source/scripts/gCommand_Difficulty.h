#pragma once
#include "../GameScript.h"
#include "../GameDifficulty.h"

class gCommand_Difficulty : public gCommand {
  private:
    DifficultyMode gMode;

  public:
    gCommand_Difficulty(DifficultyMode mode);
    void start() override;
};

gCommand_Difficulty::gCommand_Difficulty(DifficultyMode mode) {
  gMode = mode;
}

void gCommand_Difficulty::start() {
  bStarted = true;
  gEngine->setDifficultyMode(gMode);
  bCompleted = true;
}