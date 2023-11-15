#pragma once
#include "../GameScript.h"

class GameEngine;

class gCommand_Score : public gCommand {
  public:
    gCommand_Score();
    void start() override;
};

gCommand_Score::gCommand_Score() {}
void gCommand_Score::start() {
  bStarted = true;
  gEngine->addScore();
  bCompleted = true;
}