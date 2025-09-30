#include "../include/SnakeGame/MainMenuState.h"
#include "../include/SnakeGame/Game.h"
#include "../include/SnakeGame/Render.h"

void MainMenuState::on_enter(Game* game)
{
    if (game)
    {
        game->get_snake().reset(Coord {3, 5}, Direction:: RIGHT);
        if (game -> get_playerNum() == 2)
            game->get_snake2().reset(Coord {8, 10}, Direction:: LEFT);
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
        game->start_new_game(Mode::Classics);
        return;
    }

    if (key.type == KeyMessageType::Others)
    {
        // 按空格键或回车键开始游戏
        if (key.key == ' ' || key.key == VK_RETURN)
        {
            game->start_new_game(Mode::Classics);
        }
        if (key.key == 'r')
        {
            game->start_new_game(Mode::Railways);
        }
        if (key.key == '2')
        {
            game->start_new_game(Mode::Classics, 2);
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

    draw_text_centered(140, "贪吃蛇", 24, 0x00FFFFFF);
    draw_text_centered(220, "按空格、回车或方向键开始", 14, 0x00FFFF66);
    draw_text_centered(240, "按r键进入Railway地图", 14, 0x00FFFF66);
    draw_text_centered(260, "按2键进入双蛇模式", 14, 0x00FFFF66);
    draw_text_centered(280, "WASD / 方向键移动，ESC 暂停", 12, 0x00FFFFFF);

}
