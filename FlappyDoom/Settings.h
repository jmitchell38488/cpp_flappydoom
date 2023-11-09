#pragma once

#include "olcPixelGameEngine.h"

const int GAME_WIDTH = 320;
const int GAME_HEIGHT = 200;
const int GAME_PIXEL = 4;

const int SCROLL_SPEED = 8;
const float FPS = 60.0f;
const float GAME_TICK = 1.0f / 60.0f;
const float ANIM_TICK = 1.0f / 12.0f;

const olc::vi2d BIRD_ORIG_POS = olc::vi2d(GAME_WIDTH / 4, GAME_HEIGHT / 3);

//struct Settings {
//public:
//	const int SCROLL_SPEED = 8;
//	const float FPS = 60.0f;
//	const float TICK = 1.0f / 60.0f;
//	const float ANIM_TICK = 1.0f / 12.0f;
//};
