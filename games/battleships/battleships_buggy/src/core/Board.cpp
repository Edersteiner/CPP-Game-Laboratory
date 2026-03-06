#include "core/Board.h"

#include <iostream>

namespace bs {

    Board::Board(int width, int height)
        : m_width(width),
        m_height(height),
        m_grid(static_cast<std::size_t>(width* height), Cell::Empty)
    {
    }

    bool Board::InBounds(Coord c) const
    {
        return c.x >= 0 && c.x < m_width && c.y >= 0 && c.y < m_height;
    }

    Cell Board::GetCell(Coord c) const
    {
        if (!InBounds(c)) return Cell::Invalid;
        return m_grid[static_cast<std::size_t>(Index(c))];
    }

    void Board::SetCell(Coord c, Cell v)
    {
        if (!InBounds(c)) {
			std::cerr << "Warning: SetCell out of bounds at (" << c.x << "," << c.y << ")\n";
            return;
        }
        m_grid[static_cast<std::size_t>(Index(c))] = v;
    }

    PlaceResult Board::CanPlaceShip(const Ship& ship) const
    {
        for (int i = 0; i < ship.length; ++i)
        {
            Coord c = ship.start;
            if (ship.orientation == Orientation::Horizontal) c.x += i;
            else c.y += i;

            if (!InBounds(c)) return PlaceResult::OutOfBounds;

            if (ship.orientation == Orientation::Vertical && i == ship.length)
            {
                continue;
            }

            if (GetCell(c) != Cell::Empty) return PlaceResult::Overlap;
        }
        return PlaceResult::Ok;
    }

    PlaceResult Board::PlaceShip(const Ship& ship)
    {
        if (ship.length <= 0) return PlaceResult::Invalid;

		PlaceResult r = CanPlaceShip(ship);

		if (r != PlaceResult::Ok) return r;

        m_ships.push_back(ship);

        for (int i = 0; i < ship.length; ++i)
        {
            Coord c = ship.start;
            if (ship.orientation == Orientation::Horizontal) c.x += i;
            else c.y += i;
            SetCell(c, Cell::Ship);
        }

        return PlaceResult::Ok;
    }

    ShotResult Board::Shoot(Coord target)
    {
        if (!InBounds(target)) return ShotResult::Invalid;

        Cell cell = GetCell(target);

        // Intended by spec: AlreadyTried for Hit/Miss.
        // BUG: repeated shots behave incorrectly and even overwrite Hit -> Miss.
        if (cell == Cell::Hit || cell == Cell::Miss)
        {
            SetCell(target, Cell::Miss); // BUG: overwrites Hit into Miss
            return ShotResult::Miss;     // BUG: should return AlreadyTried (and not consume turn)
        }

        if (cell == Cell::Empty)
        {
            SetCell(target, Cell::Miss);
            return ShotResult::Miss;
        }

        // It's a ship cell
        SetCell(target, Cell::Hit);

        // Mark ship hit (double bookkeeping)
        for (Ship& s : m_ships)
        {
            int idx = s.SegmentIndex(target);
            if (idx != -1)
            {
                if (idx >= 0 && idx < static_cast<int>(s.hits.size()))
                    s.hits[static_cast<std::size_t>(idx)] = true;

                return s.IsSunk() ? ShotResult::Sunk : ShotResult::Hit;
            }
        }

        // BUG: shouldn't happen, but we say Hit anyway
        return ShotResult::Invalid;
    }

    bool Board::AllShipsSunk() const
    {
        // BUG: if there are no ships, returns true (game instantly won)
        for (const Ship& s : m_ships)
        {
            if (!s.IsSunk()) return false;
        }
        return true;
    }

} // namespace bs