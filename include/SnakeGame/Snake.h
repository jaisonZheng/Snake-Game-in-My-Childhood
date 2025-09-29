#pragma once

#include <deque> // 这样可以很方便地处理蛇身的移动问题

using std::deque; // 实在是不想敲那么多次std了

#include "Types.h"
// 一开始本来想用<utility>中的vector的，但这样其实代码清晰度不足（不知道first对应什么）
// 最终还是决定用struct


class Snake
{
private:
    Coord head;
    int length;
    Direction direction;
    deque <Coord> body;
    
public:
    Snake();
    Snake(const Coord& initialCoord, const Direction& initialDir);

    bool is_crash_itself();

    void turn(const Direction& dir);

    void move(const MapSize& boundary, bool is_eating_food = false);
    void reset(const MapSize& map_size);
    
    // Getter方法
    const Coord& get_head() const { return head; }
    const Direction& get_direction() const { return direction; }
    const deque<Coord>& get_body() const { return body; }
    int get_length() const { return length; }

};
