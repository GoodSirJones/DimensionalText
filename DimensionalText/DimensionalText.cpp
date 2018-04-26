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
string directionName;
string currentLevel;
std::stringstream MoveID;
std::stringstream problemResolved;
std::stringstream RoomName;
std::stringstream NewPath;
std::stringstream DescribeItem;
char* error;
sqlite3 *db;
sqlite3 *stmt;
string exitName;
bool exitsPresent;
bool wayClear;
char* problem;



void Look();
void moveRoom();
void Inventory();
void createPath();
void useItem();
void checkExits();
void destroyPath();
void collect();
void dropItem();
void interact();
void seeItems();
void seeObjects();
void floorLevel();
void examineInventoryItem();
void examineItem();
void earnReward(char* problem);

void seeRewards();


int main()
{
	std::string input = "";

	rc = sqlite3_open("minihouse.db", &db);

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
		else if (input == "examine")
		{
			examineItem();
		}
		else if (input == "interact")
		{
			interact();
		}
		else if (input == "items")
		{
			seeItems();
		}
		else if (input == "objects")
		{
			seeObjects();
		}
		else if (input == "floor")
		{
			floorLevel();
		}
		else if (input == "rewards")
		{
			seeRewards();
		}
		
	}

	cout << "Thank you for playing" << endl;
	sqlite3_close(db);


    return 0;
}

void collect()
{
	std::stringstream CollectItem;
	std::stringstream checkCarry;
	char* itemCheck;
	std::stringstream ItemInventory;

	char itemName[10];

	cout << "Which item do you want to collect?" << endl;
	cin >> itemName;

	checkCarry << "SELECT COLLECTABLE FROM objects WHERE NAME = '" << itemName << "';";
	string s = checkCarry.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;


	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	int cellPosition = rows;



	itemCheck = results[cellPosition];
	
	cout << itemCheck << endl;
	

	if (itemCheck = "0")
	{
		CollectItem << "UPDATE objects SET HIDDEN = 1 WHERE NAME = '" << itemName << "';";
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
	}
	else if (itemCheck = "1")
	{
		cout << "You cannot collect this item." << endl;
		
	}


	

	return;
}

void examineItem()
{
	std::stringstream itemDescription;
	char itemName[10];

	cout << "Which item do you want to examine?" << endl;
	cin >> itemName;

	itemDescription << "SELECT DESCRIPTION FROM objects WHERE NAME = '" << itemName << "';";
	string s = itemDescription.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;


	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	int cellPosition = rows;

	cout << results[cellPosition] << endl;

	return;
}

void dropItem()
{
	std::stringstream ItemLand;
	std::stringstream itemCrash;
	char itemName[10];


	cout << "Which item do you want to get rid of? Remember, you will be able to pick it up again later." << endl;
	cin >> itemName;

	
	ItemLand << "UPDATE objects SET ROOM_LINK = " << IDRoom << ", HIDDEN = 0 WHERE NAME = '" << itemName << "';";
	string si = ItemLand.str();
	char* thud = &si[0];
	const char* sqlInsert = thud;
	rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);
	
	if (rc)
	{
		cerr << "You cannot get rid of this item because: " << sqlite3_errmsg(db) << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Item dropped" << endl;
	}



	return;
}

void examineInventoryItem()
{
	std::stringstream itemDescription;
	char itemName[10];

	cout << "Which item do you want to examine?" << endl;
	cin >> itemName;

	itemDescription << "SELECT DESCRIPTION FROM objects WHERE NAME = '" << itemName << "';";
	string s = itemDescription.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;


	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	if (rc)
	{
		cerr << "You cannot examine this item because: " << sqlite3_errmsg(db) << endl;
		sqlite3_free(error);
	}
	else
	{
		int cellPosition = rows;

		cout << results[cellPosition] << endl;

	}

	

	return;
}

void checkExits()
{
	std::stringstream availableExits;
	std::stringstream displayCodeNames;

	availableExits << "SELECT DIRECTION FROM exits WHERE FROM_ROOM = '" << currentRoom << "';";
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

	displayCodeNames << "SELECT CODENAME FROM exits WHERE FROM_ROOM = '" << currentRoom << "';";
	string t = displayCodeNames.str();
	char* dts = &t[0];
	const char* lookup = dts;
	char** findings;
	int row1, column1;

	sqlite3_get_table(db, lookup, &findings, &row1, &column1, &error);

	if (rc)
	{
		cerr << "Error encountered: " << sqlite3_errmsg(db) << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "For identification purposes, the exits have the following codenames: " << endl;

		for (int codenameCount = 0; codenameCount <= row1; codenameCount++)
		{
			int posCell = codenameCount;

			cout << findings[posCell] << endl;
		}
	}

	return;
}

void useItem()
{
	char itemUse[10];
	char verb[10];
	char* objectUse;
	char* problemFix;
	std::string consequences = "";
	std::stringstream itemInteraction;
	std::stringstream actionUndertake;
	std::stringstream updateProblem;
	std::stringstream alternateCheck;
	std::stringstream stateChange;
	
	cout << "Which item do you wish to use?" << endl;
	cin >> itemUse;

	cout << "How do you want to use the item?" << endl;
	cin >> verb;

	actionUndertake << "SELECT NAME FROM objects INNER JOIN actions ON objects.ID = actions.OBJECT2_ID WHERE actions.OBJECT_1 = '" << itemUse << "' AND actions.VERB = '" << verb << "' AND objects.ROOM_LINK = " << IDRoom << ";";
	string t = actionUndertake.str();
	char* fds = &t[0];
	const char* changes = fds;
	char** solving;
	int row1, col1;

	sqlite3_get_table(db, changes, &solving, &row1, &col1, &error);

	int posCell = row1;
	
	objectUse = solving[posCell];

	cout << "You use the " << itemUse << " to " << verb << " the " << objectUse << ".";

	itemInteraction << "SELECT RESULT FROM actions WHERE OBJECT_1 = '" << itemUse << "' AND VERB = '" << verb << "';";
	string s = itemInteraction.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	int cellPosition = rows;

	cout << results[cellPosition] << endl;

	updateProblem << "UPDATE objects SET HIDDEN = 2 WHERE NAME = '" << objectUse << "';";
	string r = updateProblem.str();
	char* gfs = &r[0];
	const char* remark = gfs;
	rc = sqlite3_exec(db, remark, NULL, NULL, &error);

	alternateCheck << "SELECT ALTERNATE_EXISTS FROM objects WHERE NAME = '" << objectUse << "';";
	string e = alternateCheck.str();
	char* dfs = &e[0];
	const char* lookup = dfs;
	char** findings;
	int row2, col2;

	sqlite3_get_table(db, lookup, &findings, &row2, &col2, &error);

	int pCell = row2;

	problemFix = findings[pCell];

	if (problemFix = "0")
	{
		cout << "No further action necessary." << endl;
	}
	else if (problemFix = "1")
	{
		stateChange << "UPDATE objects SET HIDDEN = 0 WHERE ALTERNATE_STATE = '" << objectUse << "';";
		string k = stateChange.str();
		char* fnv = &k[0];
		const char* change = fnv;
		rc = sqlite3_exec(db, change, NULL, NULL, &error);
	}

	earnReward(objectUse);

	return;
}

void interact()
{
	char objectUse[10];
	char* problemFix;
	std::stringstream alternateCheck;
	std::stringstream objectInteraction;
	std::stringstream objectState;

	cout << "Which object do you wish to interact with?" << endl;
	cin >> objectUse;

	objectInteraction << "UPDATE objects SET HIDDEN = 2 WHERE NAME = '" << objectUse << "';";
	string s = objectInteraction.str();
	char* str = &s[0];
	const char* query = str;
	rc = sqlite3_exec(db, query, NULL, NULL, &error);


	alternateCheck << "SELECT ALTERNATE_EXISTS FROM objects WHERE NAME = '" << objectUse << "';";
	string e = alternateCheck.str();
	char* dfs = &e[0];
	const char* lookup = dfs;
	char** findings;
	int row2, col2;

	sqlite3_get_table(db, lookup, &findings, &row2, &col2, &error);

	int pCell = row2;

	problemFix = findings[pCell];

	if (problemFix = "0")
	{
		cout << "No further action necessary." << endl;
	}
	else if (problemFix = "1")
	{
		objectState << "UPDATE objects SET HIDDEN = 0 WHERE ALTERNATE_STATE = '" << objectUse << "';";
		string k = objectState.str();
		char* fnv = &k[0];
		const char* change = fnv;
		rc = sqlite3_exec(db, change, NULL, NULL, &error);

		cout << "Things have changed." << endl;

	}


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
	std::string action = "";

	//if CARRIED = 1: Item in inventory
	const char* inventoryCheck = "SELECT NAME FROM objects WHERE HIDDEN = 1;";
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
		cout << "You open up your inventory. You currently have :" << endl;
		for (int rowCtr = 0; rowCtr <= rows; rowCtr++)
		{
			
				int cellPosition = rowCtr;				
				cout << results[cellPosition] << endl;
		}

		while (action != "back")
		{
			cin >> action;
			if (action == "inspect")
			{
				examineInventoryItem();
			}
			if (action == "use")
			{
				useItem();
			}
		}
	}

	cout << "You close your inventory." << endl;
	
	return;
}

void createPath()
{
	char pathDirection[10];
	char codename[10];
	char toRoom[10];
	char fromRoom[10];
	

	cout << "Room you are currently in: " << currentRoom << endl;

	cout << "Enter the direction you want the new path to head in." << endl;
	cin >> pathDirection;


	cout << "Which room is the portal going towards?" << endl;
	cin >> toRoom;


	cout << "Please input a codename for ease of identification." << endl;
	cin >> codename;

	NewPath << "INSERT INTO exits (FROM_ROOM, DIRECTION, TO_ROOM, CODENAME) VALUES('" << currentRoom << "', '" << pathDirection << "', '" << toRoom << "', '" << codename << "');";
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
	char destroyName[10];
	std::stringstream destroy;
	std::string choice = "";

	cout << "Input the codename of the exit you wish to destroy" << endl;
	cin >> destroyName;

	cout << "WARNING! Once you destroy this path, it will be forever lost to you! Are you sure you want to proceed?" << endl;
	cin >> choice;

	if (choice == "yes")
	{
		destroy << "DELETE FROM exits WHERE CODENAME = '" << destroyName << "';";
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
	std::stringstream codenameConstraint;
	char* codenameCheck;

	cout << "Which direction do you wish to move in?" << endl;
	cin >> exitName;

	codenameConstraint << "SELECT CODENAME FROM exits WHERE DIRECTION = '" << exitName << "' AND FROM_ROOM = '" << currentRoom << "';";
	string vr = codenameConstraint.str();
	char* df = &vr[0];
	const char* confine = df;
	char** discover;
	int row2, col2;

	sqlite3_get_table(db, confine, &discover, &row2, &col2, &error);

	if (rc)
	{
		cout << "You can't go that way." << endl;
		sqlite3_free(error);
	}
	else
	{
		int positionCell = row2;

		cout << "Exit codename: " << discover[positionCell] << endl;

		codenameCheck = discover[positionCell];
	

	

	//exitName = directionName;

	RoomName << "SELECT NAME FROM locations INNER JOIN exits ON locations.NAME = exits.TO_ROOM WHERE exits.CODENAME = '" << codenameCheck << "';";
	string s = RoomName.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);
	if (rc == SQLITE_NOTFOUND)
	{
		cout << "You can't go that way." << endl;
		sqlite3_free(error);
	}
	else
	{
		int cellPosition = rows;

		cout << "You are now in a " << results[cellPosition] << "." << endl;

		currentRoom = results[cellPosition];
		
	
		
		MoveID << "SELECT ID FROM locations WHERE NAME = '" << results[cellPosition] << "';";
		currentID = MoveID.str();
		char* search = &currentID[0];
		const char* check = search;
		char** find;
		int row1, column1;

		sqlite3_get_table(db, check, &find, &row1, &column1, &error);

		int posCell = row1;

		IDRoom = find[posCell];
		
		cout <<"What do you do next? " << endl;
	}
}
	return;
}


void seeItems()
{
	std::stringstream itemList;
	std::string action = "";

	cout << "You look around for any items." << endl;

	
	itemList << "SELECT NAME FROM objects WHERE ROOM_LINK = " << IDRoom << " AND HIDDEN = 0 AND COLLECTABLE = 0;";
	string s = itemList.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);
	if (rc == SQLITE_EMPTY)
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

void seeObjects()
{
	std::stringstream objectList;
	std::string action = "";

	cout << "You look around for any objects." << endl;


	objectList << "SELECT NAME FROM objects WHERE ROOM_LINK = " << IDRoom << " AND HIDDEN = 0 AND COLLECTABLE = 1;";
	string s = objectList.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);
	if (rc == SQLITE_EMPTY)
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

void floorLevel()
{
	std::stringstream checkLevel;
	int cellPosition;

	checkLevel << "SELECT FLOOR FROM locations WHERE ID = '" << IDRoom << "';";
	string s = checkLevel.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	cout << "Current floor: " << endl;

	for (int rowCtr = 0; rowCtr <= rows; rowCtr++)
	{
		cellPosition = rowCtr;

		cout << results[cellPosition] << endl;
	}

	return;
}

void earnReward(char* problem)
{
	char* rewardName;
	std::stringstream findReward;
	std::stringstream getReward;

	findReward << "SELECT DESCRIPTION FROM rewards WHERE PROBLEM_NAME = '" << problem << "';";
	string s = findReward.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	int cellPosition = rows;

	cout << "You have just earned " << results[cellPosition] << "." << endl;

	rewardName = results[cellPosition];

	getReward << "UPDATE rewards SET EARNED = 1 WHERE NAME = '" << rewardName << "';";
	string st = getReward.str();
	char* get = &st[0];
	const char* sqlInsert = get;
	rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);
	
	return;
}

void seeRewards()
{
	std::stringstream viewRewards;

	viewRewards << "SELECT NAME FROM rewards WHERE EARNED = 1;";
	string s = viewRewards.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	cout << "Rewards currently earned: " << endl;

	for (int rowCtr = 0; rowCtr <= rows; rowCtr++)
	{
		int cellPosition = rowCtr;

		cout << results[cellPosition] << endl;
	}

	
	return;
}


