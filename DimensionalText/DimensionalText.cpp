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
char currentRoom;
char IDRoom;
std::stringstream RoomName;
std::stringstream MoveID;
std::stringstream NewPath;
std::atomic<GameAction> game_action = GameAction::NONE;
char* error;
sqlite3 *db;
sqlite3 *stmt;
char exitName[10];

void Look();
void moveRoom();
void Inventory();
void createPath();
void checkRoomID();
void destroyPath();
void collect();
void interact();
void action(std::string&);

int main()
{
	std::string input = "";

	roomID = 1;
	
	IDRoom = '1';

	MoveID.str() = "1";

	rc = sqlite3_open("house6.db", &db);

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

	cout << "Thank you for playing" << endl;
	sqlite3_close(db);


    return 0;
}

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
		cout << "You currently have :" << endl;
		int cellPosition = rows;

		cout << results[cellPosition] << endl;
	}
	
	return;
}

void createPath()
{
	//int fromID;
	char pathDirection[10];
	int pathID;
	char toRoom[10];
	char fromRoom[10];

	cout << "Enter the room you are currently in: " << endl;
	cin >> fromRoom;

	cout << "Enter the direction you want the new path to head in." << endl;
	cin >> pathDirection;

	cout << "Which room is the portal going towards?" << endl;
	cin >> toRoom;

	

	NewPath << "INSERT INTO exits (FROM_ROOM, DIRECTION, TO_ROOM) VALUES('" << fromRoom << "', '" << pathDirection << "', '" << toRoom << "');";
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
	char destroyName;
	std::stringstream destroy;
	char choice[5];

	cout << "Input the name of the exit you wish to destroy, or the direction it heads in" << endl;
	cin >> destroyName;

	cout << "WARNING! Once you destroy this path, it will be forever lost to you! Are you sure you want to proceed?" << endl;
	cin >> choice;

	if (choice == "yes")
	{
		destroy << "DELETE FROM exits WHERE DIRECTION = '" << destroyName << "';";
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
	else if (choice == "no")
	{
		cout << "Path destruction cancelled." << endl;
	}

	

	return;
}

void moveRoom()

{
	cout << "Which direction do you wish to move in?" << endl;
	cin >> exitName;
	
	

	RoomName << "SELECT NAME FROM locations INNER JOIN exits ON locations.NAME = exits.TO_ROOM WHERE exits.DIRECTION = '" << exitName << "';";
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

		roomID = cellPosition;


	}

	return;
}

void checkRoomID()
{

}


