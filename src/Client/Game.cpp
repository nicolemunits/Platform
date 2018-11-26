#include "Game.h"

namespace npl {

Game::Game(int port, GameNotify* notify) {
	this->notify = notify;
	this->udpClient = new UDPClient(port, this);
	this->endOfGame = true;
	this->myTurn = false;
}
void Game::onReadCommandFromPeer(UDPClient* tcpClient, string msg) {
	handleInput(msg, this->opponentUsername);
	if (msg.length() == 1) {
		cout << "Your opponent guessed the letter " << msg << endl;
	} else {
		cout << "Your opponent guessed the word " << msg << endl;
	}
	this->myTurn = !this->myTurn;
}

void Game::handleInput(string input, string userPlayed) {
	if (input.length() == 1) {
		// if one char -> update guess
		char choice = input[0];
		for (unsigned int i = 0; i < realWord.length(); ++i) {
			if (realWord[i] == choice) {
				this->guess[i] = choice;
			}
		}
	}
	if (realWord == input || realWord == this->guess) {
		winner = userPlayed;
		this->endOfGame = true;
	}
}

void Game::run() {
	Global::debug("Game::run");
	this->udpClient->start();
	string currentUserTurn = this->gameDetails[FIRST_USER_TO_PLAY];
	string ipToSend = this->gameDetails[IP_KEY];
	string portToSend = this->gameDetails[PORT_KEY];

	this->opponentUsername = this->gameDetails[OPPONENT_USER_NAME];
	this->realWord = this->gameDetails[GAME_REAL];
	this->guess = this->gameDetails[GAME_GUESS];
	this->endOfGame = false;
	this->myTurn = currentUserTurn == this->username;

	Global::debug("currentUserTurn = " + currentUserTurn);
	Global::debug("this->username = " + this->username);

	if (myTurn) {
		cout << "You play first" << endl;
	} else {
		cout << "Your opponent play first" << endl;
	}
	Global::debug("opponentUsername = " + opponentUsername);
	Global::debug("username = " + username);
	Global::debug("currentUserTurn = " + currentUserTurn);
	Global::debug("guess = " + guess);
	Global::debug("realWord = " + realWord);

	while (!endOfGame) {
		//Global::debug("!endOfGame = true");

		if (myTurn) {
			//Global::debug("myTurn = true");
			if (endOfGame)
				break;
			cout << "The current pattern: " << this->guess << endl;
			cout << "Choose a letter or take a guess of length "
					<< realWord.length() << ":" << endl;
			string input = "";
			getline(cin, input);

			if (DEBUG && input == "backdoor") {
				input = realWord;
			}

			while (input.length() != 1 && input.length() != realWord.length()) {
				cout << "Please choose a letter or take a guess of length "
						<< realWord.length() << ":" << endl;
				getline(cin, input);
			}
			handleInput(input, this->username);
			if (!endOfGame) {
				if (input.length() == 1) {
					cout << "The pattern after your guess: " << this->guess
							<< endl;
				} else {
					cout << input << " is a wrong guess" << endl;
				}
				cout << "Waiting for opponent move..." << endl;
			}
			this->udpClient->sendMsg(input, ipToSend, portToSend);
			myTurn = !myTurn;
		} else {
			//Global::debug("myTurn = false");
			// nothing, wait for opponent move
		}
	}
	notify->onGameEnded(this->winner);
}

void Game::startGame(map<string, string> gameDetails, string username) {
	this->gameDetails = gameDetails;
	this->username = username;
	start();
}

void Game::close() {
	this->udpClient->close();
}

Game::~Game() {
}

} /* namespace npl */

