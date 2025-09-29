#include "../include/SnakeGame/MainMenuState.h"
#include "../include/SnakeGame/Game.h"
#include "../include/SnakeGame/Render.h"

void MainMenuState::on_enter(Game* game)
{
    if (game)
    {
        game->get_snake().reset(game->get_map_size());
    }
}

void MainMenuState::handle_input(Game* game, const KeyEvent& key)
{
    if (!game)
    {
        return;
    }

    if (key.type == KeyMessageType::Direction)
    {
        game->start_new_game();
        return;
    }

    if (key.type == KeyMessageType::Others)
    {
        // 按空格键或回车键开始游戏
        if (key.key == ' ' || key.key == VK_RETURN)
        {
            game->start_new_game();
        }
    }
}

void MainMenuState::update(Game* game)
{
    (void)game;
    // TODO: 更新主菜单界面逻辑
}

void MainMenuState::draw(Game* game)
{
    (void)game;
    // 绘制主菜单界面
    draw_background();

    draw_text_centered(140, "贪吃蛇", 48, 0x00FFFFFF);
    draw_text_centered(220, "按空格、回车或方向键开始", 28, 0x00FFFF66);
    draw_text_centered(280, "WASD / 方向键移动，ESC 暂停", 24, 0x00FFFFFF);
}
