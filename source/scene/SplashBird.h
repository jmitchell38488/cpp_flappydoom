#pragma once
#include "Bird.h"

enum class SB_ANIM_DIR {
	LEFT, RIGHT
};

class SplashBird : public Bird {
private:
	SB_ANIM_DIR sAnimDir;
public:
	SplashBird() : Bird() {
		bCanAnimate = true;
	}

	SplashBird(SB_ANIM_DIR dir) : SplashBird() {
		sAnimDir = dir;
	}

	void init() {
		Bird::init();
		float fPosX = sAnimDir == SB_ANIM_DIR::RIGHT ? 0 - BIRD_SW : GAME_WIDTH;
		setPosition({fPosX, 100});
		if (sAnimDir == SB_ANIM_DIR::LEFT) fScaleFactorX = 0-fScaleFactorX;
	}

	bool checkCollisionBounds() {
		return false;
	}

	void update(float fElapsedTime, float gameRunTime, bool canAnimate = true) {
		// Update frame
		fVelocity = sine_between(gameRunTime, 0.65, -1.25f, 1.25f);
		Animation::update(fElapsedTime);

		float fPosX = sAnimDir == SB_ANIM_DIR::RIGHT ? fPosition.x + 3.0f : fPosition.x - 3.0f;
		setPosition({ fPosX, (float)(fPosition.y + fVelocity) });
	}
};