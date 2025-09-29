#pragma once

#include "GameState.h"

class GameOverState : public GameState 
// c++中类继承就是这个语法，python中才是括号
{
public:
    void on_enter(Game* game) override;
    void on_exit(Game* game) override;
    void handle_input(Game* game, const KeyEvent& key) override;
    void update(Game* game) override;
    void draw(Game* game) override;
};
