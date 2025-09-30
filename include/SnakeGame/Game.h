#pragma once

#include <memory>
#include <optional>
#include <chrono>

#include "GameState.h"
#include "Snake.h"
#include "Reward.h"
#include "Types.h"

class Game
{
public:
    Game();

    void run();
    void start_new_game(const Mode& mode, int playerNum = 1);

    MapSize get_map_size() const;
    void change_state(std::unique_ptr<GameState> new_state);
    Snake& get_snake();
    const Snake& get_snake() const;

    int get_score() const;
    void on_apple_collected(bool is_snake2);
    bool should_spawn_bonus() const;
    void bonus_spawned(const Coord& position);
    void on_bonus_collected(bool is_snake2);
    bool has_active_bonus() const;
    std::optional<Coord> get_bonus_position() const;
    float get_bonus_time_remaining() const;
    Mode get_mode();
    Snake& get_snake2();
    int get_playerNum();
    void set_result(Result result);
    Result get_result();
    int get_score();
    int get_score2();

private:
    void process_events();
    void update();
    void render();
    void reset_progress();
    void update_bonus_timer(float delta_seconds);

    int playerNum_ = 1;
    Result result_;
    MapSize map_size;
    Snake snake;
    Snake snake2;
    std::unique_ptr<GameState> state_;

    Mode mode_ = Mode::Railways;

    int score_;
    int score2_;
    int total_apples_collected_;
    int apples_since_last_bonus_;
    int bonus_required_apples_;
    bool bonus_spawn_pending_;
    float bonus_timer_;
    std::optional<Coord> bonus_position_;
    std::chrono::steady_clock::time_point last_update_time_;
    bool first_update_;

    static constexpr int BONUS_DEFAULT_THRESHOLD = 10;
    static constexpr int BONUS_REDUCED_THRESHOLD = 5;
    static constexpr float BONUS_DURATION_SECONDS = 6.0f;
};
