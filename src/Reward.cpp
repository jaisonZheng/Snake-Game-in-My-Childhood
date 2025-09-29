#include <vector>
#include "../include/SnakeGame/Types.h"
#include "../include/SnakeGame/Reward.h"

Reward::Reward() : point(1) {}

Reward::Reward(int point) : point(point) {}

// Coord emerge(MapSize& boundry, std::vector<Coord> walls); // 该逻辑应当在Game类中处理。
Coord Reward::emerge(const Coord& emergeIndex)
{
    return emergeIndex;
}

void Reward::submerge()
{
    // 奖励消失的逻辑
}

Apple::Apple() : Reward(1) {}

Bonus::Bonus() : Reward(5) {} // 这里的是baseReward，reward是剩余时间越多就越多的

