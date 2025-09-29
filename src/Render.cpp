#include "../include/SnakeGame/Render.h"
//#include "../ege/include/graphics.h"
#include <graphics.h>
#include <filesystem>
#include <string>
#include <stdexcept>



namespace
{
	bool g_initialized = false;
	
	// 图片资源
	PIMAGE g_bg_image = nullptr;
	PIMAGE g_apple_image = nullptr;
	PIMAGE g_snake_head_image = nullptr;
	PIMAGE g_snake_body_image = nullptr;
	PIMAGE g_wall_image = nullptr;

	constexpr int SNAKE_BODY_FRAMES = 9;
	constexpr int SNAKE_BODY_SIZE = CELL_SIZE;

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
}

	MapSize get_default_map_size()
	{
		PIMAGE temp = load_image("bg_ap.png");
		MapSize size{ getwidth(temp), getheight(temp) };
		delimage(temp);
		return size;
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
	if (g_bg_image) delimage(g_bg_image);
	if (g_apple_image) delimage(g_apple_image);
	if (g_snake_head_image) delimage(g_snake_head_image);
	if (g_snake_body_image) delimage(g_snake_body_image);
	if (g_wall_image) delimage(g_wall_image);

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
		putimage(0, 0, g_bg_image);
	}
}

// 绘制苹果
void draw_apple(const Coord& position)
{
	if (g_apple_image)
	{
		putimage(position.x * CELL_SIZE, position.y * CELL_SIZE, g_apple_image);
	}
}

// 绘制蛇头
void draw_snake_head(const Coord& position, const Direction& direction)
{
	if (g_snake_head_image)
	{
		PIMAGE temp = newimage(CELL_SIZE, CELL_SIZE);
		getimage(temp, g_snake_head_image, 0, 0, CELL_SIZE, CELL_SIZE);

		int x = position.x * CELL_SIZE;
		int y = position.y * CELL_SIZE;

		putimage(x, y, temp);
		delimage(temp);
	}
}

// 绘制蛇身
void draw_snake_body(const std::deque<Coord>& body)
{
	if (g_snake_body_image && !body.empty())
	{
		for (size_t i = 0; i < body.size(); ++i)
		{
			int frame = static_cast<int>(i % SNAKE_BODY_FRAMES);
			int src_x = frame * SNAKE_BODY_SIZE;
			int src_y = 0;

			PIMAGE body_frame = newimage(SNAKE_BODY_SIZE, SNAKE_BODY_SIZE);
			getimage(body_frame, g_snake_body_image, src_x, src_y, SNAKE_BODY_SIZE, SNAKE_BODY_SIZE);
			putimage(body[i].x * CELL_SIZE, body[i].y * CELL_SIZE, body_frame);
			delimage(body_frame);
		}
	}
}

// 绘制墙壁
void draw_wall(const Coord& position)
{
	if (g_wall_image)
	{
		putimage(position.x * CELL_SIZE, position.y * CELL_SIZE, g_wall_image);
	}
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
