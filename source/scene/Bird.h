#ifndef SCENE_BIRD
#define SCENE_BIRD

#pragma once
#include "../Settings.h"
#include "Animation.h"
#include "BirdData.h"
#include "../Util.h"

enum class BirdState {
	IDLE, DEAD, FLAP
};

class Bird : public Animation {

protected:
	BirdState state = BirdState::IDLE;

	float fVelocity = 0.0f;

	bool bFlapped = false;
	float fPrevVel = 0.0f;

public:

	Bird();
	void init();
	bool checkCollisionBounds();
	void update(float fElapsedTime, float gameRunTime, bool canAnimate);
	void reset();
	void setPlaying();
	void setDead();
	void setIdle();
	void flapped();
	void rotate();
  float getWidth();
  BirdData getBirdColMask();
};

#endif // SCENE_BIRD