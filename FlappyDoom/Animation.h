#pragma once

#include "olcPixelGameEngine.h"
#include "GameObj.h"

class Animation {

private:
	std::vector<olc::Decal*> fFrames = {};
	int fCurFrameIdx = 0;
	olc::Decal* fCurFrame = nullptr;

protected:
	olc::vf2d fPosition;
	bool bCanAnimate = true;
	float fScaleFactor = 1.0f;

	float fAnimRate = ANIM_TICK;
	float fLastAnimTime = 0.0f;

	float fRotation = 0.0f;

public: 
	~Animation() {
		// Delete pointers
		for (olc::Decal* fr : fFrames) delete fr;
	}

	Animation() {}

	Animation(float scale, olc::vf2d fPosition);

	void reset();

	void setScale(float scale) {
		fScaleFactor = scale;
	}

	void setPosition(olc::vf2d pos) {
		fPosition = pos;
	}

	void setAnimRate(float aRate) {
		fAnimRate = aRate;
	}

	void addFrame(olc::Sprite* frame) {
		olc::Decal * decal = new olc::Decal(frame);
		fFrames.push_back(decal);

		if (fFrames.size() == 1) fCurFrame = fFrames[0];
	}

	void update(float fElapsedTime) {
		fLastAnimTime += fElapsedTime;

		if (fLastAnimTime < fAnimRate) return;
		if (fFrames.size() < 1) return;

		fLastAnimTime = 0.0f;

		if (!bCanAnimate) return;
		if ((fCurFrameIdx +1 ) < fFrames.size()) fCurFrameIdx++;
		else fCurFrameIdx = 0;

		fCurFrame = fFrames[fCurFrameIdx];
	}

	olc::Decal* getCurrentFrame() {
		return fCurFrame;
	}

	olc::vf2d getPosition() {
		return fPosition;
	}

	olc::vf2d getScale() {
		return { fScaleFactor, fScaleFactor };
	}

	void render(olc::PixelGameEngine* engine, float fElapsedTime) {
		if (fRotation == 0) {
			engine->DrawDecal(fPosition, fCurFrame, getScale());
		}
		else {
			olc::vf2d centre = {
				fCurFrame->sprite->width / 2 * fScaleFactor,
				fCurFrame->sprite->height / 2 * fScaleFactor,
			};
			engine->DrawRotatedDecal(fPosition, fCurFrame, fRotation, centre, getScale());
		}
		
	}

};

Animation::Animation(float scale, olc::vf2d pos) {
	fScaleFactor = scale;
	fPosition = pos;
}

void Animation::reset() {
	fLastAnimTime = 0.0f;
	fCurFrameIdx = 0;
	fCurFrame = fFrames[fCurFrameIdx];
	bCanAnimate = true;
}