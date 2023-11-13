#pragma once

class Pipe {

private:
	float fScale = 1.0f;
	float fWidth = 1.0f;
	float fHeight = 1.0f;

	float fOffX = 0.0f;
	float fOffY = 0.0f;
	float fVertGap = GAME_HEIGHT / 4;
	float fVertYOffset = 0.0f;

	olc::Decal* dPipe;

	bool bFlip = false;

public: 
	bool traversed = false;

private:
	void setOffY();

public:
	Pipe() {}
	Pipe(float offX, float offY, olc::Decal* dPipe, float width, float height);
	void setOffset(float offX, float offY);
	bool checkCollision(Bird bird);
	bool isVisible(float offX, float sw);
	bool isPast(float offX);
	void render(olc::PixelGameEngine* engine, float fElapsedTime);
	void reset(float offX);
};

Pipe::Pipe(float offX, float offY, olc::Decal* pipe, float width, float height) {
	fOffX = offX;
	fOffY = offY;
	dPipe = pipe;
	fWidth = width;
	fHeight = height;
	fScale = 0.25f;

	setOffY();
}

void Pipe::setOffset(float offX, float offY) {
	fOffX = offX;
	fOffY = offY;
}

bool Pipe::checkCollision(Bird bird) { return false; }

bool Pipe::isVisible(float offX, float sw) {
	return fOffX > offX && fOffX < offX + sw;
}

bool Pipe::isPast(float offX) {
	return fOffX < offX;
}

void Pipe::render(olc::PixelGameEngine* engine, float fElapsedTime) {

	float offY = bFlip ? -fVertYOffset : fVertYOffset;
	// Draw top
	float wMp = GAME_HEIGHT / 2;
	float offYt = 0 - offY - (fVertGap / 2);
	//float offYt = wMp - fVertGap / 2 - fVertYOffset;
	olc::vf2d top = { fOffX - fWidth / 2, offYt };
	engine->DrawDecal(top, dPipe, { fScale, fScale });

	// Draw bottom
	//float offYb = wMp + fVertGap / 2 + fVertYOffset;
	float offYb = wMp + (fVertGap / 2) + offY;
	//float offYb = GAME_HEIGHT + fVertYOffset - fHeight + (fVertGap / 2);
	olc::vf2d bot = { fOffX - fWidth / 2, offYb };
	engine->DrawDecal(bot, dPipe, { fScale, fScale });

	bFlip = !bFlip;
}

void Pipe::reset(float offX) {
	fOffX = offX;
	fOffY = 0.0f;
	traversed = false;

	setOffY();
}

void Pipe::setOffY() {
	float dy = fVertGap - (GAME_HEIGHT - fHeight * 2);
	// height of a segment
	//float dy = fHeight - ((GAME_HEIGHT - fVertGap) / 2);
	fVertYOffset = ((float)std::rand() / (RAND_MAX)) * dy;
}