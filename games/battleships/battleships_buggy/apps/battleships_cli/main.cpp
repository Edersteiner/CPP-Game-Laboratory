#include "core/Game.h"
#include "cli/ConsoleRenderer.h"
#include "cli/Input.h"
#include <iostream>
#include <string>
#include <tuple>

using namespace bs;

static void PrintHelp()
{
    std::cout <<
        "Commands:\n"
        "  help                - show this help\n"
        "  quit                - quit game\n"
        "  reveal              - toggle reveal ships on your own board (debug)\n"
        "  During play:\n"
        "    shoot A5           - shoot coordinate\n"
        "    or just: A5\n\n";
}

std::tuple<bool, Orientation> AskOrientation()
{
    std::cout << "Orientation (h/v): ";
    std::string s;
    std::getline(std::cin, s);

	if (s.empty()) return std::tuple<bool, Orientation>{ false, Orientation::Horizontal };
	if (s == "h" || s == "H") return std::tuple<bool, Orientation>{ true, Orientation::Horizontal };
    if (s == "v" || s == "V") return std::tuple<bool, Orientation>{ true, Orientation::Vertical };
    std::cout << "Invalid orientation. Defaulting to horizontal.\n";
	return std::tuple<bool, Orientation>{ false, Orientation::Horizontal };
}

int main()
{
    // Spec defaults: 10x10 grid, ships 2/3/3/4/5 (in Game::ShipLengths()).
    Game game(10, 10);
    game.StartSetup();

    ConsoleRenderer r;
    bool reveal = false;

    while (true)
    {
        r.ClearScreen();

        if (game.State() == GameState::Setup)
        {
            r.PrintTitle("Battleships (BUGGY) - Setup");
            const auto& player = game.CurrentPlayer();
            std::cout << player.name << ", place your ships.\n\n";

            RenderOptions opt;
            opt.revealShips = true;
            r.PrintBoard(player.own, opt);

            int placed = static_cast<int>(player.own.Ships().size());

            if (placed >= static_cast<int>(game.ShipLengths().size()))
            {
                std::cout << "All ships placed for " << player.name << ". Press Enter to continue...\n";
                std::string tmp; std::getline(std::cin, tmp);

                game.FinishSetupIfReady();
                game.NextPlayerDuringSetup();
                continue;
            }

            int length = game.ShipLengths()[static_cast<std::size_t>(placed)];
            std::cout << "Place ship of length " << length << ". Enter start coord (e.g. A1): ";

            std::string line;
            std::getline(std::cin, line);
            Command cmd = ParseCommandLoose(line);
            if (cmd.type == CommandType::Quit) break;
            if (cmd.type == CommandType::Help)
            {
                PrintHelp();
                std::cout << "Press Enter...\n";
                std::getline(std::cin, line);
                continue;
            }

			std::tuple<bool, Coord> parseCoordResult = ParseCoordLoose(line);

            if (!std::get<0>(parseCoordResult))
            {
                std::cout << "Invalid coordinate. Press Enter to try again...\n";
                std::getline(std::cin, line);
                continue;
			}

            std::tuple<bool, Orientation> parseOrientationResult = AskOrientation();

			if (!std::get<0>(parseOrientationResult))
            {
                std::cout << "Invalid orientation. Press Enter to try again...\n";
				std::getline(std::cin, line);
                continue;
            }

            PlaceResult pr = game.PlaceShipForCurrent(Ship{ length, std::get<1>(parseCoordResult), std::get<1>(parseOrientationResult)});
            if (pr != PlaceResult::Ok)
            {
                switch (pr) {
                    case PlaceResult::OutOfBounds:
                        std::cout << "Ship placement is out of bounds.\n";
					    break;
                    case PlaceResult::Overlap:
						std::cout << "Ship placement overlaps with existing ship.\n";
                        break;
                }

                std::cout << "Press Enter...\n";
                std::getline(std::cin, line);
            }

            continue;
        }

        if (game.State() == GameState::Playing)
        {
            r.PrintTitle("Battleships (BUGGY) - Play");
            auto& current = game.CurrentPlayerMut();
            auto& other = game.OtherPlayerMut();

            std::cout << "Turn: " << current.name << "\n\n";
            r.PrintTwoBoards(current, other, RenderOptions{ reveal, true });

            std::cout << "Enter command (A5 / shoot A5 / help / quit / reveal): ";
            std::string line;
            std::getline(std::cin, line);

            Command cmd = ParseCommandLoose(line);
            if (cmd.type == CommandType::Quit) break;
            if (cmd.type == CommandType::Help)
            {
                PrintHelp();
                std::cout << "Press Enter...\n";
                std::getline(std::cin, line);
                continue;
            }
            if (cmd.type == CommandType::RevealToggle)
            {
                reveal = !reveal;
                continue;
            }

            if (cmd.type == CommandType::Shoot)
            {
                ShotResult sr = game.ShootAtOpponent(cmd.shootTarget);
                std::cout << "Result: ";
                switch (sr)
                {
                case ShotResult::Miss:   std::cout << "Miss\n"; break;
                case ShotResult::Hit:    std::cout << "Hit\n"; break;
                case ShotResult::Sunk:   std::cout << "Sunk!\n"; break;
                case ShotResult::Invalid:std::cout << "Invalid\n"; break;
                default:                 std::cout << "?\n"; break;
                }

                std::cout << "Press Enter...\n";
                std::getline(std::cin, line);
            }
            continue;
        }

        if (game.State() == GameState::GameOver)
        {
            r.PrintTitle("Battleships (BUGGY) - Game Over");
            int winner = game.WinnerIndex();

            if (winner >= 0)
                std::cout << "Winner: " << (winner == 0 ? "Player 1" : "Player 2") << "\n";
            else
                std::cout << "Winner: (unknown)\n";

            std::cout << "\nPress Enter to quit...\n";
            std::string tmp; std::getline(std::cin, tmp);
            break;
        }
    }

    std::cout << "Bye!\n";
    return 0;
}