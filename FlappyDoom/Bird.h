#pragma once
#include "Settings.h";
#include "Animation.h";

enum StateMachine {
	IDLE, DEAD, FLAP
};

class Bird : public Animation {

private:
	StateMachine state = IDLE;

public:

	Bird() {
		setAnimRate(1.0f / 10);
	}

	void init() {
		for (int i = 0; i < 5; i++) addFrame(new olc::Sprite((std::string)"./assets/bird/" + std::to_string(i) + ".png"));
		setPosition(BIRD_ORIG_POS);
		setScale(0.25);
	}

	bool isCollision() {

	}


};