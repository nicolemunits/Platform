
#ifndef MESSENGERPROTOCOL_H_
#define MESSENGERPROTOCOL_H_

#include "TCPSocket.h"
#include <map>
/**
 * Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
#define MSNGR_PORT 3346
#define FILE_PATH

#define TEST_PEER_NAME "test"
#define SESSION_REFUSED_MSG "Connection refused, try again later"

typedef enum {
	NONE, // null msg
	PORT_EXCHANGE,
	LOGIN,
	REGISTER,
	ERROR_LOGIN,
	ERROR_REGISTER,
	SUCCESS_LOGIN,
	SUCCESS_REGISTER,
	LIST_CONNECTED_USERS,
	SEND_GAME_REQUEST,
	NO_SUCH_USER,
	YOU_CANT_START_A_GAME_WITH_YOURSELF,
	USER_ALREADY_IN_GAME,
	GAME_REQUEST_REFUSED,
	GAME_REQUEST_ACCEPTED,
	DISCONNECT,
	SHUT_DOWN,
	COMMAND_NOT_FOUND,
	GAME_ENDED,
	SCOREBOARD,
	BRING_RANDOM_PLAYER,
	NO_PLAYERS_AVAILABLE,

//SESSION_ESTABLISHED=9,
//EXIT_SESSION=14,
//PORT_EXCHANGE=23,

} Command;

#define ROOM_NAME_KEY "ROOM_NAME"
#define USER_NAME_KEY "USER_NAME"
#define PASSWORD_KEY "PASSWORD"
#define PORT_KEY "port"
#define IP_KEY "ip"
#define GAME_GUESS "guess"
#define GAME_REAL "real"
#define FIRST_USER_TO_PLAY "first_user_to_play"
#define OPPONENT_USER_NAME "opponent_username"

namespace npl {

class Protocol {
public:

	void static IntSend(TCPSocket* sock, int data) {
		int tmp = htonl(data);
		char* buff = (char*) &tmp;
		sock->send(buff, 4);
	}

	void static StringSend(TCPSocket* sock, string& data) {
		if (data.size() > 0) {
			int size = data.size();
			size = htonl(size);
			sock->send((char*) &size, 4);
			sock->send(data);
		}
	}

	void static ReadString(TCPSocket* sock, string& data) {
		int len;
		sock->recv((char*) &len, 4);
		len = ntohl(len);
		char buff[512];
		sock->recv(buff, len);
		buff[len] = '\0';
		data = buff;
	}

	void static ReadInt(TCPSocket* sock, int& data) {
		int len;
		sock->recv((char*) &len, 4);
		data = ntohl(len);
	}

	void static readMSG(TCPSocket* sock, Command& com,
			map<string, string>& dataMap) {
		dataMap.clear();
		// Get Command
		ReadInt(sock, (int&) com);
		// Get Map Size
		int mapSize = 0;
		ReadInt(sock, mapSize);
		// Foreach Pair
		for (int i = 0; i < mapSize; ++i) {
			pair<string, string> currPair;
			// Get Key
			ReadString(sock, currPair.first);
			// Get Data
			ReadString(sock, currPair.second);
			// Insert Pair
			dataMap.insert(currPair);

		}

	}

	void static SendMSG(TCPSocket* sock, Command com,
			map<string, string> dataMap) {
		// Send Command
		IntSend(sock, com);

		// Send Map Size
		int mapSize = dataMap.size();
		IntSend(sock, mapSize);
		// Foreach Pair
		for (map<string, string>::iterator it = dataMap.begin();
				it != dataMap.end(); it++) {
			pair<string, string> currPair = *it;
			// Get Key
			StringSend(sock, currPair.first);
			// Get Data
			StringSend(sock, currPair.second);
		}
	}

	void static SendMSG(TCPSocket* sock, Command com) {
		// Send Command
		IntSend(sock, com);
		// Send Map Size
		int mapSize = 0;
		IntSend(sock, mapSize);
	}

};
}

#endif /* MESSENGERPROTOCOL_H_ */
