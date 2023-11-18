#pragma once
#include <cstdlib>

#include "BirdData.h"
#include "../definitions.h"
#include "../Score.h"

class GameEngine;

struct PipeColData {
  float sx;
  float sy;
  float sw;
  float sh;
  float dx;
  float dy;
};

class Pipe
{
private:
  float fScaleFactor = 1.0f;
  float fWidth = 1.0f;
  float fHeight = 1.0f;
  float fScoreOffsetX = 0.0f;

  float fOffX = 0.0f;
  float fOffY = 0.0f;
  float fVertGap = GAME_HEIGHT / 3;
  float fVertYOffset = 0.0f;

  float fMidY = GAME_HEIGHT / 2;
  float fGapY = 0.0f;

  olc::Decal *dPipes;

  PipeColData * colDataTop;
  PipeColData * colDataBot;

public:
  bool bTraversed = false;
  bool bFirst = false;

  static GameEngine * gEngine;

private:
  void setOffY(bool first);
  void initColData();
  bool testColPipe(Bird * bird, PipeColData * cd, float fGameDistance);

public:
  Pipe() {
    setOffY(false);
    initColData();
  }

  Pipe(float offX, float offY, olc::Decal *pipes, float sw, float sh, float scale, bool first, float scoreX);
  void setOffset(float offX, float offY, float scoreX);
  bool checkCollision(Bird * bird, float fGameDistance);
  bool isVisible();
  bool isPast();
  void render(olc::PixelGameEngine *engine, float fElapsedTime);
  void reset(float offX, bool first);
  void update(float fElapsedTime, float gameSpeed);
  float getX();
};

GameEngine * Pipe::gEngine = nullptr;

Pipe::Pipe(float offX, float offY, olc::Decal *pipes, float sw, float sh, float scale, bool first, float scoreX)
{
  fOffX = offX;
  fOffY = offY;
  dPipes = pipes;
  fScaleFactor = scale;
  fWidth = sw * scale;
  fHeight = sh * scale;
  bFirst = first;
  fScoreOffsetX = scoreX;

  setOffY(bFirst);
  initColData();
}

void Pipe::initColData() {
  float dY = ((fHeight * fScaleFactor * 2) - GAME_HEIGHT) / 2;
  float offTopY = 0 - dY - fVertGap / 2 + fVertYOffset / 2;
  float offBotY = fHeight * fScaleFactor + offTopY + fVertGap;

  colDataTop = new PipeColData();
  colDataBot = new PipeColData();

  colDataTop->dx = fOffX;
  colDataBot->dx = fOffX;
  colDataTop->dy = offTopY;
  colDataBot->dy = offBotY;
  colDataTop->sx = fOffX;
  colDataBot->sx = fOffX;
  colDataTop->sy = offTopY + fHeight * fScaleFactor;
  colDataBot->sy = offBotY;
  colDataTop->sh = fHeight * fScaleFactor;
  colDataBot->sh = fHeight * fScaleFactor;
  colDataTop->sw = fWidth * fScaleFactor;
  colDataBot->sw = fWidth * fScaleFactor;
}

void Pipe::setOffset(float offX, float offY, float scoreX)
{
  fOffX = offX;
  fOffY = offY;
  fScoreOffsetX = scoreX;
}

bool Pipe::checkCollision(Bird * bird, float fGameDistance)
{
  // Skip!
  // if (bTraversed || !isVisible()) return false;
  return testColPipe(bird, colDataTop, fGameDistance) || testColPipe(bird, colDataBot, fGameDistance);
}

bool Pipe::testColPipe(Bird * bird, PipeColData * cd, float fGameDistance) {
  BirdData bd = bird->getBirdColMask();
  float testX = bd.cx;
  float testY = bd.cy;

  // Closest edge
  if (bd.cx < cd->dx)                 testX = cd->dx;           // left edge
  else if (bd.cx > cd->dx + cd->sw)   testX = cd->dx + cd->sw;  // right edge
  if (bd.cy < cd->dy)                 testY = cd->dy;           // top edge
  else if (bd.cy > cd->dy + cd->sh)   testY = cd->dy + cd->sh;  // bottom edge

  // Get distance from edge
  float distX = bd.cx - testX;
  float distY = bd.cy - testY;
  float distance = sqrt((distX * distX) + (distY * distY));

  float t = 1;
  if (distance <= bd.r) {
       t++;
    // do nothing
  }

  //  If the distance is less than radius
    return (distance <= bd.r);
}

bool Pipe::isVisible()
{
  return !isPast() && fOffX < GAME_WIDTH;
}

bool Pipe::isPast()
{
  return (fOffX + fWidth * fScaleFactor) + 1 < 0;
}

void Pipe::render(olc::PixelGameEngine *engine, float fElapsedTime)
{
  if (!gSettings.DEBUG_MODE) {
    olc::vf2d top = {colDataTop->dx, colDataTop->dy};
    engine->DrawPartialDecal(top, {fWidth, fHeight}, dPipes, {PIPE_SW, 0}, {fWidth, fHeight});
    // engine->DrawDecal(top, dPipes, {fScaleFactor, fScaleFactor});

    #ifdef __APPLE__
    olc::vf2d bot = {colDataBot->dx, colDataBot->dy};
    engine->DrawPartialDecal(bot, {fWidth, fHeight}, dPipes, {0, 0}, {fWidth, fHeight});
    // engine->DrawDecal(bot, dPipeTop, {fScaleFactor, fScaleFactor});
    #endif // __APPLE__
    #ifndef __APPLE__
    olc::vf2d bot = {colDataBot->dx, colDataBot->dy};
    engine->DrawDecal(bot, dPipeBot, {fScaleFactor, fScaleFactor});
    #endif // !__APPLE__
  }

  if (gSettings.DEBUG_MODE) {
    engine->DrawRect({(int)colDataBot->dx, (int)colDataBot->dy}, {(int)colDataBot->sw, (int)colDataBot->sh}, {0,0,255});
    engine->DrawRect({(int)colDataTop->dx, (int)colDataTop->dy}, {(int)colDataTop->sw, (int)colDataTop->sh}, {0,0,255});
  }
}

void Pipe::update(float fElapsedTime, float gameSpeed)
{
  fOffX -= gameSpeed;
  colDataTop->dx = fOffX;
  colDataBot->dx = fOffX;

  // Check if passed
  if (!bTraversed && fOffX + fWidth * fScaleFactor < fScoreOffsetX)
  {
    bTraversed = true;
    gScore.newScore = true;
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
  if (std::abs(fVertYOffset) > fHeight * fScaleFactor)
  {
    fVertYOffset *= 0.85f;
  }
}

float Pipe::getX()
{
  return fOffX;
}