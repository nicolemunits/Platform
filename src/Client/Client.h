/*
 * Client.h
 *
 *  Created on: May 25, 2016
 *      Author: user
 */

#ifndef CLIENT_H_
#define CLIENT_H_
#include "TCPClient.h"
#include "UDPClient.h"
#include "Display.h"
#include "EventHandler.h"
#include "Game.h"

typedef enum {
	NOT_CONNECTED,
	CONNECTED,
	WAITING_FOR_YER_OR_NO,
	WAITING_FOR_USER_ANSWER,
	IN_GAME,
	WAITING_FOR_RANDOM_PLAYER,
	WAITING_FOR_SCOREBOARD
} Status;

using namespace std;
namespace npl {

class Client: public TCPClient::Notify, Game::GameNotify {
private:
	TCPClient* tcpClient;
	EventHandler* handler;
	Game* game;
	int serverPort;
	Display * display;

public:
	string username;
	Status status;
	string fromUser;
	void onReadCommandFromServer(TCPClient* tcpClient, Command command,
			map<string, string> data);
	void onGameEnded(string winnerUsername);

	// Server methods
	void connect(string ip);
	void yesAnswer();
	void noAnswer();
	void listOnLineUsers();
	void login(string UserDetials);
	void regiester(string UserDetials);
	void sendGameRequest(string userName);
	void sendRandomPlayerRequest();

	// Teardown
	void dissconect();
	void shutdown();
	Client(int serverPort, EventHandler* handler);
	virtual ~Client();
};

} /* namespace npl */

#endif /* CLIENT_H_ */
