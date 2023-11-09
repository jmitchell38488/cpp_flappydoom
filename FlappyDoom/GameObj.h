#pragma once

#include "olcPixelGameEngine.h"

class GameObj {
public:
	virtual void update(float fElapsedTime) = 0;
	virtual void render(olc::PixelGameEngine* engine, float fElapsedTime) = 0;
};