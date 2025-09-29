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
    generate_walls(game->get_mode());

    Snake& snake = game->get_snake();
    generate_apple(game, map_size, snake);
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
	if (!game)
	{
		return;
	}

    // 在第一次更新时生成墙壁
    if (walls.empty())
    {
        generate_walls(game->get_mode());
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
        game->on_apple_collected();
        // 生成新的苹果位置
        generate_apple(game, map_size, snake);
    }

    if (auto bonus_position = game->get_bonus_position())
    {
        if (snake.get_head() == *bonus_position)
        {
            game->on_bonus_collected();
        }
    }

    if (game->should_spawn_bonus())
    {
        generate_bonus(game, map_size, snake);
    }
    
    // 移动蛇
    snake.move(map_size, eating_apple);
    
    // 检查游戏结束条件
    if (snake.is_crash_itself())
    {
        // 游戏结束，切换到游戏结束状态
        game->change_state(std::make_unique<GameOverState>());
    }
    
    // 在这里写撞墙逻辑 因为这是唯一既有墙信息又能拿到snake信息的地方。
    for (Coord& wall : walls)
    {
        if (snake.get_head() == wall)
            game->change_state(std::make_unique<GameOverState>());
    }
}

void PlayingState::draw(Game* game)
{
    if (!game)
    {
        return;
    }

    // 绘制背景
    draw_background();
    
    // 绘制墙壁
    for (const Coord& wall : walls)
    {
        draw_wall(wall);
    }

    // 绘制苹果
    draw_apple(apple_position);

    if (auto bonus_position = game->get_bonus_position())
    {
        draw_bonus(*bonus_position);
    }

    draw_hud(game->get_score(), game->get_bonus_time_remaining());
    
    // 绘制蛇
    Snake& snake = game->get_snake();
    draw_snake_head(snake.get_head(), snake.get_direction());
    draw_snake_body(snake.get_body(), snake.get_head());
}

void PlayingState::generate_apple(Game* game, const MapSize& map_size, const Snake& snake)
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
    
    Coord new_pos{};
    bool found = false;
    int attempts = 0;
    const int max_attempts = 1024;

    while (!found && attempts < max_attempts)
    {
        new_pos = { dis_x(gen), dis_y(gen) };
        ++attempts;

        bool conflicts_bonus = false;
        if (game)
        {
            if (auto bonus_position = game->get_bonus_position())
            {
                conflicts_bonus = (new_pos == *bonus_position);
            }
        }

        if (new_pos == snake.get_head())
        {
            continue;
        }

        if (!conflicts_bonus && is_position_valid(new_pos, map_size, snake))
        {
            found = true;
        }
    }

    if (found)
    {
        apple_position = new_pos;
        return;
    }

    // fallback: search entire grid sequentially
    for (int y = 1; y < max_y - 1 && !found; ++y)
    {
        for (int x = 1; x < max_x - 1 && !found; ++x)
        {
            Coord fallback_pos{ x, y };
            bool conflicts_bonus = false;
            if (game)
            {
                if (auto bonus_position = game->get_bonus_position())
                {
                    conflicts_bonus = (fallback_pos == *bonus_position);
                }
            }

            if (fallback_pos == snake.get_head())
            {
                continue;
            }

            if (!conflicts_bonus && is_position_valid(fallback_pos, map_size, snake))
            {
                apple_position = fallback_pos;
                found = true;
            }
        }
    }

    if (!found)
    {
        apple_position = { 1, 1 };
    }
}

void PlayingState::generate_bonus(Game* game, const MapSize& map_size, const Snake& snake)
{
    if (!game)
    {
        return;
    }

    if (!game->should_spawn_bonus())
    {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    const int max_x = std::max(1, map_size.width / CELL_SIZE);
    const int max_y = std::max(1, map_size.height / CELL_SIZE);

    if (max_x <= 2 || max_y <= 2)
    {
        return;
    }

    std::uniform_int_distribution<> dis_x(1, max_x - 2);
    std::uniform_int_distribution<> dis_y(1, max_y - 2);

    Coord new_pos{};
    bool found = false;
    int attempts = 0;
    const int max_attempts = 1024;

    while (!found && attempts < max_attempts)
    {
        new_pos = { dis_x(gen), dis_y(gen) };
        ++attempts;

        if (new_pos == apple_position)
        {
            continue;
        }

        if (!is_position_valid(new_pos, map_size, snake))
        {
            continue;
        }

        found = true;
    }

    if (found)
    {
        game->bonus_spawned(new_pos);
    }
}

void PlayingState::generate_walls(Mode mode)
{
    walls.clear();
    
    if (mode == Mode::Railways)
    {
        walls = {
            // 前两行是左上角的，后两行是右下角的。
            {7,0}, {7,1}, {7,2}, {7,3}, {7,4},
            {6,4}, {5,4}, {4,4}, {3,4}, {2,4}, {1,4}, {0,4},
            {4,15},{4,14},{4,13},{4,12},{4,11},
            {5,11},{6,11},{7,11},{8,11},{9,11},{10,11},{11,11}
            
        };
    }
}

bool PlayingState::is_position_valid(const Coord& pos, const MapSize& map_size, const Snake& snake) const
{    
    const int max_x = std::max(1, map_size.width / CELL_SIZE);
    const int max_y = std::max(1, map_size.height / CELL_SIZE);

    if (pos.x <= 0 || pos.x >= max_x - 1 ||
        pos.y <= 0 || pos.y >= max_y - 1)
    {
        return false;
    }

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
