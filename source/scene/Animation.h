#pragma once

#include "../lib/olcPixelGameEngine.h"
#include "../definitions.h"

struct AnimFrame
{
	olc::Sprite *sprite;
	olc::Decal *decal;
	float offX;
	float offY;
	float sw;
	float sh;
	float ow;
	float oh;
	bool bPartial;
};

class Animation
{
protected:
	olc::vf2d fPosition;
	olc::vf2d fRotPosition;
	olc::vf2d fCentre;
	bool bCanAnimate = true;
	bool bSprite = false;
	float fScaleFactor = 1.0f;

	float fAnimRate = ANIM_TICK;
	float fLastAnimTime = 0.0f;

	float fRotation = 0.0f;
	AnimFrame *fCurFrame = nullptr;
	std::vector<AnimFrame *> fFrames = {};
	int fCurFrameIdx = 0;

public:
	~Animation();
	Animation() {}
	Animation(float scale, olc::vf2d fPosition);

	void reset();
	void addHorzSprite(olc::Sprite *sprite, int n, float sw, float sh, float scale);
	void setScale(float scale);
	void setPosition(olc::vf2d pos);
	void setAnimRate(float aRate);
	void addFrame(olc::Sprite *sprite, float sw, float sh, float offX, float offY, bool partial);
	void update(float fElapsedTime);
	void render(olc::PixelGameEngine *engine, float fElapsedTime);

	AnimFrame *getCurrentFrame();
	olc::vf2d getPosition();
	olc::vf2d getScale();
};

Animation::Animation(float scale, olc::vf2d pos)
{
	fScaleFactor = scale;
	fPosition = pos;
}

Animation::~Animation()
{
	// Delete pointers
	for (auto *fr : fFrames)
		delete fr;
}

void Animation::reset()
{
	fLastAnimTime = 0.0f;
	fCurFrameIdx = 0;
	fCurFrame = fFrames[fCurFrameIdx];
	bCanAnimate = true;
}

void Animation::addHorzSprite(olc::Sprite *sprite, int n, float sw, float sh, float scale)
{
	for (int i = 0; i < n; i++)
		addFrame(sprite, sw, sh, i * sw, 0, true);
}

void Animation::setScale(float scale)
{
	fScaleFactor = scale;

	if (fFrames.size() > 0)
	{
		for (auto frame : fFrames)
		{
			if (!frame->bPartial)
			{
				frame->sw = frame->sprite->width * fScaleFactor;
				frame->sh = frame->sprite->height * fScaleFactor;
			}
			else
			{
				frame->sw = frame->ow * fScaleFactor;
				frame->sh = frame->oh * fScaleFactor;
			}
		}
	}
}

void Animation::setPosition(olc::vf2d pos)
{
	fPosition = pos;
	fRotPosition = {(fCurFrame->sw / 2) + fPosition.x, (fCurFrame->sh / 2) + fPosition.y};
	fCentre = {
			fCurFrame->sw / 2,
			fCurFrame->sh / 2,
	};
}

void Animation::setAnimRate(float aRate)
{
	fAnimRate = aRate;
}

void Animation::addFrame(olc::Sprite *sprite, float sw = 0, float sh = 0, float offX = 0, float offY = 0, bool partial = false)
{
	AnimFrame *frame = new AnimFrame();
	frame->sprite = sprite;
	frame->decal = new olc::Decal(sprite);
	frame->offX = offX;
	frame->offY = offY;
	frame->ow = sw;
	frame->oh = sh;
	frame->sw = partial ? sw * fScaleFactor : sprite->width * fScaleFactor;
	frame->sh = partial ? sh * fScaleFactor : sprite->height * fScaleFactor;
	frame->bPartial = partial;

	fFrames.push_back(frame);

	if (fFrames.size() == 1)
		fCurFrame = fFrames[0];
}

void Animation::update(float fElapsedTime)
{
	fLastAnimTime += fElapsedTime;

	if (fLastAnimTime >= fAnimRate && fFrames.size() > 0 && bCanAnimate)
	{
		if (fCurFrameIdx + 1 < fFrames.size())
			fCurFrameIdx++;
		else
			fCurFrameIdx = 0;
		fCurFrame = fFrames[fCurFrameIdx];
		fLastAnimTime -= fAnimRate;
	}

	fRotPosition = {(fCurFrame->sw / 2) + fPosition.x, (fCurFrame->sh / 2) + fPosition.y};
	// fRotPosition = {(fCurFrame->sprite->width / 2 * fScaleFactor) + fPosition.x, (fCurFrame->sprite->height / 2 * fScaleFactor) + fPosition.y};
}

AnimFrame *Animation::getCurrentFrame()
{
	return fCurFrame;
}

olc::vf2d Animation::getPosition()
{
	return fPosition;
}

olc::vf2d Animation::getScale()
{
	return {fScaleFactor, fScaleFactor};
}

void Animation::render(olc::PixelGameEngine *engine, float fElapsedTime)
{
	if (!gSettings.DEBUG_MODE)
	{
		if (fRotation == 0)
		{
			if (!fCurFrame->bPartial)
				engine->DrawDecal(fPosition, fCurFrame->decal, getScale());
			else
				engine->DrawPartialDecal(fPosition, fCurFrame->decal, {fCurFrame->offX, fCurFrame->offY}, {fCurFrame->sw, fCurFrame->sh}, getScale());
		}
		else
		{
			if (!fCurFrame->bPartial)
				engine->DrawRotatedDecal(fRotPosition, fCurFrame->decal, fRotation, fCentre, getScale());
			else
				engine->DrawPartialRotatedDecal(fRotPosition, fCurFrame->decal, fRotation, fCentre, {fCurFrame->offX, fCurFrame->offY}, {fCurFrame->sw, fCurFrame->sh}, getScale());
			// else engine->DrawRotatedDecal(fRotPosition, fCurFrame->decal, fRotation, fCentre, getScale());
		}
	}

	if (gSettings.DEBUG_MODE)
	{
		engine->DrawCircle({(int)(fPosition.x + (fCurFrame->sw / 2)),
												(int)(fPosition.y + (fCurFrame->sh / 2))},
											 (int)(fCurFrame->sw / 2 * 0.95), {0, 255, 0});
	}
}