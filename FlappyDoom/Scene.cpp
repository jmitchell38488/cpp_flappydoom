#pragma once
#include "Scene.h"

void Scene::loadAssets() {
	Bird sBird;
	sBird.init();
}

void Scene::initScene() {
	loadAssets();
}

bool Scene::tick(float fElapsedTime) {
	sBird.update(fElapsedTime);
	return true;
}

void Scene::render(olc::PixelGameEngine* engine, float fElapsedTime) {
	engine->Clear(olc::BLACK);
	engine->DrawString({ 20, 20 }, "fElapsedTime: " + std::to_string(fElapsedTime), olc::Pixel(255, 255, 255), 1);
	sBird.render(engine, fElapsedTime);
}