#pragma once

#include "GameState.h"
#include "Reward.h"
#include <vector>

class Snake;

class PlayingState : public GameState
{
private:
    Apple apple;
    Coord apple_position;
    std::vector<Coord> walls;
    int game_speed;
    int frame_counter;
    
public:
    PlayingState();
    void on_enter(Game* game) override;
    void handle_input(Game* game, const KeyEvent& key) override;
    void update(Game* game) override;
    void draw(Game* game) override;
    
private:
    void generate_apple(const MapSize& map_size, const Snake& snake);
    void generate_walls(const MapSize& map_size);
    bool is_position_valid(const Coord& pos, const MapSize& map_size, const Snake& snake) const;
};
