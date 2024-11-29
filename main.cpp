#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"

int main(){
    Game* game = new Game("config.txt");
    game->run();
    delete game;
    return 0;
}

