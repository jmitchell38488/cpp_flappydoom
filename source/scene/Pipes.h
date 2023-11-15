#pragma once
#include "Pipe.h"

enum class PipesState
{
  IDLE,
  SCROLLING
};

class Pipes
{
private:
  std::vector<Pipe *> vPipes;
  olc::Sprite *sPipeTop;
  olc::Sprite *sPipeBot;
  olc::Decal *dPipeTop;
  olc::Decal *dPipeBot;

  float fScrollRate = 0.0f;
  float fScoreOffX = 0.0f;
  float fPipeStepX = GAME_WIDTH / 3;

  PipesState state = PipesState::IDLE;

private:
  float getMaxXOffset();

protected:
  void addPipes();

public:
  Pipes() {}
  void init();
  void update(float fElapsedTime, float gameSpeed);
  void render(olc::PixelGameEngine *engine, float fElapsedTime);
  void setScrolling();
  void setIdle();
  void reset();
  bool checkCollision(Bird bird);
  void setScoreOffX(float offX);
};

void Pipes::init()
{
  sPipeTop = new olc::Sprite((std::string) "./assets/images/top_pipe.png");
  dPipeTop = new olc::Decal(sPipeTop);

  sPipeBot = new olc::Sprite((std::string) "./assets/images/bot_pipe.png");
  dPipeBot = new olc::Decal(sPipeBot);

  addPipes();
}

float Pipes::getMaxXOffset() {
  // Get max offset x value
  float maxX = 0.0f;
  for (auto pipe : vPipes) maxX = pipe->getX() > maxX ? pipe->getX() : maxX;
  return maxX;
}


void Pipes::update(float fElapsedTime, float gameSpeed)
{
  if (state == PipesState::SCROLLING)
  {
    for (auto pipe : vPipes)
    {
      if (pipe->isPast() && pipe->bTraversed)
      {
        pipe->reset(getMaxXOffset() + fPipeStepX, false);
      }
      else
      {
        pipe->update(fElapsedTime, gameSpeed);
      }
    }
  }
}
void Pipes::render(olc::PixelGameEngine *engine, float fElapsedTime)
{
  for (auto pipe : vPipes)
  {
    if (pipe->isPast())
      continue;
    if (!pipe->isVisible())
      continue;

    pipe->render(engine, fElapsedTime);
  }
}
void Pipes::setScrolling()
{
  state = PipesState::SCROLLING;
}

void Pipes::setIdle()
{
  state = PipesState::IDLE;
}

void Pipes::reset()
{
  state = PipesState::IDLE;
  float startOffX = GAME_WIDTH * 1.15;
  float nextX = startOffX;
  float fLastXOffset = startOffX;

  for (auto pipe : vPipes) {
    pipe->reset(nextX, pipe == vPipes.front());
    nextX = fLastXOffset + fPipeStepX;
    fLastXOffset = nextX;
  }
}

bool Pipes::checkCollision(Bird bird) { return false; }

void Pipes::addPipes()
{
  float startOffX = GAME_WIDTH * 1.15;
  float nextX = startOffX;
  float fLastXOffset = startOffX;
  
  for (int i = 0; i < 5; i++)
  {
    vPipes.push_back(new Pipe(nextX, 0, dPipeTop, dPipeBot, i == 0, fScoreOffX));
    nextX = fLastXOffset + fPipeStepX;
    fLastXOffset = nextX;
  }
}

void Pipes::setScoreOffX(float offX) {
  fScoreOffX = offX;
}