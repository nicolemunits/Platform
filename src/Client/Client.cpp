/*
 * Client.cpp
 *
 *  Created on: May 25, 2016
 *      Author: user
 */

#include "Client.h"

namespace npl {

Client::Client(int serverPort, EventHandler* handler) {
	this->status = NOT_CONNECTED;
	this->serverPort = serverPort;
	this->display = new Display();
	this->game = NULL;
	this->tcpClient = NULL;
	this->handler = handler;
}

void Client::onGameEnded(string winnerUsername) {

	Global::debug("Game ended, the winner is " + winnerUsername);
	this->display->showGameResult(winnerUsername == this->username);
	map<string, string> data;
	data[USER_NAME_KEY] = winnerUsername;
	Global::debug("before sending GAME_ENDED");
	Protocol::SendMSG(this->tcpClient->getTcpSocket(), GAME_ENDED, data);
	this->handler->onMessageSentToServer(GAME_ENDED);
	Global::debug("after sending GAME_ENDED");
	this->status = WAITING_FOR_SCOREBOARD;
}

void Client::onReadCommandFromServer(TCPClient* tcpClient, Command cmd,
		map<string, string> data) {
	int com = cmd;
	if (com < 0) {
		Global::debug("Error connecting to server");
		this->display->print("Error: Error in connction to server");
		this->shutdown();
		return;
	}
	handler->onMessageRecvFromServer(cmd);
	switch (cmd) {

	case ERROR_REGISTER:
		this->display->print("User In Use");
		this->status = CONNECTED;
		break;

	case SUCCESS_REGISTER:
		this->display->print("Register success");
		this->status = CONNECTED;
		break;

	case SEND_GAME_REQUEST: {
		this->status = WAITING_FOR_YER_OR_NO;
		this->fromUser = data[USER_NAME_KEY];
		cout << "Do you want to start a game with " << this->fromUser
				<< "? (yes/no)" << endl;
	}
		break;

	case GAME_REQUEST_ACCEPTED: {
		cout << "Starting the game!" << endl;
		if (DEBUG) {
			Global::printMap(data);
		}
		this->status = IN_GAME;
		this->game->startGame(data, this->username);
	}
		break;

	case GAME_REQUEST_REFUSED: {
		cout << "The user refused your request" << endl;
		this->status = CONNECTED;
	}
		break;

	case SCOREBOARD: {
		this->display->showScoreboard(data);
		this->status = CONNECTED;
	}
		break;

	case BRING_RANDOM_PLAYER: {
		string chosen = data[USER_NAME_KEY];
		cout << "The server pick the player: " << chosen << endl;
		this->sendGameRequest(chosen);
	}
		break;

	case NO_PLAYERS_AVAILABLE: {
		this->display->print("No users available to play with");
		this->status = CONNECTED;
	}
		break;

	case NO_SUCH_USER:
		this->display->print("User does not exist in the list");
		this->status = CONNECTED;
		break;

	case YOU_CANT_START_A_GAME_WITH_YOURSELF:
		this->display->print("You cant start a game with yourself");
		this->status = CONNECTED;
		break;

	case USER_ALREADY_IN_GAME:
		this->display->print("User is already in a game with someone");
		this->status = CONNECTED;
		break;

	case COMMAND_NOT_FOUND:
		this->display->print("Command Not Found (Server)");
		break;

	case ERROR_LOGIN:
		this->display->print(
				"Bad credentials or the user is already connected");
		break;

	case SUCCESS_LOGIN:
		this->display->print("Login success");
		this->status = CONNECTED;
		this->username = data[USER_NAME_KEY];
		break;

	case LIST_CONNECTED_USERS:
		this->display->printConnectedUsersList(data);
		break;

	case PORT_EXCHANGE: {
		int intPort = atoi(data.find(PORT_KEY)->second.c_str());
		Global::debug(
				"Started Game App at port " + Global::convertTostring(intPort));
		this->game = new Game(intPort, this);
	}
		break;

	default:
		this->display->print("Command Not Found");
		break;
	}

}

Client::~Client() {
	this->shutdown();
}

void Client::connect(string ip) {
	if (this->status == NOT_CONNECTED) {
		if (ip.compare("c") == 0) {
			ip = "127.0.0.1";
		}
		cout << "trying to connect to ip = " << ip << endl;
		this->tcpClient = new TCPClient(this, ip, this->serverPort);
		this->status = CONNECTED;
		Global::debug("after creating tcp client");
	} else {
		cout << "Error: You are Already Connected" << endl;
	}
}

void Client::yesAnswer() {
	map<string, string> data;
	data[USER_NAME_KEY] = this->fromUser;
	Protocol::SendMSG(this->tcpClient->getTcpSocket(), GAME_REQUEST_ACCEPTED,
			data);
	this->handler->onMessageSentToServer(GAME_REQUEST_ACCEPTED);
	this->status = IN_GAME;

}
void Client::noAnswer() {
	map<string, string> data;
	data[USER_NAME_KEY] = this->fromUser;
	Protocol::SendMSG(this->tcpClient->getTcpSocket(), GAME_REQUEST_REFUSED,
			data);
	this->handler->onMessageSentToServer(GAME_REQUEST_REFUSED);
	this->status = CONNECTED;
}

void Client::listOnLineUsers() {
	if (this->status == CONNECTED) {
		Protocol::SendMSG(this->tcpClient->getTcpSocket(),
				LIST_CONNECTED_USERS);
		this->handler->onMessageSentToServer(LIST_CONNECTED_USERS);
	}

	else {
		cout << "Error: You are NOT Connected" << endl;
	}
}

void Client::login(string UserDetials) {
	if (this->status == CONNECTED) {
		string delimiter = " ";
		string userName = UserDetials.substr(0, UserDetials.find(delimiter));
		string passWord = UserDetials.substr(UserDetials.find(delimiter) + 1);
		map<string, string> UserDetialsMap;
		UserDetialsMap[USER_NAME_KEY] = userName;
		UserDetialsMap[PASSWORD_KEY] = passWord;
		Protocol::SendMSG(this->tcpClient->getTcpSocket(), LOGIN,
				UserDetialsMap);
		this->handler->onMessageSentToServer(LOGIN);
	} else {
		if (this->status == NOT_CONNECTED) {
			cout << "Error: You are NOT Connected" << endl;
		} else {
			cout << "Error: You are already logged in" << endl;
		}
	}

}

void Client::regiester(string UserDetials) {
	if (this->status == CONNECTED) {
		string delimiter = " ";
		string userName = UserDetials.substr(0, UserDetials.find(delimiter));
		string passWord = UserDetials.substr(UserDetials.find(delimiter) + 1);
		map<string, string> UserDetialsMap;
		UserDetialsMap[USER_NAME_KEY] = userName;
		UserDetialsMap[PASSWORD_KEY] = passWord;
		Protocol::SendMSG(this->tcpClient->getTcpSocket(), REGISTER,
				UserDetialsMap);
		this->handler->onMessageSentToServer(REGISTER);
	} else {
		if (this->status == NOT_CONNECTED) {
			cout << "Error: You are NOT Connected" << endl;
		} else {
			cout << "Error: You are already logged in" << endl;
		}
	}

}

void Client::sendRandomPlayerRequest() {
	Protocol::SendMSG(this->tcpClient->getTcpSocket(), BRING_RANDOM_PLAYER);
	this->handler->onMessageSentToServer(BRING_RANDOM_PLAYER);
	cout << "Waiting for the server to choose a random player" << endl;
	this->status = WAITING_FOR_RANDOM_PLAYER;
}

void Client::sendGameRequest(string userName) {
	if (this->status == CONNECTED
			|| this->status == WAITING_FOR_RANDOM_PLAYER) {
		map<string, string> UserDetialsMap;
		UserDetialsMap[USER_NAME_KEY] = userName;
		Protocol::SendMSG(this->tcpClient->getTcpSocket(), SEND_GAME_REQUEST,
				UserDetialsMap);
		this->handler->onMessageSentToServer(SEND_GAME_REQUEST);
		cout << "Game request sent, waiting " << userName << " to response"
				<< endl;
		this->status = WAITING_FOR_USER_ANSWER;
	} else {
		if (this->status == NOT_CONNECTED) {
			cout << "you are not connected" << endl;
		} else {
			cout << "you are already in session" << endl;
		}
	}

}

void Client::dissconect() {
	if (status == IN_GAME)
		this->game->close();
	if (this->status != NOT_CONNECTED) {
		Protocol::SendMSG(this->tcpClient->getTcpSocket(), DISCONNECT);
		this->handler->onMessageSentToServer(DISCONNECT);
		this->tcpClient->close();
		this->status = NOT_CONNECTED;
	} else
		cout << "Error: You are NOT Connected" << endl;

}

void Client::shutdown() {
	if (this->status == IN_GAME) {
		Global::debug("before closing game");
		this->game->close();
		Global::debug("after closing game");
	}
	if (this->status == CONNECTED) {
		Global::debug("before disconnect");
		this->dissconect();
		Global::debug("after disconnect");
	}
	this->status = NOT_CONNECTED;
	this->display->~Display();
}

} /* namespace npl */

