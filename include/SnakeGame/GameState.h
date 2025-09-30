#pragma once

#include "Types.h"

// 这是整个游戏状态控制的核心，所有的游戏状态都是以这个为基类，所有的状态类都需要重写本类。
// 这么搞是为了良好的工程实践。要不是为了学学这些奇怪的工程实践，这玩意也就200行代码吧。。。。。。。

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
