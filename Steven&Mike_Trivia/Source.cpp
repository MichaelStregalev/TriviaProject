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

	Server server(&db);
	server.run();

	return 0;
}