#ifndef SCENE_SPLASH_BIRD
#define SCENE_SPLASH_BIRD

#pragma once
#include "Bird.h"

enum class SB_ANIM_DIR {
	LEFT, RIGHT
};

class SplashBird : public Bird {

private:
	SB_ANIM_DIR sAnimDir;
	bool bDied = false;

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
		if (!bDied)
			fVelocity = sine_between(gameRunTime, 0.65, -1.55f, 1.55f);

		// Update frame
		Animation::update(fElapsedTime);

		if (bDied) {
			float fPosX = sAnimDir == SB_ANIM_DIR::RIGHT ? fPosition.x + 1.25f : fPosition.x - 1.25f;
			fVelocity += GRAVITY_TICK / 2;
			setPosition({ fPosX, (float)(fPosition.y + fVelocity) });
			rotate();
		} else {
			float fPosX = sAnimDir == SB_ANIM_DIR::RIGHT ? fPosition.x + 5.0f : fPosition.x - 5.0f;
			setPosition({ fPosX, (float)(fPosition.y + fVelocity) });
		}
	}

	void died() {
		bDied = true;
		bCanAnimate = false;
	}

	bool hasDied() {
		return bDied;
	}
};

#endif // SCENE_SPLASH_BIRD