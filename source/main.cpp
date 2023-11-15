#include "GameEngine.h"
#include "GameScript.h"
#include "scripts/gCommand_Score.h"

int main() {
	GameEngine game;
	gCommand::gEngine = &game;
	// Run that game!
	game.run();

	return 0;
}