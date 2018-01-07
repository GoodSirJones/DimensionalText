// DimensionalText.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <ios>
#include <sstream>
#include "sqlite3.h"
#include "Actions.h"

using namespace std;

int rc;
int roomID;
std::stringstream RoomName;
std::stringstream MoveID;
std::stringstream NewPath;
std::atomic<GameAction> game_action = GameAction::NONE;
char* error;
sqlite3 *db;
sqlite3 *stmt;
char exitName[10];

void Look();
//void processInput();
void moveRoom();
void Inventory();
void createPath();
void destroyPath();
void collect();
void interact();
void action(std::string&);
void transition();

int main()
{
	std::string input = "";

	roomID = 1;
	

	rc = sqlite3_open("house2.db", &db);

	if (rc)
	{
		cerr << "Error loading game: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_close(db);
		return 1;
	}
	else
	{
		cout << "Game successfully loaded" << endl;
	}

	cout << "Welcome to the game." << endl;

	RoomName << "SELECT NAME FROM locations WHERE ID = " << roomID << ";";

	string s = RoomName.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;


	sqlite3_get_table(db, query, &results, &rows, &columns, &error);


	int cellPosition = rows;
	
	cout << "You find yourself in a " << results[cellPosition] <<  ". What do you do?" << endl;



	while (input != "quit")
	{
		cin >> input;
		if (input == "move")
		{
			moveRoom();
		}
		else if (input == "look")
		{
			Look();
		}
		else if (input == "inventory")
		{
			Inventory();
		}
		else if (input == "collect")
		{
			collect();
		}
		else if (input == "create")
		{
			createPath();
		}
		else if (input == "destroy")
		{
			destroyPath();
		}
		else if (input == "interact")
		{
			interact();
		}
	}
	
/*	
	while (game_action != GameAction::QUIT)
	{
		cin >> input;

		action(input);
		processInput();
	}*/

	cout << "Thank you for playing" << endl;
	sqlite3_close(db);


    return 0;
}

/*
void action(std::string& input)
{
	if (input == ActionStringLiterals::look)
	{
		game_action = GameAction::LOOK;
	}
	else if (input == ActionStringLiterals::move)
	{
		game_action = GameAction::MOVE;
	}
	else if (input == ActionStringLiterals::inventory)
	{
		game_action = GameAction::INVENTORY;
	}
	else if (input == ActionStringLiterals::create)
	{
		game_action = GameAction::CREATE;
	}
	else if (input == ActionStringLiterals::destroy)
	{
		game_action = GameAction::DESTROY;
	}
	else if (input == ActionStringLiterals::collect)
	{
		game_action = GameAction::COLLECT;
	}
	else if (input == ActionStringLiterals::quit)
	{
		game_action = GameAction::QUIT;
	}
	else
	{
		game_action = GameAction::INVALID;
	}
	
	
	return;
}

void processInput()
{
	if (game_action == GameAction::LOOK)
	{
		Look();
	}
	else if (game_action == GameAction::MOVE)
	{
		moveRoom();
	}
	else if (game_action == GameAction::INVENTORY)
	{
		Inventory();
	}
	else if (game_action == GameAction::CREATE)
	{
		createPath();
	}
	else if (game_action == GameAction::DESTROY)
	{
		destroyPath();
	}
	else if (game_action == GameAction::COLLECT)
	{
		collect();
	}
	else if (game_action == GameAction::QUIT)
	{
		cout << "Thank you for playing our game!" << endl;
	}
	else if (game_action == GameAction::INVALID)
	{
		cout << "I'm sorry, I don't know what that means" << endl;
	}

	return;
}*/

void collect()
{
	char itemName;

	cout << "Which item do you want to collect?" << endl;
	cin >> itemName;

	cout << "That item does not exist." << endl;

	return;
}

void interact()
{
	cout << "There is nothing to interact with" << endl;

	return;
}

void Look()
{
	std::stringstream Description;
	
	Description << "SELECT DESCRIPTION FROM locations WHERE ID = " << roomID << ";";
	string s = Description.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	if (rc)
	{
		cerr << "Cannot perform function due to: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		int cellPosition = rows;

		cout << results[cellPosition] << endl;
	}

	
	return;
}


void Inventory()
{
	const char* inventoryCheck = "SELECT * FROM inventory;";
	char **results;
	int rows, columns;

	sqlite3_get_table(db, inventoryCheck, &results, &rows, &columns, &error);

	if (rc)
	{
		cerr << "Error loading inventory due to: " << sqlite3_errmsg(db) << endl;
		sqlite3_free(error);
	}
	else
	{
		int cellPosition = rows;

		cout << results[cellPosition] << endl;
	}

	//cout << "Your inventory is currently empty." << endl;
	
	return;
}

void createPath()
{
	//int fromID;
	char pathDirection;
	int toID;

	
	

	cout << "Enter the direction you want the new path to head in." << endl;
	cin >> pathDirection;

	cout << "Enter the room the path is leading to." << endl;
	cin >> toID;

	NewPath << "INSERT INTO exits VALUES(" << roomID << ", '" << pathDirection << "', " << toID << ");";
	string s = NewPath.str();
	char *str = &s[0];
	
	const char *sqlInsert = str;
	rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);

	if (rc)
	{
		cerr << "Cannot create path because: " << sqlite3_errmsg(db) << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Path successfully created" << endl;
	}

	return;
}

void destroyPath()
{
	int destroyID;
	std::stringstream destroy;
	char choice;

	cout << "Input the ID of the room that the exit you wish to destroy leads to" << endl;
	cin >> destroyID;

	cout << "WARNING! Once you destroy this path, it will be forever lost to you! Are you sure you want to proceed?" << endl;
	cin >> choice;

	if (choice == 'yes')
	{
		destroy << "DELETE FROM exits WHERE TO_ID = " << destroyID << ";";
		string s = destroy.str();

		char *str = &s[0];

		const char *sqlInsert = str;
		rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);

		if (rc)
		{
			cerr << "Cannot destroy path because: " << sqlite3_errmsg(db) << endl;
			sqlite3_free(error);
		}
		else
		{
			cout << "Path successfully destroyed. Hope you didn't need it!" << endl;
		}
	}
	else if (choice == 'no')
	{
		cout << "Path destruction cancelled." << endl;
	}

	

	return;
}

void moveRoom()

{

	unsigned int transferID;
	cout << "Which direction do you wish to move in?" << endl;
	cin >> exitName;

	
	

	//cout << "Enter the ID of the room you wish to open a portal to." << endl;
	//cin >> roomID;

	/**/
	MoveID << "SELECT TO_ID FROM exits WHERE EXIT_NAME = '" << exitName << "';";
	string s1 = MoveID.str();
	char* str1 = &s1[0];
	const char* query1 = str1;
	char** results1;
	int rows1, columns1;

	
	sqlite3_get_table(db, query1, &results1, &rows1, &columns1, &error);
	
	int cellPosition1 = rows1;

	cout << "You are now headed " << results1[cellPosition1] << "." << endl;

	MoveID >> transferID;
	
	/*
	if (exitName == "North")
	{

	}*/
	

	RoomName << "SELECT NAME FROM locations INNER JOIN exits ON locations.ID = exits.TO_ID WHERE exits.TO_ID = " << transferID << ";";
	//RoomName << "SELECT NAME FROM locations INNER JOIN exits ON locations.ID = exits.TO_ID WHERE exits.EXIT_NAME = '" << exitName << "';";
	//RoomName << "SELECT NAME FROM locations WHERE ID = " << roomID << ";";
	string s = RoomName.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);
	if (rc)
	{
		cout << "You can't go in that direction." << endl;
		sqlite3_free(error);

	}
	else
	{

		int cellPosition = rows;

		cout << "You are now in a " << results[cellPosition] << ". What do you do next?" << endl;
	}
	


	return;
}

void transition()
{

	return;
}
