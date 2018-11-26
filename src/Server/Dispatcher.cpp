/*
 * Dispatcher.cpp
 *
 *  Created on: May 16, 2016
 *      Author: user
 */

#include "Dispatcher.h"

namespace npl {

Dispatcher::Dispatcher() {
}

PeerName* Dispatcher::FindPeer(TCPSocket* sock) {
	PeerName* ret = NULL;
	pthread_mutex_lock(&peerlistlock);
	vector<PeerName*>::iterator it;
	for (it = peerlist.begin(); it != peerlist.end(); it++) {
		PeerName* usr = *it;

		if (usr->getPeer() == sock) {
			ret = *it;
		}
	}
	pthread_mutex_unlock(&peerlistlock);
	return ret;
}

PeerName* Dispatcher::FindPeer(string usrname) {
	PeerName* ret = NULL;
	pthread_mutex_lock(&peerlistlock);
	vector<PeerName*>::iterator it;
	for (it = peerlist.begin(); it != peerlist.end(); it++) {
		PeerName* usr = *it;

		if (usr->getUserName() == usrname) {
			ret = *it;
		}
	}
	pthread_mutex_unlock(&peerlistlock);
	return ret;
}

Dispatcher::~Dispatcher() {
}

void Dispatcher::add(PeerName* thisPeer) {
	pthread_mutex_lock(&peerlistlock);
	this->peerlist.push_back(thisPeer);
	pthread_mutex_unlock(&peerlistlock);
	if (peerlist.size() == 1)
		start();
}

map<string, string> Dispatcher::getScoreboard() {
	map<string, string> scoreboard;
	pthread_mutex_lock(&peerlistlock);
	for (vector<PeerName*>::iterator it = peerlist.begin();
			it != peerlist.end(); ++it) {
		PeerName* usr = *it;
		string userName = usr->getUserName();
		string score = Global::convertTostring(usr->getScore());
		scoreboard.insert(pair<string, string>(userName, score));
	}
	pthread_mutex_unlock(&peerlistlock);
	return scoreboard;
}

string Dispatcher::pickRandomFreePlayer(string exclude) {
	string chosen = "";
	pthread_mutex_lock(&peerlistlock);
	vector<PeerName*> available;
	for (vector<PeerName*>::iterator it = peerlist.begin();
			it != peerlist.end(); ++it) {
		PeerName* usr = *it;
		if (usr->status == PEER_FREE && usr->getUserName() != exclude)
			available.push_back(usr);
	}
	chosen =
			available.empty() ?
					"" : available[rand() % available.size()]->getUserName();
	pthread_mutex_unlock(&peerlistlock);
	return chosen;

}

string randomWord() {
	string words[] = { "diary", "bottle", "water", "packet", "chewing gum",
			"tissue", "glasses", "watch", "sweet", "photo", "camera", "stamp",
			"postcard", "dictionary", "coin", "brush", "credit card",
			"identity card", "key", "mobile phone", "phone", "card", "wallet",
			"button", "umbrella", "pen", "pencil", "lighter", "cigarette",
			"match", "lipstick", "purse", "case", "clip", "scissors", "rubber",
			"file", "bank", "note", "passport", "driving licence", "combat",
			"notebook", "laptop", "rubbish", "mirror", "painkiller",
			"sunscreen", "toothbrush", "headphone", "player", "battery",
			"light", "newspaper", "magazine", "alarm clock" };
	int size = sizeof(words) / sizeof(words[0]);
	return words[rand() % size];
}

void Dispatcher::run() {
	while (peerlist.size() > 0) {
		MTCPListener listener;
		listener.add(getPeersVector());
		//cout << "before listen" << endl;
		TCPSocket * peer = listener.listen(1);
		//cout << "after listen" << endl;
		if (peer != NULL) {
			PeerName* peerName = FindPeer(peer);
			//cout << "peer != NULL" << endl;
			//cout << "username is " << peerName->getUserName() << endl;
			Command command = NONE;
			map<string, string> data;
			//cout << "before readMSG" << endl;
			Protocol::readMSG(peer, command, data);
			//cout << "after readMSG" << endl;
			cout << "got command number " << command << " with params:" << endl;
			Global::printMap(data);

			switch (command) {
			case SEND_GAME_REQUEST:
				cout << "SEND_GAME_REQUEST command" << endl;
				this->SendGameRequest(peerName, data);
				break;

			case GAME_REQUEST_REFUSED: {
				cout << "GAME_REQUEST_REFUSED command" << endl;
				PeerName* destUser = FindPeer(data.find(USER_NAME_KEY)->second);
				Protocol::SendMSG(destUser->getPeer(),
						GAME_REQUEST_REFUSED, data);
			}
				break;

			case BRING_RANDOM_PLAYER: {
				string chosen = this->pickRandomFreePlayer(
						peerName->getUserName());
				if (chosen.empty()) {
					Protocol::SendMSG(peerName->getPeer(),
							NO_PLAYERS_AVAILABLE);
				} else {
					map<string, string> data;
					data[USER_NAME_KEY] = chosen;
					Protocol::SendMSG(peerName->getPeer(),
							BRING_RANDOM_PLAYER, data);
				}
			}
				break;

			case GAME_ENDED: {
				cout << "GAME_ENDED command" << endl;
				if (data.find(USER_NAME_KEY) != data.end()) {
					string winner = data[USER_NAME_KEY];
					PeerName* peer = FindPeer(winner);
					peer->addScore(1);
				}
				map<string, string> scoreboard = this->getScoreboard();
				cout << "before sending SCOREBOARD" << endl;
				Protocol::SendMSG(peerName->getPeer(), SCOREBOARD,
						scoreboard);
				cout << "after sending SCOREBOARD" << endl;
				peerName->status = PEER_FREE;
			}
				break;

			case GAME_REQUEST_ACCEPTED: {
				cout << "GAME_REQUEST_ACCEPTED command" << endl;
				PeerName* destUser = FindPeer(data.find(USER_NAME_KEY)->second);

				// build data for the game
				string real = randomWord();
				string guess = real;
				for (string::iterator it = guess.begin(); it != guess.end();
						++it) {
					if (*it != ' ') {
						*it = '-';
					}
				}
				map<string, string> peerGameDetails;
				peerGameDetails[USER_NAME_KEY] = destUser->getUserName();
				peerGameDetails[IP_KEY] = destUser->getPeer()->getDestAddr();
				peerGameDetails[PORT_KEY] = Global::convertTostring(
						destUser->getPeer()->getDestPort());
				peerGameDetails[GAME_GUESS] = guess;
				peerGameDetails[GAME_REAL] = real;
				peerGameDetails[FIRST_USER_TO_PLAY] = destUser->getUserName();
				peerGameDetails[OPPONENT_USER_NAME] = destUser->getUserName();

				map<string, string> destUserGameDetails;
				destUserGameDetails[USER_NAME_KEY] = peerName->getUserName();
				destUserGameDetails[IP_KEY] =
						peerName->getPeer()->getDestAddr();
				destUserGameDetails[PORT_KEY] = Global::convertTostring(
						peerName->getPeer()->getDestPort());
				destUserGameDetails[GAME_GUESS] = guess;
				destUserGameDetails[GAME_REAL] = real;
				destUserGameDetails[FIRST_USER_TO_PLAY] =
						destUser->getUserName();
				destUserGameDetails[OPPONENT_USER_NAME] =
						peerName->getUserName();

				destUser->status = PEER_IN_GAME;
				peerName->status = PEER_IN_GAME;

				Protocol::SendMSG(peerName->getPeer(),
						GAME_REQUEST_ACCEPTED, peerGameDetails);

				Protocol::SendMSG(destUser->getPeer(),
						GAME_REQUEST_ACCEPTED, destUserGameDetails);
			}
				break;

			case LIST_CONNECTED_USERS:
				cout << "LIST_CONNECTED_USERS command" << endl;
				Protocol::SendMSG(peer, LIST_CONNECTED_USERS,
						this->getMapOfAllConnectPeers());
				break;

			case DISCONNECT:
				cout << "DISCONNECT command" << endl;
				this->removePeer(peerName);
				break;
			case NONE:
				cout << "NONE command" << endl;
				this->removePeer(peerName);
				break;

			default:
				cout << "default command" << endl;
				Protocol::SendMSG(peer, COMMAND_NOT_FOUND);

			}
		}
	}
	cout << "done Dispatcher::run" << endl;
}

void Dispatcher::SendGameRequest(PeerName* peerName, map<string, string> data) {
	PeerName* destUser = FindPeer(data.find(USER_NAME_KEY)->second);
	if (destUser == NULL) {
		Protocol::SendMSG(peerName->getPeer(), NO_SUCH_USER);
	} else if (peerName->getUserName().compare(destUser->getUserName()) == 0) {
		Protocol::SendMSG(peerName->getPeer(),
				YOU_CANT_START_A_GAME_WITH_YOURSELF);
	} else if (destUser->status == PEER_IN_GAME) {
		Protocol::SendMSG(peerName->getPeer(), USER_ALREADY_IN_GAME);
	} else {
		cout << "sending game request from " + peerName->getUserName() << " to "
				<< destUser->getUserName() << endl;
		map<string, string> data;
		data.insert(
				pair<string, string>(USER_NAME_KEY, peerName->getUserName()));
		Protocol::SendMSG(destUser->getPeer(), SEND_GAME_REQUEST,
				data);
	}
}

void Dispatcher::removePeer(PeerName* peerName) {
	vector<PeerName*>::iterator it;
	pthread_mutex_lock(&peerlistlock);
	for (it = peerlist.begin(); it != peerlist.end(); it++) {
		PeerName* usr = *it;
		if (usr->getPeer() == peerName->getPeer())
			break;
	}
	if (it != peerlist.end()) {
		peerlist.erase(it);
	}
	pthread_mutex_unlock(&peerlistlock);
}

vector<TCPSocket*> Dispatcher::getPeersVector() {

	vector<TCPSocket*> ret;
	pthread_mutex_lock(&peerlistlock);
	for (unsigned int i = 0; i < peerlist.size(); i++) {
		ret.push_back(peerlist[i]->getPeer());
	}
	pthread_mutex_unlock(&peerlistlock);
	return ret;
}

map<string, string> Dispatcher::getMapOfAllConnectPeers() {
	map<string, string> usrList;
	int i = 0;
	pthread_mutex_lock(&peerlistlock);
	for (vector<PeerName*>::iterator it = peerlist.begin();
			it != peerlist.end(); ++it) {
		PeerName* usr = *it;
		string userName = usr->getUserName();
		string UserIndex = Global::convertTostring(i);
		usrList.insert(pair<string, string>(UserIndex, userName));
		i++;
	}
	pthread_mutex_unlock(&peerlistlock);
	return usrList;

}

}
