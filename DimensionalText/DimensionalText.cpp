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
char* currentRoom;
char* itemDescription;
char* IDRoom;
string currentID;
std::stringstream MoveID;
std::stringstream RoomName;
std::stringstream NewPath;
std::stringstream DescribeItem;
char* error;
sqlite3 *db;
sqlite3 *stmt;
char exitName[10];

void Look();
void moveRoom();
void Inventory();
void createPath();
void useItem();
void checkRoomID(bool examine);
void destroyPath();
void collect();
void dropItem();
void interact();


int main()
{
	std::string input = "";

	roomID = 1;



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

	IDRoom = "1";

	RoomName << "SELECT NAME FROM locations WHERE ID = " << IDRoom << ";";
	string s = RoomName.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;


	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	int cellPosition = rows;
	
	cout << "You find yourself in a " << results[cellPosition] <<  ". What do you do?" << endl;

	currentRoom = results[cellPosition];

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
		else if (input == "drop")
		{
			dropItem();
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
	std::stringstream CollectItem;
	std::stringstream LiftItem;

	char itemName[10];

	cout << "Which item do you want to collect?" << endl;
	cin >> itemName;

	DescribeItem << "SELECT DESCRIPTION FROM items WHERE NAME = '" << itemName << "';";
	string s = DescribeItem.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	int cellPosition = rows;

	itemDescription = results[cellPosition];

	CollectItem << "INSERT INTO inventory (ITEM_NAME, DESCRIPTION) VALUES ('" << itemName << "', '" << itemDescription << "');";
	string st = CollectItem.str();
	char* get = &st[0];
	const char* sqlInsert = get;
	rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);

	if (rc)
	{
		cerr << "Cannot collect item because: " << sqlite3_errmsg(db) << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Item collected" << endl;
	}
	

	LiftItem << "DELETE FROM ITEMS WHERE NAME = '" << itemName << "';";
	string ts = LiftItem.str();
	char* store = &ts[0];
	const char* sqlDelete = store;
	rc = sqlite3_exec(db, sqlDelete, NULL, NULL, &error);

	if (rc)
	{
		cerr << "Item remains where it was because: " << sqlite3_errmsg(db) << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Item now in inventory" << endl;
	}

	return;
}

void dropItem()
{
	std::stringstream ItemDrop;
	std::stringstream ItemLand;
	char itemName[10];

	cout << "Which item do you want to get rid of? Remember, you will be able to pick it up again later." << endl;
	cin >> itemName;

	DescribeItem << "SELECT DESCRIPTION FROM items WHERE NAME = '" << itemName << "';";
	string s = DescribeItem.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	int cellPosition = rows;

	itemDescription = results[cellPosition];

	ItemDrop << "DELETE FROM inventory WHERE ITEM_NAME = '" << itemName << "';";
	string ds = ItemDrop.str();
	char* rid = &ds[0];
	const char* sqlDelete = rid;
	rc = sqlite3_exec(db, sqlDelete, NULL, NULL, &error);

	if (rc)
	{
		cerr << "You cannot get rid of this item because: " << sqlite3_errmsg(db) << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Item dropped" << endl;
	}

	ItemLand << "INSERT INTO items (NAME, DESCRIPTION) VALUES ('" << itemName << "', '" << itemDescription << "');";
	string si = ItemLand.str();
	char* thud = &si[0];
	const char* sqlInsert = thud;
	rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);

	if (rc)
	{
		cerr << "Something unexpected occurs: " << sqlite3_errmsg(db) << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Item lands with a thud on the floor" << endl;
	}

	return;
}

void useItem()
{

}

void interact()
{
	cout << "There is nothing to interact with" << endl;

	return;
}

void Look()
{
	std::stringstream Description;
	
	Description << "SELECT DESCRIPTION FROM locations WHERE ID = " << IDRoom << ";";
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
	const char* inventoryCheck = "SELECT DESCRIPTION FROM inventory;";
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
		for (int rowCtr = 0; rowCtr <= rows; rowCtr++)
		{
			
				int cellPosition = rowCtr;				
				cout << results[cellPosition] << endl;
		}		
	}
	
	return;
}

void createPath()
{
	char pathDirection[10];
	char toRoom[10];
	char fromRoom[10];

	cout << "Room you are currently in: " << currentRoom << endl;

	cout << "Enter the direction you want the new path to head in." << endl;
	cin >> pathDirection;

	cout << "Which room is the portal going towards?" << endl;
	cin >> toRoom;

	NewPath << "INSERT INTO exits (FROM_ROOM, DIRECTION, TO_ROOM) VALUES('" << currentRoom << "', '" << pathDirection << "', '" << toRoom << "');";
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
	char destroyName[10];
	std::stringstream destroy;
	std::string choice = "";

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

		currentRoom = results[cellPosition];

		//roomID = cellPosition;

		
		MoveID << "SELECT ID FROM locations WHERE NAME = '" << results[cellPosition] << "';";
		currentID = MoveID.str();
		char* search = &currentID[0];
		const char* check = search;
		char** find;
		int row1, column1;

		sqlite3_get_table(db, check, &find, &row1, &column1, &error);

		int posCell = row1;

		IDRoom = find[posCell];
	}

	return;
}

void checkRoomID(bool examine)
{
	
	return;
}


