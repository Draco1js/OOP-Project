#include "Game.h"

int main()
{
    // Game Instance
    Game game;
    // Initializes Game
    game.Initialize();

    // Runs Game Loop
    game.RunGameLoop();

    // Deletes / Cleans Memory
    game.Cleanup();

    return 0;
}