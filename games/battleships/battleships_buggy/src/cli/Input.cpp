#include "cli/Input.h"
#include "core/Coord.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>

namespace bs {

    static std::string ToLower(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
        return s;
    }

    static std::vector<std::string> Split(const std::string& line)
    {
        std::istringstream iss(line);
        std::vector<std::string> parts;
        std::string p;
        while (iss >> p) parts.push_back(p);
        return parts;
    }

    Command ParseCommandLoose(const std::string& line)
    {
        Command cmd;
        cmd.raw = line;
        auto parts = Split(line);
        if (parts.empty()) return cmd;

        std::string head = ToLower(parts[0]);

        if (head == "q" || head == "quit" || head == "exit")
        {
            cmd.type = CommandType::Quit;
            return cmd;
        }
        if (head == "h" || head == "help")
        {
            cmd.type = CommandType::Help;
            return cmd;
        }
        if (head == "reveal")
        {
            cmd.type = CommandType::RevealToggle;
            return cmd;
        }

        if (head == "shoot")
        {
            std::string coordText = (head == "shoot" && parts.size() >= 2) ? parts[1] : parts[0];
            //cmd.type = CommandType::Shoot;
            //cmd.shootTarget = ParseCoordLoose(coordText); // BUG: {0,0} on failure
            return cmd;
        }

        return cmd;
    }

} // namespace bs