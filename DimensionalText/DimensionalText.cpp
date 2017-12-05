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
std::atomic<GameAction> game_action = GameAction::NONE;
char* error;
sqlite3 *db;
sqlite3 *stmt;
char directionName;

void Look();
void processInput();
void moveRoom();
void action(std::string&);

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

	/*
	do
	{

	} while (game_action != GameAction::QUIT);*/
	
	while (game_action != GameAction::QUIT)
	{
		cin >> input;

		action(input);
		processInput();
	}

	sqlite3_close(db);


    return 0;
}

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
		cout << "Your inventory is currently empty" << endl;
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

void moveRoom()
{
	
	//cout << "Which direction do you wish to move in?" << endl;
	//cin >> directionName;



	cout << "Enter the ID of the room you wish to move to " << endl;
	cin >> roomID;

	RoomName << "SELECT NAME FROM locations WHERE ID = " << roomID << ";";
	string s = RoomName.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	int cellPosition = rows;

	if (rc)
	{
		cout << "I'm sorry, that room does not appear to exist." << endl;
		sqlite3_free(error);

	}
	else
	{
		cout << "You are now in a " << results[cellPosition] << ". What do you do next?" << endl;
	}
	


	return;
}

