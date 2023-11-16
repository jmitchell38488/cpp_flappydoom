#pragma once
#include "../Settings.h"
#include "Animation.h"
#include "BirdData.h"

#define OLC_SOUNDWAVE
#include "../lib/olcSoundWaveEngine.h"

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
	for (int i = 0; i < 5; i++) addFrame(new olc::Sprite((std::string)"./assets/bird/" + std::to_string(i) + ".png"));
	setPosition({ BIRD_X, BIRD_Y });
	setScale(0.5);
}

float Bird::getWidth() {
  return fCurFrame->sprite->width * fScaleFactor;
}

bool Bird::checkCollisionBounds() {
  // Test top
  if (fPosition.y < GROUND_HEIGHT) {
    return true;
  }
  float dh = fCurFrame->sprite->height * fScaleFactor;
  if (fPosition.y + dh > GAME_HEIGHT - GROUND_HEIGHT) {
    return true;
  }

	return false;
}

void Bird::update(float fElapsedTime) {
	float localTick = fLastAnimTime + fElapsedTime;
	if (state == BirdState::DEAD) {
		return;
	}

	// Bob up and down on a sin wave
	if (state == BirdState::IDLE) {
		return;
	}

	// Update frame
	Animation::update(fElapsedTime);

	fVelocity += GRAVITY_TICK;
	//fPosition = { fPosition.x + fVelocity * 0.5 + GRAVITY, fPosition.y };
	setPosition({ BIRD_X, (float)(fPosition.y + fVelocity * 0.5 + GRAVITY_TICK) });
	rotate();
}

void Bird::flapped() {
	fVelocity = -BIRD_JUMP;
	bFlapped = true;
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
	fRotation = 1.45 * fVelocity;
	if (fRotation > BIRD_ANGLE / 90) fRotation = BIRD_ANGLE / 90;
}

BirdData Bird::getBirdColMask() {
  BirdData bd;
  bd.cx = fRotPosition.x;
  bd.cy = fRotPosition.y;
  bd.dx = fPosition.x;
  bd.dy = fPosition.y;
  bd.dw = fCurFrame->sprite->width * fScaleFactor;
  bd.dh = fCurFrame->sprite->height * fScaleFactor;
  bd.r = fCurFrame->sprite->width / 2 * fScaleFactor * 0.9;

  return bd;
}