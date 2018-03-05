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
string moveLevel;

char* previousPath;
string currentLevel;
std::stringstream MoveID;
std::stringstream RoomName;
std::stringstream NewPath;
std::stringstream DescribeItem;
char* error;
sqlite3 *db;
sqlite3 *stmt;
char exitName[10];
bool exitsPresent;

void Look();
void moveRoom();
void Inventory();
void createPath();
void useItem();
void checkExits();
void objects();
void destroyPath();
void collect();
void dropItem();
void interact();
void checkPreviousPath(char*);
void moveBack();
void seeItems();
void floorLevel(char);


int main()
{
	std::string input = "";

	rc = sqlite3_open("house7.db", &db);

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
	
	cout << "You find yourself in a " << results[cellPosition] << ". " << endl;

	currentRoom = results[cellPosition];

	cout << "What do you do?" << endl;

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
		else if (input == "objects")
		{
			objects();
		}
		else if (input == "backtrack")
		{
			moveBack();
		}
		else if (input == "items")
		{
			seeItems();
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

	cout << itemDescription << endl;

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

	ItemLand << "INSERT INTO items(NAME, DESCRIPTION, ROOM_LINK) VALUES ('" << itemName << "', '" << itemDescription << "', " << IDRoom << ");";
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

void checkExits()
{
	std::stringstream availableExits;

	availableExits << "SELECT PATH_TOWARD FROM exits WHERE FROM_ROOM = '" << currentRoom << "';";
	string s = availableExits.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;
	

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);
	int exitCount;


	if (rc)
	{
		cerr << "Error encountered: " << sqlite3_errmsg(db) << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Exits currently available: " << endl;
		

		
		for (exitCount = 0; exitCount <= rows; exitCount++)
		{
			int cellPosition = exitCount;
			cout << results[cellPosition] << endl;
		}

	
	}


	return;
}

void useItem()
{
	char objectUse[10];
	
	cout << "Which object do you wish to use?" << endl;
	cin >> objectUse;

	

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
		checkExits();

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
	char backDirection[10];
	char toRoom[10];
	char fromRoom[10];
	

	cout << "Room you are currently in: " << currentRoom << endl;

	cout << "Enter the direction you want the new path to head in." << endl;
	cin >> pathDirection;

	cout << "Enter the direction the path will head towards when you go back." << endl;
	cin >> backDirection;

	cout << "Which room is the portal going towards?" << endl;
	cin >> toRoom;


	

	NewPath << "INSERT INTO exits (FROM_ROOM, PATH_TOWARD, PATH_BACK, TO_ROOM) VALUES('" << currentRoom << "', '" << pathDirection << "', '" << backDirection << "', '" << toRoom << "');";
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

	cout << "Input the ID of the exit you wish to destroy" << endl;
	cin >> destroyID;

	cout << "WARNING! Once you destroy this path, it will be forever lost to you! Are you sure you want to proceed?" << endl;
	cin >> choice;

	if (choice == "yes")
	{
		destroy << "DELETE FROM exits WHERE ID = '" << destroyID << "';";
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
	std::stringstream checkLevel;

	
	
	cout << "Which direction do you wish to move in?" << endl;
	cin >> exitName;

	checkPreviousPath(exitName);
	
	RoomName << "SELECT NAME FROM locations INNER JOIN exits ON locations.NAME = exits.TO_ROOM WHERE exits.PATH_TOWARD = '" << exitName << "';";
	string s = RoomName.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);
	if (rc)
	{
		cout << "You can't go that way." << endl;
		sqlite3_free(error);
	}
	else
	{
		int cellPosition = rows;

		cout << "You are now in a " << results[cellPosition] << "." << endl;
		
		

		currentRoom = results[cellPosition];


		
		
		cout <<"What do you do next? " << endl;
		
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

void moveBack()
{
	std::stringstream backTrack;
	
	backTrack << "SELECT NAME FROM locations INNER JOIN exits ON locations.NAME = exits.FROM_ROOM WHERE exits.PATH_BACK = '" << previousPath << "';";
	string s = backTrack.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;



	sqlite3_get_table(db, query, &results, &rows, &columns, &error);
	if (rc)
	{
		cout << "You can't go that way." << endl;
		sqlite3_free(error);
	}
	else
	{
		int cellPosition = rows;

		cout << "You are now in a " << results[cellPosition] << "." << endl;

		currentRoom = results[cellPosition];

		cout << "What do you do next? " << endl;

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

void seeItems()
{
	std::stringstream itemList;

	itemList << "SELECT DESCRIPTION FROM items WHERE ROOM_LINK = " << IDRoom << ";";
	string s = itemList.str();
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
		cout << "You can see: " << endl;
		for (int rowCtr = 0; rowCtr <= rows; rowCtr++)
		{
			int cellPosition = rowCtr;

			cout << results[cellPosition] << endl;
		}
	}

	return;
}

void objects()
{
	std::stringstream objectList;

	objectList << "SELECT DESCRIPTION FROM objects WHERE ROOM_LINK = " << IDRoom << ";";
	string s = objectList.str();
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
		cout << "You can see: " << endl;
		for (int rowCtr = 0; rowCtr <= rows; rowCtr++)
		{
			int cellPosition = rowCtr;

			cout << results[cellPosition] << endl;
		}
		
	}
	
	return;
}

void checkPreviousPath(char* lastPath)
{
	std::stringstream pathMemory;

	if (lastPath == "East")
	{
		previousPath = "West";
	}
	else if (lastPath == "North")
	{
		previousPath = "South";
	}
	else if (lastPath == "West")
	{
		previousPath = "East";
	}
	else if (lastPath == "South")
	{
		previousPath = "North";
	}
	else if (lastPath == "Upstairs")
	{
		previousPath = "Downstairs";
	}
	else if (lastPath == "Downstairs")
	{
		previousPath = "Upstairs";
	}

	return;
}

void floorLevel(char roomName)
{
	std::stringstream checkLevel;
	int cellPosition;

	checkLevel << "SELECT FROM locations FLOOR WHERE NAME = '" << roomName << "';";
	string s = checkLevel.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	for (int rowCtr = 0; rowCtr <= rows; rowCtr++)
	{
		cellPosition = rowCtr;
	}

	

	return;
}


