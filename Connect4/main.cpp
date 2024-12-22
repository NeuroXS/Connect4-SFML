#include "Game.h"
#include "Textures.h"

int main()
{
    Textures::loadTextures();
    Game::get_instance().start();

    return 0;
}