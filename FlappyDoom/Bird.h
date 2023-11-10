#pragma once
#include "Settings.h";
#include "Animation.h";

enum class BirdState {
	IDLE, DEAD, FLAP
};

class Bird : public Animation {

private:
	BirdState state = BirdState::IDLE;

	float fFallVelocity = 0.0f;

public:

	Bird();
	void init();
	bool isCollision();
	void update(float fElapsedTime);
	void reset();
	void setPlaying();
	void setDead();
	void setIdle();
	void flapped();
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

bool Bird::isCollision() {
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

	fFallVelocity += GRAVITY_TICK;
	//fPosition = { fPosition.x + fFallVelocity * 0.5 + GRAVITY, fPosition.y };
	setPosition({ BIRD_X, (float)(fPosition.y + fFallVelocity * 0.5 + GRAVITY_TICK) });
}

void Bird::flapped() {
	fFallVelocity = 0.0f;
}

void Bird::reset() {
	Animation::reset();
	setPosition({ BIRD_X, BIRD_Y });
	state = BirdState::IDLE;
	bCanAnimate = false;
	fFallVelocity = 0.0f;
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