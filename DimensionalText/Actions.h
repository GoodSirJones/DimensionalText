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
	CREATE,
	DESTROY,
	COLLECT,
	QUIT
};

namespace ActionStringLiterals
{
	const std::string look = "look";
	const std::string move = "move";
	const std::string inventory = "inventory";
	const std::string create = "create";
	const std::string destroy = "destroy";
	const std::string collect = "collect";
	const std::string quit = "quit";
}

extern std::atomic<GameAction> game_action;