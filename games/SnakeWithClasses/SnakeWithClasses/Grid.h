#pragma once
#include <vector>
#include "Vec2.h"
#include <string>

class Grid
{
public:
	Grid(int width, int height, char emptyChar = '.');

	int Width() const { return m_width; }
	int Height() const { return m_height; }

	bool InBounds(Vec2 p) const;
	int ToIndex(Vec2 p) const;

	void Clear(); // grid.Clear();
	void SetCell(Vec2 p, char c);
	char GetCell(Vec2 p) const;

	// Render to a string
	std::string ToString() const; // std::cout << grid.ToString


private:
	int m_width{ 10 };
	int m_height{ 10 };
	char m_empty{ '.' };
	std::vector<char> m_cells;
};

