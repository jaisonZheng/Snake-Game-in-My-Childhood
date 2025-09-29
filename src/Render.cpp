#include "../include/SnakeGame/Render.h"
//#include "../ege/include/graphics.h"
#include <graphics.h>
#include <filesystem>
#include <string>
#include <stdexcept>
#include <array>
#include <algorithm>
#include <cstdlib>
#include <utility>
#include <sstream>
#include <iomanip>

// 所有跟ege这个垃圾图形库有关的东西全部在这个文件中搞定。
// 这个图形库真的是依托答辩，我实在是搞不懂配环境就要配6h+的垃圾图形库怎么会有人用。
// 还有这个狗屎image显示 显示png的透明部分都这么麻烦，，，，

namespace
{
	bool g_initialized = false;

	constexpr unsigned char OPAQUE_ALPHA = 0xFF;

	// 图片资源
	PIMAGE g_bg_image = nullptr;
	PIMAGE g_apple_image = nullptr;
	PIMAGE g_snake_head_image = nullptr;
	PIMAGE g_snake_body_image = nullptr;
	PIMAGE g_wall_image = nullptr;

	color_t g_apple_key = 0;
	void release_snake_variants(); // Forward declaration
	std::array<PIMAGE, 4> g_snake_head_variants{};
	std::array<PIMAGE, 4> g_snake_body_variants{};

	color_t g_snake_head_key = 0;
	color_t g_snake_body_key = 0;
	color_t g_wall_key = 0;

	int g_bg_width = 0;
	int g_bg_height = 0;
	int g_apple_width = CELL_SIZE;
	int g_apple_height = CELL_SIZE;
	int g_snake_head_width = CELL_SIZE;
	int g_snake_head_height = CELL_SIZE;
	int g_snake_body_width = CELL_SIZE;
	int g_snake_body_height = CELL_SIZE;
	int g_wall_width = CELL_SIZE;
	int g_wall_height = CELL_SIZE;

	std::string resolve_asset_path(const std::string& filename)
	{
		namespace fs = std::filesystem;
		fs::path local = fs::current_path() / "images" / filename;
		if (fs::exists(local))
		{
			return local.string();
		}

		fs::path fallback = fs::path("..") / "images" / filename;
		if (fs::exists(fallback))
		{
			return fallback.string();
		}

		return filename; // 交给图形库尝试
	}

	PIMAGE load_image(const std::string& filename)
	{
		PIMAGE image = newimage();
		std::string full_path = resolve_asset_path(filename);
		if (getimage(image, full_path.c_str()) != grOk)
		{
			delimage(image);
			throw std::runtime_error("Failed to load asset: " + full_path);
		}
		return image;
	}

	int cell_to_pixel_centered(int cell_value, int image_size)
	{
		return cell_value * CELL_SIZE + (CELL_SIZE - image_size) / 2;
	}

	int direction_to_index(Direction dir)
	{
		switch (dir)
		{
		case Direction::UP:
			return 0;
		case Direction::DOWN:
			return 1;
		case Direction::LEFT:
			return 2;
		case Direction::RIGHT:
			return 3;
		}
		return 0;
	}

	std::pair<int, int> rotate_point(int x, int y, Direction dir, int size)
	{
		switch (dir)
		{
		case Direction::DOWN:
			return { x, y };
		case Direction::UP:
			return { size - 1 - x, size - 1 - y };
		case Direction::LEFT:
			return { size - 1 - y, x };
		case Direction::RIGHT:
			return { y, size - 1 - x };
		}
		return { x, y };
	}

	PIMAGE make_oriented_image(PIMAGE src, color_t transparent, Direction dir, int width, int height)
	{
		const int size = std::min(width, height);
		PIMAGE dst = newimage(size, size);
		for (int py = 0; py < size; ++py)
		{
			for (int px = 0; px < size; ++px)
			{
				putpixel(px, py, transparent, dst);
			}
		}

		for (int sy = 0; sy < size; ++sy)
		{
			for (int sx = 0; sx < size; ++sx)
			{
				color_t color = getpixel(sx, sy, src);
				if (color == transparent)
				{
					continue;
				}
				auto [dx, dy] = rotate_point(sx, sy, dir, size);
				putpixel(dx, dy, color, dst);
			}
		}

		return dst;
	}

	int wrap_delta(int delta, int max_value)
	{
		if (delta > max_value / 2)
		{
			delta -= max_value;
		}
		if (delta < -max_value / 2)
		{
			delta += max_value;
		}
		return delta;
	}

	Direction direction_from_points(const Coord& from, const Coord& to)
	{
		int dx = wrap_delta(to.x - from.x, TILE_COUNT.width);
		int dy = wrap_delta(to.y - from.y, TILE_COUNT.height);

		if (std::abs(dx) >= std::abs(dy))
		{
			if (dx > 0)
			{
				return Direction::RIGHT;
			}
			if (dx < 0)
			{
				return Direction::LEFT;
			}
		}
		if (dy > 0)
		{
			return Direction::DOWN;
		}
		if (dy < 0)
		{
			return Direction::UP;
		}
		return Direction::DOWN;
	}

	void build_snake_variants()
	{

		if (g_snake_head_image)
		{
			g_snake_head_key = getpixel(0, 0, g_snake_head_image);
			g_snake_head_width = getwidth(g_snake_head_image);
			g_snake_head_height = getheight(g_snake_head_image);

			g_snake_head_variants[direction_to_index(Direction::DOWN)] = g_snake_head_image;
			g_snake_head_variants[direction_to_index(Direction::UP)] = make_oriented_image(g_snake_head_image, g_snake_head_key, Direction::UP, g_snake_head_width, g_snake_head_height);
			g_snake_head_variants[direction_to_index(Direction::LEFT)] = make_oriented_image(g_snake_head_image, g_snake_head_key, Direction::LEFT, g_snake_head_width, g_snake_head_height);
			g_snake_head_variants[direction_to_index(Direction::RIGHT)] = make_oriented_image(g_snake_head_image, g_snake_head_key, Direction::RIGHT, g_snake_head_width, g_snake_head_height);

			for (Direction dir : { Direction::UP, Direction::LEFT, Direction::RIGHT })
			{
				int idx = direction_to_index(dir);
				if (!g_snake_head_variants[idx])
				{
					g_snake_head_variants[idx] = g_snake_head_image;
				}
			}
		}

		if (g_snake_body_image)
		{
			g_snake_body_key = getpixel(0, 0, g_snake_body_image);
			g_snake_body_width = getwidth(g_snake_body_image);
			g_snake_body_height = getheight(g_snake_body_image);

			g_snake_body_variants[direction_to_index(Direction::DOWN)] = g_snake_body_image;
			g_snake_body_variants[direction_to_index(Direction::UP)] = make_oriented_image(g_snake_body_image, g_snake_body_key, Direction::UP, g_snake_body_width, g_snake_body_height);
			g_snake_body_variants[direction_to_index(Direction::LEFT)] = make_oriented_image(g_snake_body_image, g_snake_body_key, Direction::LEFT, g_snake_body_width, g_snake_body_height);
			g_snake_body_variants[direction_to_index(Direction::RIGHT)] = make_oriented_image(g_snake_body_image, g_snake_body_key, Direction::RIGHT, g_snake_body_width, g_snake_body_height);

			for (Direction dir : { Direction::UP, Direction::LEFT, Direction::RIGHT })
			{
				int idx = direction_to_index(dir);
				if (!g_snake_body_variants[idx])
				{
					g_snake_body_variants[idx] = g_snake_body_image;
				}
			}
		}
	}

	void release_snake_variants()
	{
		const auto release = [](std::array<PIMAGE, 4>& variants, PIMAGE base)
		{
			for (auto& img : variants)
			{
				if (img && img != base)
				{
					delimage(img);
				}
				img = nullptr;
			}
		};

		release(g_snake_head_variants, g_snake_head_image);
		release(g_snake_body_variants, g_snake_body_image);
	}
}

void initialize(const MapSize& size)
{
	if (g_initialized)
	{
		return;
	}

	initgraph(size.width, size.height, INIT_DEFAULT);
	setrendermode(RENDER_MANUAL); // 手动刷新
	setbkcolor(EGERGB(0, 0, 0));

	try
	{
		g_bg_image = load_image("bg_ap.png");
		g_apple_image = load_image("apple-sheet0.png");
		g_snake_head_image = load_image("snake_lock_head-sheet0.png");
		g_snake_body_image = load_image("snake_body-sheet0.png");
		g_wall_image = load_image("tbstaticwall.png");

		if (g_bg_image)
		{
			g_bg_width = getwidth(g_bg_image);
			g_bg_height = getheight(g_bg_image);
		}

		if (g_apple_image)
		{
			g_apple_width = getwidth(g_apple_image);
			g_apple_height = getheight(g_apple_image);
			g_apple_key = getpixel(0, 0, g_apple_image);
		}

		if (g_snake_head_image)
		{
			g_snake_head_width = getwidth(g_snake_head_image);
			g_snake_head_height = getheight(g_snake_head_image);
		}

		if (g_snake_body_image)
		{
			g_snake_body_width = getwidth(g_snake_body_image);
			g_snake_body_height = getheight(g_snake_body_image);
		}

		if (g_wall_image)
		{
			g_wall_width = getwidth(g_wall_image);
			g_wall_height = getheight(g_wall_image);
			g_wall_key = getpixel(0, 0, g_wall_image);
		}

		build_snake_variants();
	}
	catch (const std::exception&)
	{
		shutdown();
		throw;
	}
	
	g_initialized = true;
}

void shutdown()
{
	if (!g_initialized)
	{
		return;
	}

	// 释放图片资源
	release_snake_variants();
	if (g_bg_image)
	{
		delimage(g_bg_image);
		g_bg_image = nullptr;
	}
	if (g_apple_image)
	{
		delimage(g_apple_image);
		g_apple_image = nullptr;
		g_apple_key = 0;
	}
	if (g_snake_head_image)
	{
		delimage(g_snake_head_image);
		g_snake_head_image = nullptr;
	}
	if (g_snake_body_image)
	{
		delimage(g_snake_body_image);
		g_snake_body_image = nullptr;
	}
	if (g_wall_image)
	{
		delimage(g_wall_image);
		g_wall_image = nullptr;
		g_wall_key = 0;
	}

	closegraph();
	g_initialized = false;
}

bool is_running()
{
	return g_initialized && is_run();
}

void begin_frame()
{
	if (!g_initialized)
	{
		return;
	}

	cleardevice();
}

void end_frame(int target_fps)
{
	if (!g_initialized)
	{
		return;
	}

	delay_fps(target_fps);
}

std::optional<KeyEvent> poll_key_event()
{
	if (!g_initialized || !kbmsg())
	{
		return std::nullopt;
	}

	const key_msg msg = getkey();
	if (msg.msg != key_msg_down && msg.msg != key_msg_char)
	{
		return std::nullopt;
	}

	KeyEvent event{};
	event.key = msg.key;
	event.flags = msg.flags;

	// 根据按键类型分类
	switch (msg.key)
	{
	case 'W':
	case 'w':
	case 'A':
	case 'a':
	case 'S':
	case 's':
	case 'D':
	case 'd':
	case VK_LEFT:
	case VK_UP:
	case VK_RIGHT:
	case VK_DOWN:
		event.type = KeyMessageType::Direction;
		break;
	case VK_ESCAPE:
	case ' ': // 空格键
	case VK_RETURN: // 回车键
	case 'r': // 模式鍵
		event.type = KeyMessageType::Others;
		break;
	default:
		event.type = KeyMessageType::Unknown;
		break;
	}

	return event;
}

// 绘制背景
void draw_background()
{
	if (g_bg_image)
	{
		int x = (getwidth() - g_bg_width) / 2;
		int y = (getheight() - g_bg_height) / 2;
		putimage(x, y, g_bg_image);
	}
}

// 绘制苹果
void draw_apple(const Coord& position)
{
	if (!g_apple_image)
	{
		return;
	}

	const int tile_left = position.x * CELL_SIZE;
	int x = tile_left + (CELL_SIZE - g_apple_width) / 2;
	const int tile_bottom = (position.y + 1) * CELL_SIZE;
	int y = tile_bottom - g_apple_height;
	putimage_transparent(nullptr, g_apple_image, x, y, g_apple_key);
}

void draw_bonus(const Coord& position)
{
	const int center_x = position.x * CELL_SIZE + CELL_SIZE / 2;
	const int center_y = position.y * CELL_SIZE + CELL_SIZE / 2;
	const int radius = std::max(2, CELL_SIZE / 2 - 2);

	setcolor(EGERGB(255, 0, 0));
// 	void solidellipse(
//     int x,
//     int y,
//     int xradius,
//     int yradius,
//     PIMAGE pimg = NULL
// 	);
//  不敢睁开眼 以为是幻觉，，，，居然连这玩意都没有吗 那我怎么画填充圆？
// 人机ege
	circle(center_x, center_y, radius);
}

// 绘制蛇头
void draw_snake_head(const Coord& position, const Direction& direction)
{
	if (!g_snake_head_image)
	{
		return;
	}

	const int index = direction_to_index(direction);
	PIMAGE oriented = g_snake_head_variants[index] ? g_snake_head_variants[index] : g_snake_head_image;
	int x = cell_to_pixel_centered(position.x, g_snake_head_width);
	int y = cell_to_pixel_centered(position.y, g_snake_head_height);
	putimage_alphatransparent(nullptr, oriented, x, y, g_snake_head_key, OPAQUE_ALPHA);
}

// 绘制蛇身
void draw_snake_body(const std::deque<Coord>& body, const Coord& head)
{
	if (!g_snake_body_image || body.empty())
	{
		return;
	}

	for (size_t i = 0; i < body.size(); ++i)
	{
		const Coord& segment = body[i];
		const Coord& reference = (i == 0) ? head : body[i - 1];
		Direction dir = direction_from_points(segment, reference);
		const int index = direction_to_index(dir);
		PIMAGE oriented = g_snake_body_variants[index] ? g_snake_body_variants[index] : g_snake_body_image;
		if (!oriented)
		{
			continue;
		}

		int x = cell_to_pixel_centered(segment.x, g_snake_body_width);
		int y = cell_to_pixel_centered(segment.y, g_snake_body_height);
		putimage_alphatransparent(nullptr, oriented, x, y, g_snake_body_key, OPAQUE_ALPHA);
	}
}

// 绘制墙壁
void draw_wall(const Coord& position)
{
	const int tile_left = position.x * CELL_SIZE;
	const int tile_top = position.y * CELL_SIZE;

	const int draw_x = tile_left + (CELL_SIZE - g_wall_width) / 2;
	const int draw_y = tile_top + (CELL_SIZE - g_wall_height) / 2;
	putimage_transparent(nullptr, g_wall_image, draw_x, draw_y, g_wall_key);
	return;
	
}

void draw_text_centered(int y, const std::string& text, int font_size, unsigned int color)
{
	setbkmode(TRANSPARENT);
	setcolor(color);
	setfont(font_size, 0, "Consolas");
	int text_width = textwidth(text.c_str());
	int x = (getwidth() - text_width) / 2;
	outtextxy(x, y, text.c_str());
}

void draw_hud(int score, float bonus_time_left)
{
	setbkmode(TRANSPARENT);
	setfont(16, 0, "Consolas");
	setcolor(EGERGB(255, 255, 255));

	std::ostringstream score_stream;
	score_stream << "Score: " << score;
	const std::string score_text = score_stream.str();
	outtextxy(10, 10, score_text.c_str());

	std::ostringstream timer_stream;
	if (bonus_time_left > 0.0f)
	{
		timer_stream << "Bonus: " << std::fixed << std::setprecision(1) << bonus_time_left << "s";
	}
	else
	{
		timer_stream << "Bonus: --";
	}

	const std::string timer_text = timer_stream.str();
	int timer_width = textwidth(timer_text.c_str());
	int timer_x = getwidth() - timer_width - 10;
	outtextxy(timer_x, 10, timer_text.c_str());
}
