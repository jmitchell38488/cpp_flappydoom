#include "GameScript.h"

enum class DifficultyMode;
class GameEngine;

void GameScriptProcessor::addCommand(gCommand *cmd) {
  m_listCommands.push_back(cmd);
}

void GameScriptProcessor::processCommands(float fElapsedTime) {
  if (m_listCommands.empty()) return;

  if (!m_listCommands.front()->bCompleted) {
    if (!m_listCommands.front()->bStarted) {
      m_listCommands.front()->start();
    } else {
      m_listCommands.front()->update(fElapsedTime);
    }
  } else {
    delete m_listCommands.front();
    m_listCommands.pop_front();
  }
}

void GameScriptProcessor::completeCommand() {
  if (!m_listCommands.empty()) {
    m_listCommands.front()->bCompleted = true;
  }
}

gCommand_Score::gCommand_Score() {}
void gCommand_Score::start() {
  bStarted = true;
  gEngine->addScore();
  bCompleted = true;
}

gCommand_Difficulty::gCommand_Difficulty(DifficultyMode mode) {
  gMode = mode;
}

void gCommand_Difficulty::start() {
  bStarted = true;
  gEngine->setDifficultyMode(gMode);
  bCompleted = true;
}