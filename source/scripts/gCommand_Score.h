#ifndef GAME_SCRIPT_COMMAND_SCORE
#define GAME_SCRIPT_COMMAND_SCORE

#pragma once
#include "../GameScript.h"

class gCommand_Score : public gCommand {
  public:
    gCommand_Score();
    void start() override;
};

gCommand_Score::gCommand_Score() {}
void gCommand_Score::start() {
  bStarted = true;
  bCompleted = true;
}

#endif // GAME_SCRIPT_COMMAND_SCORE