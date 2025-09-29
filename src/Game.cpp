#include "../include/SnakeGame/Game.h"
#include "../include/SnakeGame/MainMenuState.h"
#include "../include/SnakeGame/PlayingState.h"
#include "../include/SnakeGame/Render.h"
// 我现在非常怀疑采取这样一种文件夹方式是否是一个好的工程实践，，，

#include <memory>
// 

Game::Game()
	: map_size(get_default_map_size())
	, snake{}
	, state_(std::make_unique<MainMenuState>())
{
	snake.reset(map_size);

	if (state_)
	{
		state_->on_enter(this);
	}
}

void Game::run()
{
	initialize(map_size);

	while (is_running())
	{
		process_events();
		update();

		begin_frame();
		render();
		end_frame(60); // 60 FPS
	}

	shutdown();
}

void Game::start_new_game()
{
	snake.reset(map_size);
	change_state(std::make_unique<PlayingState>());
}

MapSize Game::get_map_size() const
{
	return map_size;
}

void Game::change_state(std::unique_ptr<GameState> new_state)
{
	if (!new_state)
	{
		return;
	}

	if (state_)
	{
		state_->on_exit(this);
	}

	state_ = std::move(new_state);

	if (state_)
	{
		state_->on_enter(this);
	}
}

Snake& Game::get_snake()
{
	return snake;
}

const Snake& Game::get_snake() const
{
	return snake;
}

void Game::process_events()
{
	while (auto key = poll_key_event())
	{
		if (state_)
		{
			state_->handle_input(this, *key);
		}
	}
}

void Game::update()
{
	if (state_)
	{
		state_->update(this);
	}
}

void Game::render()
{
	if (state_)
	{
		state_->draw(this);
	}
}
