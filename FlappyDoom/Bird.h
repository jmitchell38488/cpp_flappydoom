#pragma once
#include "Settings.h";
#include "Animation.h";

#define OLC_SOUNDWAVE
#include "olcSoundWaveEngine.h"

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
	bool checkCollision();
	void update(float fElapsedTime);
	void reset();
	void setPlaying();
	void setDead();
	void setIdle();
	void flapped();
	void rotate();
	void score();
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

bool Bird::checkCollision() {
	// Check bottom edge collision with ground
	if (fPosition.y + fColHeight * (1 - fColHeightAdjust) > GAME_HEIGHT - GROUND_HEIGHT) {
		return true;
	}

	// Check top edge
	if (fPosition.y - (fColHeight * 0.65) < 0) {
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

void Bird::score() {
}