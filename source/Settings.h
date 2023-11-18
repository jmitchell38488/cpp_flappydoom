#pragma once

const int GAME_WIDTH = 800;
const int GAME_HEIGHT = 450;
const int GAME_PIXEL = 2;

// Ground
const int GROUND_HEIGHT = 37;
const int CEILING_HEIGHT = 15;

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
const float BIRD_SW = 61.0f;
const float BIRD_SH = 71.0f;
const float BIRD_SF = 1.0f;

const float PIPE_SW = 62.0f;
const float PIPE_SH = 300.0f;
const float PIPE_SF = 1.0f;
const float PIPE_GAP = GAME_HEIGHT / 3;

const bool ENABLE_DEBUG_MODE = false;