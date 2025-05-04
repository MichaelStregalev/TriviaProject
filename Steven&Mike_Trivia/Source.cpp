#pragma comment(lib, "ws2_32.lib")

#include "Server.h"
#include "SqliteDatabase.h"
#include <iostream>

/*
This is the main function, it will be responsible for starting the server.
*/

int main()
{

	SqliteDatabase db;
	db.open();				// Open the database

	Server server(&db);
	server.run();

	db.close();

	return 0;
}