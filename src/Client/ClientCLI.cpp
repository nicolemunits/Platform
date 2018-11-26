/*
 * CLI.cpp

 *
 *  Created on: May 16, 2016
 *      Author: user
 */
#include <iostream>
#include <string.h>
#include "Client.h"
#include "Global.h"
#include "ClientCLI.h"
#include "DebugEventHandler.h"
#include <unistd.h>

using namespace std;
using namespace npl;

void startClientCLI(int serverPort) {
	DebugEventHandler handler;
	Client* client = new Client(serverPort, &handler);
	Global::printClientInstructions();
	while (true) {
		if (client->status == WAITING_FOR_USER_ANSWER
				|| client->status == IN_GAME
				|| client->status == WAITING_FOR_SCOREBOARD
				|| client->status == WAITING_FOR_RANDOM_PLAYER) {
			sleep(1);
			continue;
		}

		if (client->status != WAITING_FOR_YER_OR_NO)
			cout << "Enter Command:" << endl;
		string command;
		Global::debug("before getline");
		getline(cin, command);
		Global::debug("after getline");
		string delimiter = " ";
		string beginCommand = command.substr(0, command.find(delimiter));
		string endCommand = command.substr(command.find(delimiter) + 1);

		if (client->status == WAITING_FOR_YER_OR_NO) {
			if (beginCommand == Yes) {
				client->yesAnswer();
			} else if (beginCommand == No) {
				client->noAnswer();
			} else {
				cout << "Please choose yes or no!" << endl;
			}
			continue;
		}

		if (beginCommand == ConnectServer) {
			client->connect(endCommand);
		} else if (beginCommand == ListConnectedUsers) {
			client->listOnLineUsers();
		} else if (beginCommand == Help) {
			Global::printClientInstructions();
		} else if (beginCommand == Login) {
			client->login(endCommand);
		} else if (beginCommand == RegisterUser) {
			client->regiester(endCommand);
		} else if (beginCommand == StartGameWithSpecificUser) {
			client->sendGameRequest(endCommand);
		} else if (beginCommand == StartGameWithRandomUser) {
			client->sendRandomPlayerRequest();
		} else if (beginCommand == Disconnect) {
			client->dissconect();
		} else if (beginCommand == Quit) {
			client->shutdown();
			break;
		} else {
			cout << "Error: command [" << beginCommand << "] Not found" << endl;
		}
	}

}

