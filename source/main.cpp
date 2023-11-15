#include "GameEngine.h"
#include "GameScript.h"
#include "scene/Scene.h"
#include "scripts/gCommand_Score.h"

int main() {
	GameEngine game;

	// Set some statics...
	gCommand::gEngine = &game;
	Scene::gEngine = &game;

	// Run that game!
	game.run();

	return 0;
}