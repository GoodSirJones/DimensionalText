DimensionalText Readme

Most functionality is carried out from DimensionalText.cpp.

To change out database, head to the following line:

rc = sqlite3_open("house11.db", &db);

within the quotes, type in the database files you want to use.

Database files located in DimensionalText Folder within project.

Databases used:
house11.db
minihouse.db
house10.db

actions listed in loop:
"move" to move between rooms.
"collect" to pick up objects.
"drop" to drop objects"
"create" to create exits
"destroy" to destroy exits
"look" to look around
"rewards" to view rewards earned.
"see" to examine specific object
"examine" to examine specific item
"items" to see all items in room
"objects" to see all objects in room
"interact" to interact with object
"floor" to check current floor
"quit" to exit game

"inventory" to see items in inventory
While in inventory:
"inspect" to examine specific item
"use" to use specific item
"back" to close inventory


