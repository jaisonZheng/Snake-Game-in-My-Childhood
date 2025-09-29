#pragma once

#include <optional>
#include <deque>
#include <string>

#include "Types.h"

void initialize(const MapSize& size);
void shutdown();

bool is_running();

void begin_frame();
void end_frame(int target_fps);

std::optional<KeyEvent> poll_key_event();

void draw_background();
void draw_apple(const Coord& position);
void draw_bonus(const Coord& position);
void draw_snake_head(const Coord& position, const Direction& direction);
void draw_snake_body(const std::deque<Coord>& body, const Coord& head_position);
void draw_wall(const Coord& position);

void draw_text_centered(int y, const std::string& text, int font_size, unsigned int color);
void draw_hud(int score, float bonus_time_left);
