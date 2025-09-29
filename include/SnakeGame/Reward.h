#pragma once

#include "Types.h"

class Reward
{
protected:
    int point = 1;

public:
    Reward();
    Reward(int point);
    Coord emerge(const Coord& emergeIndex);
    // 生成的奖励应当避开墙，
    // 由于apple和bonus的奖励生成地点是同样的生成方式，所以不需要在子类中重写

    void submerge();
};

class Apple: public Reward
{
public:
    Apple();
};

class Bonus: public Reward
{
public:
    Bonus();
};

