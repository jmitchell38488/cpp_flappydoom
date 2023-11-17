#pragma once

const int GAME_WIDTH = 800;
const int GAME_HEIGHT = 450;
const int GAME_PIXEL = 2;

// Ground
const int GROUND_HEIGHT = GAME_HEIGHT / 12;

// const int SCROLL_SPEED = 8;
const float FPS = 60.0f;
const float GAME_TICK = 1.0f / FPS;
const float ANIM_TICK = 1.0f / 12.0f;
const float GAME_SPEED = 1.0f / 250.0f;
const float BG_SPEED = 1.0f / 90.0f;
const float SCROLL_SPEED = 4.0f * ANIM_TICK;

// Gravity
const float GRAVITY = 1.0f;
const float GRAVITY_TICK = GRAVITY * 0.7f;

const float BIRD_X = GAME_WIDTH / 4;
const float BIRD_Y = GAME_HEIGHT / 3;
const float BIRD_ANGLE = 45.0f;
const float BIRD_JUMP = 16.5f;
