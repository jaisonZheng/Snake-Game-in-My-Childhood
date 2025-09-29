#pragma once

#include <memory>

#include "GameState.h"
#include "Snake.h"
#include "Reward.h"
#include "Types.h"

class Game
{
public:
    Game();

    void run();
    void start_new_game();

    MapSize get_map_size() const;
    void change_state(std::unique_ptr<GameState> new_state);
    Snake& get_snake();
    const Snake& get_snake() const;

private:
    void process_events();
    void update();
    void render();

    MapSize map_size;
    Snake snake;
    std::unique_ptr<GameState> state_;
};
