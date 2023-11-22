#ifndef SCENE_PIPE
#define SCENE_PIPE

#pragma once
#include <cstdlib>

#include "BirdData.h"
#include "../definitions.h"
#include "../Score.h"

extern struct GameSettings gSettings;

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
  float fVertGap = ENABLE_DEBUG_MODE ? gSettings.P_GAP : PIPE_GAP;
  float fVertYOffset = 0.0f;

  float fMidY = GAME_HEIGHT / 2;
  float fGapY = 0.0f;
  float fPipeGap = PIPE_GAP;

  olc::Decal *dPipes;

  PipeColData * colDataTop = nullptr;
  PipeColData * colDataBot = nullptr;

public:
  bool bTraversed = false;
  bool bFirst = false;

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
  bool withinRadius(Bird * bird);
  bool isVisible();
  bool isPast();
  void render(olc::PixelGameEngine *engine, float fElapsedTime);
  void reset(float offX, bool first);
  void update(float fElapsedTime, float gameSpeed, float pipeGap);
  float getX();
  float getY();
};

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

  if (colDataTop == nullptr) colDataTop = new PipeColData();
  if (colDataBot == nullptr) colDataBot = new PipeColData();

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

bool Pipe::withinRadius(Bird * bird) {
  BirdData bd = bird->getBirdColMask();
  if (bd.cx + bd.dw + 5 < colDataTop->dx) return false;
  if (bd.cx + 5 + bd.dw + bd.dw * 2 < colDataTop->dx + colDataTop->sw) return false;
  return true;
}

bool Pipe::testColPipe(Bird * bird, PipeColData * cd, float fGameDistance) {
  BirdData bd = bird->getBirdColMask();
  float testX = bd.cx;
  float testY = bd.cy;

  // Closest edge from top-left (0,0)
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
    olc::vf2d bot = {colDataBot->dx, colDataBot->dy};
    engine->DrawPartialDecal(bot, {fWidth, fHeight}, dPipes, {0, 0}, {fWidth, fHeight});
  }

  if (gSettings.DEBUG_MODE) {
    engine->DrawRect({(int)colDataBot->dx, (int)colDataBot->dy}, {(int)colDataBot->sw, (int)colDataBot->sh}, {0,0,255});
    engine->DrawRect({(int)colDataTop->dx, (int)colDataTop->dy}, {(int)colDataTop->sw, (int)colDataTop->sh}, {0,0,255});
  }
}

void Pipe::update(float fElapsedTime, float gameSpeed, float pipeGap)
{
  fOffX -= gameSpeed;
  colDataTop->dx = fOffX;
  colDataBot->dx = fOffX;

  // Check if passed
  if (!bTraversed && colDataTop->dx + colDataTop->sw < fScoreOffsetX)
  {
    bTraversed = true;
    gScore.newScore = true;
  }

  fPipeGap = pipeGap;
}

void Pipe::reset(float offX, bool first)
{
  fOffX = offX;
  fOffY = 0.0f;
  bTraversed = false;
  bFirst = first;
  fVertYOffset = 0;
  setOffY(first);
  initColData();
}

void Pipe::setOffY(bool first)
{
  if (first)
  {
    fVertYOffset = 0;
    return;
  }
  
  fVertGap = ENABLE_DEBUG_MODE ? gSettings.P_GAP : fPipeGap;

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

float Pipe::getY() {
  return fOffY;
}

#endif // SCENE_PIPE