#pragma once
#include <atomic>
#include <string>

enum class GameAction
{
	INVALID = -1,
	NONE = 0,
	LOOK,
	MOVE,
	INVENTORY,
	QUIT
};

namespace ActionStringLiterals
{
	const std::string look = "look";
	const std::string move = "move";
	const std::string inventory = "inventory";
	const std::string quit = "quit";
}

extern std::atomic<GameAction> game_action;