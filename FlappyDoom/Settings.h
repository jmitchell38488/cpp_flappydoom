#pragma once

#include "olcPixelGameEngine.h"

const int GAME_WIDTH = 800;
const int GAME_HEIGHT = 450;
const int GAME_PIXEL = 2;

// Ground
const int GROUND_HEIGHT = GAME_HEIGHT / 12;

const int SCROLL_SPEED = 8;
const float FPS = 60.0f;
const float GAME_TICK = 1.0f / FPS;
const float ANIM_TICK = 1.0f / 12.0f;
const float GAME_SPEED = 1.0f / 250.0f;
const float BG_SPEED = 1.0f / 120.0f;

// Gravity
const float GRAVITY = 1.0f / 10; // frame rate for anims
const float GRAVITY_TICK = GRAVITY / FPS; // updates per second

const float BIRD_X = GAME_WIDTH / 4;
const float BIRD_Y = GAME_HEIGHT / 3;
//const olc::vf2d BIRD_ORIG_POS = olc::vf2d(GAME_WIDTH / 4, GAME_HEIGHT / 3);

//struct Settings {
//public:
//	const int SCROLL_SPEED = 8;
//	const float FPS = 60.0f;
//	const float TICK = 1.0f / 60.0f;
//	const float ANIM_TICK = 1.0f / 12.0f;
//};
