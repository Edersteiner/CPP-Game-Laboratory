#pragma once
#include <deque>
#include "Vec2.h"
#include "Direction.h"

class Snake
{
	Snake() = default;

	Vec2 Head() const;
	Direction Dir() const { return m_dir; }

	void SetNextDirection(Direction d);

	// Moves snake one step. If grow==true, it keeps the tail
	void Move(bool grow);

	bool Occupies(Vec2 p) const;
	bool HitSelf() const;

	const std::deque<Vec2>& Body() const { return m_body; }

private:
	Vec2 StepFrom(Vec2 p, Direction d) const;

private:
	std::deque<Vec2> m_body; // front = head
	Direction m_dir{ Direction::Right };
	Direction m_nextDir{ Direction::Right };

};

