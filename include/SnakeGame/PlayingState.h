#pragma once

#include "GameState.h"
#include "Reward.h"
#include <vector>
#include "Types.h"

class Snake;

class PlayingState : public GameState
{
private:
    Apple apple;
    Coord apple_position;
    std::vector<Coord> walls; // 其实不应该放在这里的。应该放在Game中 唉 不管了
    int game_speed;
    int frame_counter;
    
public:
    PlayingState();
    void on_enter(Game* game) override;
    void handle_input(Game* game, const KeyEvent& key) override;
    void update(Game* game) override;
    void draw(Game* game) override;
    
private:
    void generate_apple(Game* game, const MapSize& map_size, const std::vector <Coord>& invalidIndex);
    void generate_bonus(Game* game, const MapSize& map_size, const std::vector <Coord>& invalidIndex);
    void generate_walls(Mode mode); // 墙壁的位置是由游戏模式决定的。
    bool is_position_valid(const Coord& pos, const MapSize& map_size, const std::vector <Coord> &index) const;
    std::vector <Coord> get_InvalidIndex(Game* game);
};
