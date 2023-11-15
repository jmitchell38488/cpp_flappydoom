#pragma once
#include <cstdlib>
#include "Scene.h"

class Pipe
{

private:
  float fScale = 1.0f;
  float fWidth = 1.0f;
  float fHeight = 1.0f;
  float fScoreOffsetX = 0.0f;

  float fOffX = 0.0f;
  float fOffY = 0.0f;
  float fVertGap = GAME_HEIGHT / 3;
  float fVertYOffset = 0.0f;

  float fMidY = GAME_HEIGHT / 2;
  float fGapY = 0.0f;

  olc::Decal *dPipeTop;
  olc::Decal *dPipeBot;

public:
  bool bTraversed = false;
  bool bFirst = false;

private:
  void setOffY(bool first);

public:
  Pipe() {}
  Pipe(float offX, float offY, olc::Decal *pipeTop, olc::Decal *pipeBot, bool first, float scoreX);
  void setOffset(float offX, float offY, float scoreX);
  bool checkCollision(Bird bird);
  bool isVisible();
  bool isPast();
  void render(olc::PixelGameEngine *engine, float fElapsedTime);
  void reset(float offX, bool first);
  void update(float fElapsedTime, float gameSpeed);
  float getX();
};

Pipe::Pipe(float offX, float offY, olc::Decal *pipeTop, olc::Decal *pipeBot, bool first, float scoreX)
{
  fOffX = offX;
  fOffY = offY;
  dPipeTop = pipeTop;
  dPipeBot = pipeBot;
  fWidth = (float)pipeTop->sprite->width;
  fHeight = (float)pipeTop->sprite->height;
  fScale = 0.35f;
  bFirst = first;
  fScoreOffsetX = scoreX;

  setOffY(bFirst);
}

void Pipe::setOffset(float offX, float offY, float scoreX)
{
  fOffX = offX;
  fOffY = offY;
  fScoreOffsetX = scoreX;
}

bool Pipe::checkCollision(Bird bird)
{
  return false;
}

bool Pipe::isVisible()
{
  return !isPast() && fOffX < GAME_WIDTH;
  // return fOffX > offX && fOffX < offX + sw;
}

bool Pipe::isPast()
{
  return (fOffX + fWidth * fScale) + 1 < 0;
}

void Pipe::render(olc::PixelGameEngine *engine, float fElapsedTime)
{
  float dY = ((fHeight * fScale * 2) - GAME_HEIGHT) / 2;

  // // Draw top
  float offY = 0 - dY - fVertGap / 2 + fVertYOffset / 2;
  olc::vf2d top = {fOffX, offY};
  engine->DrawDecal(top, dPipeTop, {fScale, fScale});

  // Draw bottom
  float offBy = dPipeTop->sprite->height * fScale + offY + fVertGap;
  // fMidY + fVertGap / 2 + fVertYOffset
  olc::vf2d bot = {fOffX, offBy};
  engine->DrawDecal(bot, dPipeBot, {fScale, fScale});
}

void Pipe::update(float fElapsedTime, float gameSpeed)
{
  fOffX -= gameSpeed;

  // Check if passed
  if (!bTraversed && fOffX + fWidth * fScale < fScoreOffsetX)
  {
    bTraversed = true;
  }
}

void Pipe::reset(float offX, bool first)
{
  fOffX = offX;
  fOffY = 0.0f;
  bTraversed = false;
  bFirst = first;
  fVertYOffset = 0;
  setOffY(first);
}

void Pipe::setOffY(bool first)
{
  if (first)
  {
    fVertYOffset = 0;
    return;
  }

  float inc = std::floor(fVertGap * 1.25 / 2);
  int offset = (std::rand() % 4) - 2;
  fVertYOffset = inc * offset;

  // Taller than one pipe, readjust
  if (std::abs(fVertYOffset) > dPipeTop->sprite->height * fScale)
  {
    fVertYOffset *= 0.85f;
  }
}

float Pipe::getX()
{
  return fOffX;
}