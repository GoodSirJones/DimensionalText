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
char* problemID;
char* probProgress;
int itemActive;

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
void seeItems();
void floorLevel();
void examineInventoryItem();
void examineItem();
void earnReward();
void checkProblems();
void solvedProblems();


int main()
{
	std::string input = "";

	rc = sqlite3_open("house8.db", &db);

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


	problemResolved << "SELECT SOLVED FROM problems INNER JOIN locations ON problems.ID = locations.PROBLEM_LINK WHERE locations.ID = " << IDRoom << ";";
	string f = problemResolved.str();
	char* sdx = &f[0];
	const char* wait = sdx;
	char** getit;
	int row2, column2;

	sqlite3_get_table(db, wait, &getit, &row2, &column2, &error);

	int pnCell = row2;

	probProgress = getit[pnCell];

	if (probProgress = "0")
	{
		checkProblems();
	}
	else if (probProgress = "1")
	{
		solvedProblems();
	}
	


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
		else if (input == "items")
		{
			seeItems();
		}
		else if (input == "floor")
		{
			floorLevel();
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
	std::stringstream ItemInventory;

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
	
	itemActive = 0;

	LiftItem << "UPDATE items SET SET_DOWN = 0 WHERE NAME = '" << itemName << "';";
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

	ItemInventory << "UPDATE items SET CARRIED = 1 WHERE NAME = '" << itemName << "';";
	string fc = ItemInventory.str();
	char* carry = &fc[0];
	const char* sqlAdd = carry;
	rc = sqlite3_exec(db, sqlAdd, NULL, NULL, &error);


	return;
}

void examineItem()
{
	std::stringstream itemDescription;
	char itemName[10];

	cout << "Which item do you want to examine?" << endl;
	cin >> itemName;

	itemDescription << "SELECT DESCRIPTION FROM items WHERE NAME = '" << itemName << "';";
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
	std::stringstream ItemDrop;
	std::stringstream ItemLand;
	std::stringstream itemCrash;
	char itemName[10];

	cout << "Which item do you want to get rid of? Remember, you will be able to pick it up again later." << endl;
	cin >> itemName;

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

	itemActive = 1;

	ItemLand << "UPDATE items SET CARRIED = 0 AND ROOM_LINK = " << IDRoom << " WHERE NAME = '" << itemName << "';";
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
		cout << "Item lands with a thud on the floor." << endl;
	}

	itemCrash << "UPDATE items SET SET_DOWN = 1 WHERE NAME = '" << itemName << "';";
	string fc = itemCrash.str();
	char* carry = &fc[0];
	const char* sqlAdd = carry;
	rc = sqlite3_exec(db, sqlAdd, NULL, NULL, &error);

	return;
}

void examineInventoryItem()
{
	std::stringstream itemDescription;
	char itemName[10];

	cout << "Which item do you want to examine?" << endl;
	cin >> itemName;

	itemDescription << "SELECT DESCRIPTION FROM inventory WHERE ITEM_NAME = '" << itemName << "';";
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


	if (rc == SQLITE_NOTFOUND)
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

	if (rc == SQLITE_EMPTY)
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
	char* problemFix;
	std::stringstream itemInteraction;
	std::stringstream updateProblem;
	
	cout << "Which item do you wish to use?" << endl;
	cin >> itemUse;



	itemInteraction << "SELECT AFTER_DESC FROM problems INNER JOIN items ON items.ID = problems.ITEM_NEEDED WHERE items.NAME = '" << itemUse << "';";
	string s = itemInteraction.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	int cellPosition = rows;

	cout << results[cellPosition] << endl;

	problemFix = results[cellPosition];

	updateProblem << "UPDATE problems SET SOLVED = 1 WHERE AFTER_DESC = '" << problemFix << "';";
	string w = updateProblem.str();
	char* fgd = &w[0];
	const char* search = fgd;
	rc = sqlite3_exec(db, search, NULL, NULL, &error);

	earnReward();

	return;
}

void interact()
{
	char objectUse[10];
	std::stringstream objectInteraction;

	cout << "Which object do you wish to interact with?" << endl;
	cin >> objectUse;

	objectInteraction << "SELECT INTERACTION FROM objects WHERE OBJECT_NAME = '" << objectUse << "';";
	string s = objectInteraction.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	int cellPosition = rows;

	cout << results[cellPosition] << endl;


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

		problemResolved << "SELECT SOLVED FROM problems INNER JOIN locations ON problems.ID = locations.PROBLEM_LINK WHERE locations.ID = " << IDRoom << ";";
		string f = problemResolved.str();
		char* sdx = &f[0];
		const char* wait = sdx;
		char** getit;
		int row2, column2;

		sqlite3_get_table(db, wait, &getit, &row2, &column2, &error);

		int pnCell = row2;

		probProgress = getit[pnCell];

		if (probProgress = "0")
		{
			checkProblems();
		}
		else if (probProgress = "1")
		{
			solvedProblems();
		}

	}

	return;
}

void Inventory()
{
	std::string action = "";


	const char* inventoryCheck = "SELECT ITEM_NAME FROM inventory;";
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

	if (rc == SQLITE_NULL)
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
	if (rc == SQLITE_NULL)
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

		problemResolved << "SELECT SOLVED FROM problems INNER JOIN locations ON problems.ID = locations.PROBLEM_LINK WHERE locations.ID = " << IDRoom << ";";
		string f = problemResolved.str();
		char* sdx = &f[0];
		const char* wait = sdx;
		char** getit;
		int row2, column2;

		sqlite3_get_table(db, wait, &getit, &row2, &column2, &error);

		int pnCell = row2;

		probProgress = getit[pnCell];

		if (probProgress = "0")
		{
			checkProblems();
		}
		else if (probProgress = "1")
		{
			solvedProblems();
		}	
		
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

	itemList << "SELECT NAME FROM items WHERE ROOM_LINK = " << IDRoom << " AND SET_DOWN = 1;";
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

void objects()
{
	std::stringstream objectList;
	std::string action = "";

	objectList << "SELECT OBJECT_NAME FROM objects WHERE ROOM_LINK = " << IDRoom << ";";
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

void earnReward()
{
	char* rewardName;
	char* rewardDescription;
	std::stringstream findReward;
	std::stringstream describeReward;
	std::stringstream getReward;

	findReward << "SELECT NAME FROM rewards WHERE PROBLEM_LINK = " << problem << ";";
	string s = findReward.str();
	char* str = &s[0];
	const char* query = str;
	char** results;
	int rows, columns;

	sqlite3_get_table(db, query, &results, &rows, &columns, &error);

	int cellPosition = rows;

	rewardName = results[cellPosition];

	describeReward << "SELECT DESCRIPTION FROM rewards WHERE NAME = '" << rewardName << "';";
	string c = describeReward.str();
	char* rts = &c[0];
	const char* search = rts;
	char** findings;
	int row1, column1;

	sqlite3_get_table(db, search, &findings, &row1, &column1, &error);

	int pCell = row1;

	rewardDescription = findings[pCell];

	getReward << "INSERT INTO inventory (ITEM_NAME, DESCRIPTION) VALUES ('" << rewardName << "', '" << rewardDescription << "');";
	string st = getReward.str();
	char* get = &st[0];
	const char* sqlInsert = get;
	rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);
	
	return;
}

void checkProblems()
{
	std::stringstream seeProblem;


	seeProblem << "SELECT BEFORE_DESC FROM problems INNER JOIN locations ON problems.ID = locations.PROBLEM_LINK WHERE locations.ID = " << IDRoom << ";";
	string g = seeProblem.str();
	char* stf = &g[0];
	const char* condrum = stf;
	char** findings;
	int row1, column1;

	sqlite3_get_table(db, condrum, &findings, &row1, &column1, &error);

	int pCell = row1;

	cout << findings[pCell] << endl;

	problem = findings[pCell];
	
	return;
}

void solvedProblems()
{
	std::stringstream seeProblem;


	seeProblem << "SELECT AFTER_DESC FROM problems INNER JOIN locations ON problems.ID = locations.PROBLEM_LINK WHERE locations.ID = " << IDRoom << ";";
	string g = seeProblem.str();
	char* stf = &g[0];
	const char* condrum = stf;
	char** findings;
	int row1, column1;

	sqlite3_get_table(db, condrum, &findings, &row1, &column1, &error);

	int pCell = row1;

	cout << findings[pCell] << endl;

	problem = findings[pCell];




	return;

}


