#pragma once
#include "../Settings.h"
#include "Animation.h"
#include "BirdData.h"

enum class BirdState {
	IDLE, DEAD, FLAP
};

class Bird : public Animation {

private:
	BirdState state = BirdState::IDLE;

	float fVelocity = 0.0f;

	bool bFlapped = false;

public:

	Bird();
	void init();
	bool checkCollisionBounds();
	void update(float fElapsedTime);
	void reset();
	void setPlaying();
	void setDead();
	void setIdle();
	void flapped();
	void rotate();
  float getWidth();
  BirdData getBirdColMask();
};

Bird::Bird() {
	setAnimRate(1.0f / 10);
	bCanAnimate = false;
}

void Bird::init() {
	// for (int i = 0; i < 5; i++) addFrame(new olc::Sprite((std::string)"./assets/bird/" + std::to_string(i) + ".png"));
	addHorzSprite(new olc::Sprite((std::string)"./assets/s-bird.png"), 5, BIRD_SW, BIRD_SH, BIRD_SF);
	setPosition({ BIRD_X, BIRD_Y });
}

float Bird::getWidth() {
  return fCurFrame->sw;
}

bool Bird::checkCollisionBounds() {
  // Test top
  if (fPosition.y < CEILING_HEIGHT) {
    return true;
  }
  float dh = fCurFrame->sh;
  if (fPosition.y + dh > GAME_HEIGHT - GROUND_HEIGHT) {
    return true;
  }

	return false;
}

void Bird::update(float fElapsedTime) {
	if (state == BirdState::DEAD || !bCanAnimate) {
		return;
	}

	// Bob up and down on a sin wave
	// if (state == BirdState::IDLE) {
	// 	return;
	// }

	// Update frame
	Animation::update(fElapsedTime);

	fVelocity += GRAVITY_TICK;
  setPosition({ BIRD_X, (float)(fPosition.y + fVelocity * 0.5 + GRAVITY_TICK) });
  rotate();
}

void Bird::flapped() {
	fVelocity = -BIRD_JUMP;
	bFlapped = true;
	fCurFrameIdx = 3;
	fCurFrame = fFrames[fCurFrameIdx];
  fLastAnimTime = 0.0f;
}

void Bird::reset() {
	Animation::reset();
	setPosition({ BIRD_X, BIRD_Y });
	state = BirdState::IDLE;
	bCanAnimate = false;
	fVelocity = 0.0f;
	fRotation = 0.0f;
	bFlapped = false;
}

void Bird::setPlaying() {
	state = BirdState::FLAP;
	bCanAnimate = true;
}

void Bird::setDead() {
	state = BirdState::DEAD;
	bCanAnimate = false;
}

void Bird::setIdle() {
	state = BirdState::IDLE;
	bCanAnimate = false;
}

void Bird::rotate() {
	fRotation = 0.15 * fVelocity;
	if (fRotation > BIRD_ANGLE / 90) fRotation = BIRD_ANGLE / 90;
}

BirdData Bird::getBirdColMask() {
  BirdData bd;
  bd.cx = fRotPosition.x;
  bd.cy = fRotPosition.y;
  bd.dx = fPosition.x;
  bd.dy = fPosition.y;
  bd.dw = fCurFrame->sw;
  bd.dh = fCurFrame->sh;
  bd.r = fCurFrame->sw / 2 * 0.95;

  return bd;
}