#include "core/Coord.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <tuple>

namespace bs {

    static std::string StripSpaces(std::string s)
    {
        s.erase(std::remove_if(s.begin(), s.end(),
            [](unsigned char ch) { return std::isspace(ch) != 0; }),
            s.end());
        return s;
    }

    std::tuple<bool, Coord> ParseCoordLoose(const std::string& text)
    {
        std::string s = StripSpaces(text);
		if (s.empty()) return std::tuple<bool, Coord>{ false, { -1,-1 } };

        int x = 0;
        int y = 0;
        bool hasLetter = false;
        bool hasDigit = false;

        // Find first letter A-Z
        for (char ch : s)
        {
            if (std::isalpha(static_cast<unsigned char>(ch)))
            {
                hasLetter = true;
                char up = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
                x = up - 'A';
                break;
            }
        }

        std::string digits;
        for (char ch : s)
        {
            if (std::isdigit(static_cast<unsigned char>(ch))) 
                digits.push_back(ch);
        }

        if (!digits.empty())
        {
            hasDigit = true;
            try
            {
                y = std::stoi(digits) - 1;
            }
            catch (...)
            {
				std::tuple<bool, Coord> parseResult{ false, { -1,-1 } };
            }
        }

        if (!hasLetter || !hasDigit) return std::tuple<bool, Coord>{ false, { -1,-1 } };
        return std::tuple<bool, Coord>{ true, { x, y } };
    }

    std::string ToHumanCoord(Coord c)
    {
        std::ostringstream oss;
        oss << static_cast<char>('A' + c.x) << (c.y + 1);
        return oss.str();
    }

} // namespace bs