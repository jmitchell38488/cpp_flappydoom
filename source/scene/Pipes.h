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
  olc::Sprite *sPipes;
  olc::Decal *dPipes;
  float fPipeW = 62.0f;
  float fPipeH = 300.0f;

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
  void init(float offX);
  void update(float fElapsedTime, float gameSpeed, float pipeGap);
  void render(olc::PixelGameEngine *engine, float fElapsedTime);
  void setScrolling();
  void setIdle();
  void reset();
  bool checkCollision(Bird * bird, float fGameDistance);
};

void Pipes::init(float offX)
{
  sPipes = new olc::Sprite((std::string) "./assets/s-pipe.png");
  dPipes = new olc::Decal(sPipes);

  fScoreOffX = offX;
  addPipes();
}

float Pipes::getMaxXOffset() {
  // Get max offset x value
  float maxX = 0.0f;
  for (auto pipe : vPipes) maxX = pipe->getX() > maxX ? pipe->getX() : maxX;
  return maxX;
}

void Pipes::update(float fElapsedTime, float gameSpeed, float pipeGap)
{
  if (state == PipesState::SCROLLING)
  {
    float lp_offy = 0.0f;
    for (auto pipe : vPipes)
    {
      if (pipe->isPast() && pipe->bTraversed)
      {
        pipe->reset(getMaxXOffset() + fPipeStepX, false);
      }
      else
      {
        if (!pipe->isVisible() && lp_offy == pipe->getY() && pipe != vPipes[0]) {
          // pipe->reset(pipe->getX(), false);
        }
        pipe->update(fElapsedTime, gameSpeed, pipeGap);
      }
      lp_offy = pipe->getY();
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

bool Pipes::checkCollision(Bird * bird, float fGameDistance) {
  // Pipes check
  for (auto * pipe : vPipes) {
    // Skip!
    if (!pipe->isVisible()) continue;
    if (!pipe->withinRadius(bird)) continue;

    // Only check next pipe, not all!
    if (pipe->checkCollision(bird, fGameDistance)) return true;
  }

  return false;
}

void Pipes::addPipes()
{
  float startOffX = GAME_WIDTH * 1.15;
  float nextX = startOffX;
  float fLastXOffset = startOffX;
  
  for (int i = 0; i < 5; i++)
  {
    // Pipe(float offX, float offY, olc::Decal *pipes, float sw, float sh, float scale, bool first, float scoreX)
    // vPipes.push_back(new Pipe(nextX, 0, dPipes, PIPE_SW, PIPE_SH, i == 0, fScoreOffX));
    vPipes.push_back(new Pipe(nextX, 0, dPipes, PIPE_SW, PIPE_SH, PIPE_SF, i == 0, fScoreOffX));
    nextX = fLastXOffset + fPipeStepX;
    fLastXOffset = nextX;
  }
}