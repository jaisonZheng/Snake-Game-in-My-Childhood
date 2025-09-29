#pragma once

// 定义虚拟键常量（不依赖windows.h）
#ifndef VK_UP
#define VK_UP 38
#endif
#ifndef VK_DOWN
#define VK_DOWN 40
#endif
#ifndef VK_LEFT
#define VK_LEFT 37
#endif
#ifndef VK_RIGHT
#define VK_RIGHT 39
#endif
#ifndef VK_ESCAPE
#define VK_ESCAPE 27
#endif
#ifndef VK_RETURN
#define VK_RETURN 13
#endif

// 坐标的xy的真实方向是一个很棘手的问题，
// 查了资料之后我还是决定将x当成横的，y当成竖的，
// 这虽然不符合数学和计算机中对矩阵、数组的定义，
// 但是符合直觉，且符合图形库的规范。
struct Coord
{
    int x;
    int y;
};

// 顺手重载了一下运算符，以方便使用
// lhs 和 rhs 分别是left-hand side和 right-hand side的缩写
inline bool operator==(const Coord& lhs, const Coord& rhs) 
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
inline Coord operator+(const Coord& lhs, const Coord& rhs) 
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}
inline Coord operator-(const Coord& lhs, const Coord& rhs) 
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}


struct MapSize
{
    int width;
    int height;
};

inline constexpr int CELL_SIZE = 32;


enum class Direction 
{ 
    UP, 
    DOWN, 
    LEFT, 
    RIGHT 
};

inline Direction oppositeDir(const Direction& dir)
{

    // using Direction::UP;
    // using Direction::DOWN;
    // using Direction::LEFT;
    // using Direction::RIGHT;
    // 无语了 又是c++新版才有的特性是吧，，，，下面这样看起来不恶心么，，，

    switch (dir)
    {
        case Direction::UP:
            return Direction::DOWN;
        case Direction::DOWN:
            return Direction::UP;
        case Direction::LEFT:
            return Direction::RIGHT;
        case Direction::RIGHT:
            return Direction::LEFT;
    }
    
    // 添加默认返回值以避免警告
    return Direction::UP;
}

enum class KeyMessageType
{
    Direction,
    Others,
    Unknown
};

struct KeyEvent
{
    int key { 0 };
    KeyMessageType type { KeyMessageType::Unknown };
    unsigned int flags { 0 };
};
