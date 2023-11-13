#pragma once
#include "Pipe.h"

enum class PipesState {
	IDLE, SCROLLING
};

class Pipes {
private:
	float fGameScrollPosX = 0.0f;

	std::vector<Pipe*> vPipes;
	olc::Sprite* sPipe;
	olc::Decal* dPipe;

	float fScrollRate = 0.0f;
	float fLastAnimTime = 0.0f;
	float fLastXOffset = 0.0f;

	PipesState state = PipesState::IDLE;

protected:
	void addPipes();

public:
	Pipes() {}
	void init();
	void update(float fElapsedTime);
	void render(olc::PixelGameEngine* engine, float fElapsedTime);
	void setScrolling();
	void setIdle();
	void reset();
	bool checkCollision(Bird bird);
};

void Pipes::init() {
	sPipe = new olc::Sprite((std::string)"./assets/images/top_pipe.png");
	dPipe = new olc::Decal(sPipe);

	addPipes();
}
void Pipes::update(float fElapsedTime) {
	fLastAnimTime += fElapsedTime;

	if (fLastAnimTime > GAME_TICK) {
		fLastAnimTime = 0.0f;
		fGameScrollPosX += ANIM_TICK;

	for (auto pipe : vPipes) {
		if (pipe->isPast(fGameScrollPosX) && pipe->traversed) {
			fLastXOffset += GAME_WIDTH / 2.5;
			pipe->reset(fLastXOffset);
		}
	}
		

		//for (Pipe* pipe : vPipes) {
		//	pipe->setOffset()
	}
}
void Pipes::render(olc::PixelGameEngine* engine, float fElapsedTime) {
	for (Pipe* pipe : vPipes) {
		if (pipe->isPast(fGameScrollPosX)) continue;
		if (!pipe->isVisible(fGameScrollPosX, GAME_WIDTH)) continue;

		pipe->render(engine, fElapsedTime);
	}
}
void Pipes::setScrolling() {
	state = PipesState::SCROLLING;
}

void Pipes::setIdle() {
	state = PipesState::IDLE;
}

void Pipes::reset() {
	state = PipesState::IDLE;
	for (Pipe* pipe : vPipes) delete pipe;

	addPipes();
}

bool Pipes::checkCollision(Bird bird) { return false; }

void Pipes::addPipes() {
	float startOffX = GAME_WIDTH / 2;
	float stepX = GAME_WIDTH / 3;
	vPipes.push_back(new Pipe(GAME_WIDTH / 2, 0, dPipe, sPipe->width, sPipe->height));
	fLastXOffset = startOffX;
	for (int i = 0; i < 1; i++) {
		float nextX = fLastXOffset + stepX;
		vPipes.push_back(new Pipe(nextX, 0, dPipe, sPipe->width, sPipe->height));
		fLastXOffset = nextX;
	}
}