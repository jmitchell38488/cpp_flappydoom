#pragma once

class Pipe {

private:
	float fScale = 1.0f;
	float fWidth = 1.0f;
	float fHeight = 1.0f;

	float fOffX = 0.0f;
	float fOffY = 0.0f;
	float fVertGap = GAME_HEIGHT / 2.5;

	olc::Decal* dPipe;

public:
	Pipe() {}
	Pipe(float offX, float offY, olc::Decal* dPipe, float width, float height);
	void setOffset(float offX, float offY);
	bool checkCollision(Bird bird);
	bool isVisible(float offX, float sw);
	bool isPast(float offX);
	void render(olc::PixelGameEngine* engine, float fElapsedTime);
};

Pipe::Pipe(float offX, float offY, olc::Decal* pipe, float width, float height) {
	fOffX = offX;
	fOffY = fOffY;
	dPipe = pipe;
	fWidth = width;
	fHeight = height;
	fScale = 0.25f;
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
	// Draw top
	float wMp = GAME_HEIGHT / 2;
	float offYt = wMp - fVertGap - fOffY;
	olc::vf2d top = { fOffX - fWidth / 2, offYt };
	engine->DrawDecal(top, dPipe, { fScale, fScale });

	float offYb = wMp + fVertGap + fOffY;
	olc::vf2d bot = { fOffX - fWidth / 2, offYb };
	engine->DrawDecal(bot, dPipe, { fScale, fScale });
	// Draw bottom
}