#include "../include/SnakeGame/PausedState.h"
#include "../include/SnakeGame/Game.h"
#include "../include/SnakeGame/MainMenuState.h"
#include "../include/SnakeGame/Render.h"

void PausedState::on_enter(Game* game)
{
    (void)game;
}

void PausedState::handle_input(Game* game, const KeyEvent& key)
{
    if (key.type == KeyMessageType::Others)
    {
        // 按ESC键继续游戏
        if (key.key == VK_ESCAPE) // ESC键
        {
            game->change_state(std::make_unique<MainMenuState>());
        }
    }
}

void PausedState::update(Game* game)
{
    (void)game;
    // TODO: 更新暂停状态下的逻辑（通常为空）
}

void PausedState::draw(Game* game)
{
    (void)game;
    // 绘制暂停界面
    draw_background();

    draw_text_centered(200, "游戏已暂停", 18, 0x00FFFFFF);
    draw_text_centered(260, "按 ESC 返回主菜单", 12, 0x00FFFF66);
}
