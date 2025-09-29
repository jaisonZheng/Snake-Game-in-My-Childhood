
#include "../include/SnakeGame/Snake.h"
#include "../include/SnakeGame/Types.h"
#include <algorithm>




Snake::Snake()
{
    /*
    需要初始化一条蛇，
    显然已开始蛇的蛇头应当出现在一个固定的位置，
    未指定时，蛇身的长度为0，但是要初始化蛇身vector
    蛇速未定，（注意 速度应当是世界的规则（即多久更新一次frame））
    头方向朝右
    */
    head = Coord{0, 0};
    length = 1;
    direction = Direction::RIGHT;
    body = deque<Coord>{};
}

Snake::Snake(const Coord& initialCoord, const Direction& initialDir)
{
    head = initialCoord;
    length = 1;
    direction = initialDir;
    body = deque<Coord>{};
}

bool Snake::is_crash_itself()
{
    for (const auto& item : body)
    {
        if (head == item)
        {
            return true;
        }
    }
    return false;
}

void Snake::turn(const Direction& dir)
{
    if (dir != oppositeDir(direction))
    {
        direction = dir;
    }
}

void Snake::move(const MapSize& boundary, bool is_eating_food)
{
    /*
    这里要实现从屏幕另一端出现的逻辑
    边界是左闭右开的
    */
    Coord bias;
    switch (direction)
    {
        case Direction::RIGHT:
            bias = Coord{1, 0};
            break;
        case Direction::LEFT:
            bias = Coord{-1, 0};
            break;
        case Direction::UP:
            bias = Coord{0, -1};
            break; 
        case Direction::DOWN:
            bias = Coord{0, 1};
            break;                   
    }

    // 下面我将使用一个数学小技巧来实现从屏幕另一端出来的功能
    const int max_x = std::max(1, boundary.width / CELL_SIZE);
    const int max_y = std::max(1, boundary.height / CELL_SIZE);

    Coord nextCoord;
    nextCoord.x = (head.x + bias.x + max_x) % max_x;
    nextCoord.y = (head.y + bias.y + max_y) % max_y;
    // 注意 这里前面括号内的值的绝对值一定小于boundary，所以不会出现+boundary之后仍然是负值的情况
    
    // 更新蛇身和蛇头的状态。用蛇这个名字感觉好怪，好哈人，不如叫蛇蛇哈哈哈哈哈。
    body.push_front(head);
    if (!is_eating_food && !body.empty())
    {
        body.pop_back();
    }

    head = nextCoord;
    length = static_cast<int>(body.size()) + 1;
}

void Snake::reset(const MapSize& map_size)
{
    body.clear();
    direction = Direction::RIGHT;

    const int max_x = std::max(1, map_size.width / CELL_SIZE);
    const int max_y = std::max(1, map_size.height / CELL_SIZE);

    head = { max_x / 2, max_y / 2 };
    length = 1;
}