#pragma once
#include <cstdlib>

class Pipe
{

private:
  float fScale = 1.0f;
  float fWidth = 1.0f;
  float fHeight = 1.0f;

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
  Pipe(float offX, float offY, olc::Decal *pipeTop, olc::Decal *pipeBot, bool first);
  void setOffset(float offX, float offY);
  bool checkCollision(Bird bird);
  bool isVisible();
  bool isPast();
  void render(olc::PixelGameEngine *engine, float fElapsedTime);
  void reset(float offX, bool first);
  void update(float fElapsedTime);
  float getX();
};

Pipe::Pipe(float offX, float offY, olc::Decal *pipeTop, olc::Decal *pipeBot, bool first)
{
  fOffX = offX;
  fOffY = offY;
  dPipeTop = pipeTop;
  dPipeBot = pipeBot;
  fWidth = (float)pipeTop->sprite->width;
  fHeight = (float)pipeTop->sprite->height;
  fScale = 0.35f;
  bFirst = first;

  setOffY(bFirst);
}

void Pipe::setOffset(float offX, float offY)
{
  fOffX = offX;
  fOffY = offY;
}

bool Pipe::checkCollision(Bird bird) { 
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
  olc::vf2d top = {fOffX - fWidth / 2, offY};
  engine->DrawDecal(top, dPipeTop, {fScale, fScale});

  // Draw bottom
  float offBy = dPipeTop->sprite->height * fScale + offY + fVertGap;
  // fMidY + fVertGap / 2 + fVertYOffset
  olc::vf2d bot = {fOffX - fWidth / 2, offBy};
  engine->DrawDecal(bot, dPipeBot, {fScale, fScale});
}

void Pipe::update(float fElapsedTime) {
  fOffX -= SCROLL_SPEED;
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

void Pipe::setOffY(bool first) {
  if (first) {
    fVertYOffset = 0;
    return;
  }

  float inc = std::floor(fVertGap / 4);
  int offset = (std::rand() % 8) - 4;
  // float offset = std::round((std::rand() * 1.0 / RAND_MAX * 8) - 4);
  fVertYOffset = inc * offset;
}

float Pipe::getX() {
  return fOffX;
}