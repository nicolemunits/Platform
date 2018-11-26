using namespace std;

#include "Client/ClientCLI.h"
#include "Server/ServerCLI.h"
#include <stdlib.h>
#include <iostream>

int showUsage() {
	cout << "Usage: ./Platform <c|s> <sport>" << endl;
	cout << "INFO:" << endl;
	cout << "	<c|s> : 'c' for client, 's' for server" << endl;
	cout << "	sport  : server port" << endl;
	return -1;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		return showUsage();
	}

	string choice(argv[1]);
	int serverPort = atoi(argv[2]);
	if (choice == "s")
		startServerCLI(serverPort);
	else if (choice == "t")
		startServerCLI(serverPort);
	else
		return showUsage();
	return 0;
}

