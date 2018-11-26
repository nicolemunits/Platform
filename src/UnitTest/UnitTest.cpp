#include "UnitTest.h"

#include <unistd.h>
#include <stdlib.h>

namespace npl {

void TestHandler::onMessageRecvFromServer(Command cmd) {
	this->lastCmdRecv = cmd;
}
void TestHandler::onMessageSentToServer(Command cmd) {
	this->lastCmdSent = cmd;
}
TestHandler::TestHandler() {
	this->lastCmdRecv = NONE;
	this->lastCmdSent = NONE;
}
TestHandler::~TestHandler() {

}

Test::Test() {
}

Test::~Test() {
}

void print(string msg) {
	cout << "[UnitTest] " << msg << endl;
}

class ServerRunner: public MThread {
	Server* server;
public:
	void run() {
		this->server->run();
	}
	ServerRunner(int serverPort) {
		server = new Server(serverPort);
	}
	~ServerRunner() {
		delete server;
	}
};

bool Test::test() {
	// the sleep(1) illustrates a realistic scenario
	TestHandler testHandler1;
	TestHandler testHandler2;

	// create server
	int serverPort = (rand() % 1000) + 6000;
	ServerRunner server(serverPort);
	server.start();
	print("Server started at port " + Global::convertTostring(serverPort));

	// create client1
	Client client1(serverPort, &testHandler1);
	// connect client1 to the server
	client1.connect("c");
	sleep(1);
	if (testHandler1.lastCmdRecv == PORT_EXCHANGE)
		print("Client 1 is connected to the server");
	else {
		print("Client 1 connect to server failed");
		return false;
	}

	// login client1 to the server with user a
	client1.login("a a");
	sleep(1);
	if (testHandler1.lastCmdRecv == SUCCESS_LOGIN
			&& testHandler1.lastCmdSent == LOGIN)
		print("Client 1 logged in");
	else {
		print("Client 1 failed to log in");
		return false;
	}

	// create client2
	Client client2(serverPort, &testHandler2);
	// connect client2 to the server
	client2.connect("c");
	sleep(1);
	if (testHandler2.lastCmdRecv == PORT_EXCHANGE)
		print("Client 2 is connected to the server");
	else {
		print("Client 2 connect to server failed");
		return false;
	}

	// login client2 to the server with user c
	client2.login("c c");
	sleep(1);
	if (testHandler2.lastCmdRecv == SUCCESS_LOGIN
			&& testHandler2.lastCmdSent == LOGIN)
		print("Client 2 logged in");
	else {
		print("Client 2 failed to log in");
		return false;
	}

	// client1 send a game request to client2
	client1.sendGameRequest("c");
	sleep(1);
	if (testHandler1.lastCmdSent == SEND_GAME_REQUEST
			&& testHandler2.lastCmdRecv == SEND_GAME_REQUEST)
		print("Client 1 sent game request to Client 2");
	else {
		print("Client 1 failed to send a game request to Client 2");
		return false;
	}

	// client2 accept the request
	client2.yesAnswer();
	sleep(1);
	if (testHandler2.lastCmdSent == GAME_REQUEST_ACCEPTED
			&& testHandler1.lastCmdRecv == GAME_REQUEST_ACCEPTED)
		print("Client 2 accept the request from Client 1");
	else {
		print("Client 2 failed to accept the request from Client 1");
		return false;
	}

	// Now they are playing the game...
	// The game ended, the winner is a
	client1.onGameEnded("a");
	client2.onGameEnded("a");
	sleep(1);
	if (testHandler1.lastCmdSent == GAME_ENDED
			&& testHandler2.lastCmdSent == GAME_ENDED
			&& testHandler1.lastCmdRecv == SCOREBOARD
			&& testHandler2.lastCmdRecv == SCOREBOARD)
		print("Client 2 accept the request from Client 1");
	else {
		print("Client 2 failed to accept the request from Client 1");
		return false;
	}

	// close clients
	client1.dissconect();
	client2.dissconect();
	if (testHandler1.lastCmdSent == DISCONNECT
			&& testHandler2.lastCmdSent == DISCONNECT)
		print("The Clients disconnected");
	else {
		print("The Clients failed to disconnect");
		return false;
	}

	return true;
}

} /* namespace npl */
