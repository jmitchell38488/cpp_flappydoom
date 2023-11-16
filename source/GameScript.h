#pragma once

using namespace std;
#include <list>
#include "Score.h"

class GameEngine;

class gCommand {
  public:
    gCommand() {}
    virtual ~gCommand(){}

    bool bStarted = false;
    bool bCompleted = false;

    virtual void start() {}
    virtual void update(float fElapsedTime) {}
};

class GameScriptProcessor {
  public:
    GameScriptProcessor() {}

  public:
    void addCommand(gCommand *cmd);
    void processCommands(float fElapsedTime);
    void completeCommand();

  private:
    list<gCommand*> m_listCommands;
};

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