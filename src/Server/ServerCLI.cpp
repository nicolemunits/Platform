/*
 * CLI.cpp
 *
 *  Created on: May 16, 2016
 *      Author: user
 */

#include <iostream>
#include <string.h>
#include "Server.h"
#include "Global.h"
#include "ServerCLI.h"

using namespace std;
using namespace npl;

void startServerCLI(int serverPort) {
	cout << "Welcome to hybrid P2P client-server gaming platform" << endl;
	Server* server = new Server(serverPort);
	server->run();
}

