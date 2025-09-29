#pragma once

#include "GameState.h"

class MainMenuState : public GameState
{
public:
    void on_enter(Game* game) override;
    void handle_input(Game* game, const KeyEvent& key) override;
    void update(Game* game) override;
    void draw(Game* game) override;
};
