#pragma once

using namespace std;
#include <list>

class GameEngine;

class gCommand {
  public:
    gCommand() {}
    virtual ~gCommand(){}

    bool bStarted = false;
    bool bCompleted = false;

    virtual void start() {}
    virtual void update(float fElapsedTime) {}

    static GameEngine * gEngine;
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

class gCommand_Score : public gCommand {
  public:
    gCommand_Score();
    void start() override;
};

class gCommand_Difficulty : public gCommand {
  private:
    DifficultyMode gMode;

  public:
    gCommand_Difficulty(DifficultyMode mode);
    void start() override;
};