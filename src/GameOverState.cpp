#include "../include/SnakeGame/GameOverState.h"
#include "../include/SnakeGame/Game.h"
#include "../include/SnakeGame/MainMenuState.h"
#include "../include/SnakeGame/Render.h"

void GameOverState::on_enter(Game* game)
{
    if (game)
    {
        // 保持当前蛇的状态以便可能显示统计信息
        // 目前不做额外处理
    }
}

void GameOverState::handle_input(Game* game, const KeyEvent& key)
{
    if (key.type == KeyMessageType::Others)
    {
        // 按空格键或回车键重新开始游戏
        if (key.key == ' ' || key.key == VK_RETURN)
        {
            game->start_new_game();
        }
    }
}

void GameOverState::update(Game* game)
{
    (void)game;
    // TODO: 更新游戏结束界面逻辑
}

void GameOverState::draw(Game* game)
{
    (void)game;
    // 绘制游戏结束界面
    draw_background();

    draw_text_centered(160, "游戏结束", 48, 0x00FF6666);
    draw_text_centered(230, "按空格或回车重新开始", 28, 0x00FFFFFF);
}
