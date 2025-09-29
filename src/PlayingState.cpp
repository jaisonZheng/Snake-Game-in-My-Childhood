#include "../include/SnakeGame/PlayingState.h"
#include "../include/SnakeGame/Game.h"
#include "../include/SnakeGame/GameOverState.h"
#include "../include/SnakeGame/MainMenuState.h"
#include "../include/SnakeGame/Render.h"
#include <random>
#include <algorithm>

PlayingState::PlayingState() 
    : game_speed(10), frame_counter(0)
{
    // 初始化苹果位置
    apple_position = {10, 10};
}

void PlayingState::on_enter(Game* game)
{
    frame_counter = 0;

    if (!game)
    {
        return;
    }

    walls.clear();

    MapSize map_size = game->get_map_size();
    generate_walls(map_size);

    Snake& snake = game->get_snake();
    generate_apple(map_size, snake);
}

void PlayingState::handle_input(Game* game, const KeyEvent& key)
{
    Snake& snake = game->get_snake();
    
    if (key.type == KeyMessageType::Direction)
    {
        // 处理WASD和方向键输入
        switch (key.key)
        {
            case 'W':
            case 'w':
            case VK_UP:
                snake.turn(Direction::UP);
                break;
            case 'S':
            case 's':
            case VK_DOWN:
                snake.turn(Direction::DOWN);
                break;
            case 'A':
            case 'a':
            case VK_LEFT:
                snake.turn(Direction::LEFT);
                break;
            case 'D':
            case 'd':
            case VK_RIGHT:
                snake.turn(Direction::RIGHT);
                break;
        }
    }
    else if (key.type == KeyMessageType::Others)
    {
        // 处理其他按键
        switch (key.key)
        {
            case VK_ESCAPE:
                // 返回主菜单
                game->change_state(std::make_unique<MainMenuState>());
                break;
        }
    }
}

void PlayingState::update(Game* game)
{
    // 在第一次更新时生成墙壁
    if (walls.empty())
    {
        generate_walls(game->get_map_size());
    }
    
    frame_counter++;
    
    // 控制游戏速度
    if (frame_counter < 60 / game_speed)
        return;
        
    frame_counter = 0;
    
    Snake& snake = game->get_snake();
    MapSize map_size = game->get_map_size();
    
    // 检查是否吃到苹果
    bool eating_apple = false;
    if (snake.get_head() == apple_position)
    {
    eating_apple = true;
    // 生成新的苹果位置
    generate_apple(map_size, snake);
    }
    
    // 移动蛇
    snake.move(map_size, eating_apple);
    
    // 检查游戏结束条件
    if (snake.is_crash_itself())
    {
        // 游戏结束，切换到游戏结束状态
        game->change_state(std::make_unique<GameOverState>());
    }
}

void PlayingState::draw(Game* game)
{
    // 绘制背景
    draw_background();
    
    // 绘制墙壁
    for (const auto& wall : walls)
    {
        draw_wall(wall);
    }
    
    // 绘制苹果
    draw_apple(apple_position);
    
    // 绘制蛇
    Snake& snake = game->get_snake();
    draw_snake_head(snake.get_head(), snake.get_direction());
    draw_snake_body(snake.get_body());
}

void PlayingState::generate_apple(const MapSize& map_size, const Snake& snake)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    const int max_x = std::max(1, map_size.width / CELL_SIZE);
    const int max_y = std::max(1, map_size.height / CELL_SIZE);

    if (max_x <= 2 || max_y <= 2)
    {
        apple_position = { 1, 1 };
        return;
    }

    std::uniform_int_distribution<> dis_x(1, max_x - 2);
    std::uniform_int_distribution<> dis_y(1, max_y - 2);
    
    Coord new_pos;
    do
    {
        new_pos = {dis_x(gen), dis_y(gen)};
    } while (!is_position_valid(new_pos, map_size, snake));
    
    apple_position = new_pos;
}

void PlayingState::generate_walls(const MapSize& map_size)
{
    walls.clear();
    
    // 在边界生成墙壁
    const int max_x = std::max(1, map_size.width / CELL_SIZE);
    const int max_y = std::max(1, map_size.height / CELL_SIZE);

    if (max_x <= 0 || max_y <= 0)
    {
        return;
    }

    for (int x = 0; x < max_x; ++x)
    {
        walls.push_back({x, 0}); // 上边界
        walls.push_back({x, max_y - 1}); // 下边界
    }
    
    for (int y = 0; y < max_y; ++y)
    {
        walls.push_back({0, y}); // 左边界
        walls.push_back({max_x - 1, y}); // 右边界
    }
}

bool PlayingState::is_position_valid(const Coord& pos, const MapSize& map_size, const Snake& snake) const
{
    // 检查是否在边界内
    const int max_x = std::max(1, map_size.width / CELL_SIZE);
    const int max_y = std::max(1, map_size.height / CELL_SIZE);

    if (pos.x <= 0 || pos.x >= max_x - 1 || 
        pos.y <= 0 || pos.y >= max_y - 1)
        return false;
    
    // 检查是否与墙壁重叠
    for (const auto& wall : walls)
    {
        if (pos == wall)
            return false;
    }

    if (pos == snake.get_head())
    {
        return false;
    }

    for (const auto& segment : snake.get_body())
    {
        if (pos == segment)
        {
            return false;
        }
    }
    
    return true;
}
