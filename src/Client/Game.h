#ifndef GAME_H_
#define GAME_H_
#include "UDPClient.h"

using namespace std;
namespace npl {

class Game: public UDPClient::Notify, public MThread {
public:
	class GameNotify {
	public:
		virtual void onGameEnded(string winnerUsername)=0;
	};
private:
	UDPClient* udpClient;
	GameNotify* notify;
	map<string, string> gameDetails;
	string username;
	bool endOfGame;
	bool myTurn;
	string realWord;
	string guess;
	string winner;
	string opponentUsername;
	void handleInput(string input, string userPlayed);
	void onReadCommandFromPeer(UDPClient* tcpClient, string msg);
public:
	Game(int port, GameNotify* notify);
	void close();
	void run();
	void startGame(map<string, string> gameDetails, string username);
	virtual ~Game();
};

} /* namespace npl */

#endif /* GAME_H_ */
