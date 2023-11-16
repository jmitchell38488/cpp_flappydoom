#pragma once

#include "../lib/olcPixelGameEngine.h"

class Animation {

private:
	std::vector<olc::Decal*> fFrames = {};
	int fCurFrameIdx = 0;

protected:
	olc::vf2d fPosition;
	bool bCanAnimate = true;
	float fScaleFactor = 1.0f;

	float fAnimRate = ANIM_TICK;
	float fLastAnimTime = 0.0f;

	float fRotation = 0.0f;
	olc::Decal* fCurFrame = nullptr;

	float fColHeight;
	float fColWidth;
	float fColHeightAdjust = 0.2f;

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

		if (fFrames.size() > 0) {
			fColHeight = fCurFrame->sprite->height * fScaleFactor;
			fColWidth = fCurFrame->sprite->width * fScaleFactor;
		}
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

		fColHeight = fCurFrame->sprite->height * fScaleFactor;
		fColWidth = fCurFrame->sprite->width * fScaleFactor;
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
				fCurFrame->sprite->width * 0.5f,
				fCurFrame->sprite->height * 0.5f,
			};
      olc::vf2d nPos = { (fCurFrame->sprite->width / 2 * fScaleFactor) + fPosition.x, (fCurFrame->sprite->height / 2 * fScaleFactor) + fPosition.y};
			engine->DrawRotatedDecal(nPos, fCurFrame, fRotation, centre, getScale());
		}

    engine->DrawCircle({
      (int)(fPosition.x + (fCurFrame->sprite->width / 2 * fScaleFactor)),
      (int)(fPosition.y + (fCurFrame->sprite->height / 2 * fScaleFactor))
      }, (int)(fCurFrame->sprite->width / 2 * fScaleFactor * 0.9), {0,255,0});
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