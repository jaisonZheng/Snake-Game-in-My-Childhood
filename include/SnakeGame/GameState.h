#pragma once

#include "Types.h"

class Game;

class GameState
{
public:
    virtual ~GameState() = default;
    virtual void on_enter(Game* game) {}
    virtual void on_exit(Game* game) {}
    virtual void handle_input(Game* game, const KeyEvent& key) = 0;
    virtual void update(Game* game) = 0;
    virtual void draw(Game* game) = 0;
};
