#include "../include/SnakeGame/GameOverState.h"
#include "../include/SnakeGame/Game.h"
#include "../include/SnakeGame/MainMenuState.h"
#include "../include/SnakeGame/Render.h"
#include "../include/SnakeGame/Types.h"


void GameOverState::on_enter(Game* game)
{
    (void)game;
    // TODO: 初始化游戏结束界面
}

void GameOverState::on_exit(Game* game)
{
    (void)game;
    // TODO: 清理游戏结束界面
}

void GameOverState::handle_input(Game* game, const KeyEvent& key)
{
    if (key.type == KeyMessageType::Others)
    {
        // 按空格键或回车键重新开始游戏
        if (key.key == ' ' || key.key == VK_RETURN)
        {
            if (game->get_playerNum()==2)
                game->start_new_game(game->get_mode(), 2);
            else
                game->start_new_game(game->get_mode(), 1);
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
    if (game->get_playerNum() == 1)
    {
        // 绘制游戏结束界面
        draw_background(); // 后面可以试试不画
        // draw_text_centered(130, f"您获得了", 35, 0x00FF6666); 要是可以用f字符串就好了
        draw_text_centered(130, "您获得了" + std::to_string(game->get_score()) + "分", 35, 0x00FFFFFF); 
        draw_text_centered(160, "游戏结束", 24, 0x00FF6666);
        draw_text_centered(230, "按空格或回车重新开始", 14, 0x00FFFFFF);
    }
    else if (game->get_playerNum() == 2)
    {
        draw_background();
        switch (game->get_result())
        {
            case Result::SnakeWin:
                draw_text_centered(130, "Snake 1 Win!", 35, 0x00FFFFFF); 
                draw_text_centered(160, "游戏结束", 24, 0x00FF6666);
                break;
            case Result::Snake2Win:
                draw_text_centered(130, "Snake 2 Win!", 35, 0x00FFFFFF); 
                draw_text_centered(160, "游戏结束", 24, 0x00FF6666);
                break;
            case Result::Tie:
                draw_text_centered(130, "Tie!", 35, 0x00FFFFFF); 
                draw_text_centered(160, "游戏结束", 24, 0x00FF6666);
                break;
        }
    }
}
