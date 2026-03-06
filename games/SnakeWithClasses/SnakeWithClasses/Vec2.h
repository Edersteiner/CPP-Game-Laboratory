#pragma once

struct Vec2
{
	int x{ 0 };
	int y{ 0 };
};

inline bool operator==(const Vec2& a, const Vec2& b)
{
	return a.x == b.x && a.y == b.y; // True AND/&& True = True
	// True AND False = False
	// False AND False = False
}