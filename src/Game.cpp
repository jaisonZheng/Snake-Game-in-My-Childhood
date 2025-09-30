#include "../include/SnakeGame/Game.h"
#include "../include/SnakeGame/MainMenuState.h"
#include "../include/SnakeGame/PlayingState.h"
#include "../include/SnakeGame/Render.h"
#include <algorithm>
#include <cmath>
// 
#include <memory>

// 我决定给自己上一门课：
// 叫做：当你自己做AI式的增删查改工作，是什么样的感受？
// 嗯。就这样。
// 给当前游戏加一个双人功能。

Game::Game() : state_(new MainMenuState()), map_size{240, 320}
{
	reset_progress();

	snake.reset(Coord {3, 5}, Direction:: RIGHT);
	if (playerNum_ == 2)
		snake2.reset(Coord {8, 10}, Direction:: LEFT);

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

void Game::start_new_game(const Mode& mode, int playerNum)
{
	playerNum_ = playerNum;
	mode_ = mode;
	reset_progress();
	snake.reset(Coord {3, 5}, Direction:: RIGHT);
	if (playerNum_ == 2)
		snake2.reset(Coord {8, 10}, Direction:: LEFT);
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

int Game::get_score() const
{
	return score_;
}

void Game::on_apple_collected(bool is_snake2 = false)
{
	++total_apples_collected_;
	++apples_since_last_bonus_;
	if (!is_snake2)
		score_ += 1;
	else
		score2_ += 1;

	if (!bonus_position_ && !bonus_spawn_pending_ && apples_since_last_bonus_ >= bonus_required_apples_)
	{
		bonus_spawn_pending_ = true;
	}
}

bool Game::should_spawn_bonus() const
{
	return bonus_spawn_pending_ && !bonus_position_;
}

void Game::bonus_spawned(const Coord& position)
{
	bonus_position_ = position;
	bonus_spawn_pending_ = false;
	bonus_timer_ = BONUS_DURATION_SECONDS;
	apples_since_last_bonus_ = 0;
}

void Game::on_bonus_collected(bool is_snake2 = false)
{
	if (!bonus_position_)
	{
		return;
	}

	const float remaining_time = std::max(0.0f, bonus_timer_);
	const int bonus_score = static_cast<int>(std::lround(static_cast<double>(remaining_time) * 5.0));
	if (!is_snake2)
		score_ += bonus_score;
	else
		score2_ += bonus_score;

	bonus_position_.reset();
	bonus_timer_ = 0.0f;
	bonus_required_apples_ = BONUS_REDUCED_THRESHOLD;
	bonus_spawn_pending_ = false;
	apples_since_last_bonus_ = 0;
}

bool Game::has_active_bonus() const
{
	return bonus_position_.has_value();
}

std::optional<Coord> Game::get_bonus_position() const
{
	return bonus_position_;
}

float Game::get_bonus_time_remaining() const
{
	if (!bonus_position_)
	{
		return 0.0f;
	}

	return std::max(0.0f, bonus_timer_);
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
	auto now = std::chrono::steady_clock::now();
	float delta_seconds = 0.0f;
	if (first_update_)
	{
		first_update_ = false;
	}
	else
	{
		delta_seconds = std::chrono::duration<float>(now - last_update_time_).count();
	}
	last_update_time_ = now;

	update_bonus_timer(delta_seconds);

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

void Game::reset_progress()
{
	score_ = 0;
	total_apples_collected_ = 0;
	apples_since_last_bonus_ = 0;
	bonus_required_apples_ = BONUS_DEFAULT_THRESHOLD;
	bonus_spawn_pending_ = false;
	bonus_timer_ = 0.0f;
	bonus_position_.reset();
	last_update_time_ = std::chrono::steady_clock::now();
	first_update_ = true;
}

void Game::update_bonus_timer(float delta_seconds)
{
	if (!bonus_position_ || delta_seconds <= 0.0f)
	{
		return;
	}

	bonus_timer_ = std::max(0.0f, bonus_timer_ - delta_seconds);
	if (bonus_timer_ <= 0.0f)
	{
		bonus_position_.reset();
		bonus_timer_ = 0.0f;
		bonus_required_apples_ = BONUS_DEFAULT_THRESHOLD;
		bonus_spawn_pending_ = false;
		if (apples_since_last_bonus_ >= bonus_required_apples_)
		{
			bonus_spawn_pending_ = true;
		}
	}
}

Mode Game::get_mode()
{
	return mode_;
}

Snake& Game::get_snake2()
{
	return snake2;
}

int Game::get_playerNum()
{
	return playerNum_;
}

void Game::set_result(Result result)
{
	result_ = result;
}

Result Game::get_result()
{
	return result_;
}

int Game::get_score2()
{
	return score2_;
}

int Game::get_score()
{
	return score_;
}