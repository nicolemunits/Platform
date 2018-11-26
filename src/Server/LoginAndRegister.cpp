/*
 * LoginAndRegister.cpp
 *
 *  Created on: May 16, 2016
 *      Author: user
 */

#include "LoginAndRegister.h"
#include "Md5.h"

namespace npl {

LoginAndRegister::LoginAndRegister() {
	dispatcher = new Dispatcher();
	this->userPassMap = Global::LoadUserPassFile();

}

void LoginAndRegister::add(TCPSocket* peer) {
	cout << "adding peer" << endl;
	pthread_mutex_lock(&peersLock);
	peers.push_back(peer);
	if (peers.size() == 1) {
		start();
	}
	pthread_mutex_unlock(&peersLock);
}

void LoginAndRegister::remove(TCPSocket* peer) {
	cout << "removing peer" << endl;
	pthread_mutex_lock(&peersLock);
	vector<TCPSocket*>::iterator it = findPeer(peer);
	if (it != peers.end())
		peers.erase(it);
	else
		cout << "Can't remove peer(peer not found)" << endl;
	pthread_mutex_unlock(&peersLock);
}

vector<TCPSocket*>::iterator LoginAndRegister::findPeer(TCPSocket* peer) {
	if (peers.size() > 0) {
		vector<TCPSocket*>::iterator it;
		for (it = peers.begin(); it != peers.end(); it++) {
			if (*it == peer) {
				return it;
			}
		}
	}
	return peers.end();
}

void LoginAndRegister::run() {
	while (peers.size() > 0) {
		MTCPListener listener;
		listener.add(this->peers);
		TCPSocket * peer = listener.listen(3);

		if (peer != NULL) {
			Command command;
			map<string, string> data;
			Protocol::readMSG(peer, command, data);

			cout << "got command number " << command << " with params:" << endl;
			Global::printMap(data);

			switch (command) {

			case REGISTER:
				cout << "REGISTER command" << endl;
				if (this->Register(data)) {
					PeerName * thisPeer = new PeerName(
							data.find(USER_NAME_KEY)->second, peer);
					dispatcher->add(thisPeer);
					this->remove(peer);
					cout << "register successful" << endl;
					Protocol::SendMSG(peer, SUCCESS_REGISTER);
				} else {
					cout << "register failed" << endl;
					Protocol::SendMSG(peer, ERROR_REGISTER);

				}
				break;

			case LOGIN:
				cout << "LOGIN command" << endl;
				if (this->Login(data)) {
					PeerName * thisPeer = new PeerName(
							data.find(USER_NAME_KEY)->second, peer);
					dispatcher->add(thisPeer);
					this->remove(peer);
					cout << "login successful" << endl;
					Protocol::SendMSG(peer, SUCCESS_LOGIN, data);
				} else {
					cout << "login failed" << endl;
					Protocol::SendMSG(peer, ERROR_LOGIN);
				}
				break;

			case DISCONNECT:
				cout << "DISCONNECT command" << endl;
				this->remove(peer);
				break;

			case NONE:
				cout << "NONE command" << endl;
				this->remove(peer);
				break;

			default:
				Protocol::SendMSG(peer, COMMAND_NOT_FOUND);
				break;
			}
		}
	}
}

bool LoginAndRegister::Login(map<string, string>& data) {
	bool ret = false;
	string userName = data.find(USER_NAME_KEY)->second;
	string passwordMd5 = md5(data.find(PASSWORD_KEY)->second);

	if (this->dispatcher->FindPeer(userName))
		return false;

	pthread_mutex_lock(&userPassMapLock);
	map<string, string>::iterator iter = FindUser(userName);
	if (iter != userPassMap.end()) {
		if (passwordMd5 == (string) iter->second) {
			ret = true;
		}
	}
	pthread_mutex_unlock(&userPassMapLock);
	return ret;
}

bool LoginAndRegister::Register(map<string, string> data) {
	string userName = data.find(USER_NAME_KEY)->second;
	string password = data.find(PASSWORD_KEY)->second;
	cout << userName << ":" << password << endl;

	bool ret = false;
	pthread_mutex_lock(&userPassMapLock);
	if (this->FindUser(userName) == userPassMap.end()) {
		userPassMap[userName] = md5(password);
		saveToFile();
		ret = true;
	}
	pthread_mutex_unlock(&userPassMapLock);
	return ret;
}

map<string, string>::iterator LoginAndRegister::FindUser(string username) {
	map<string, string>::iterator it = userPassMap.find(username);
	return it;
}

void LoginAndRegister::shutdown() {
	pthread_mutex_lock(&peersLock);
	for (vector<TCPSocket*>::iterator it = peers.begin(); it != peers.end();
			++it) {
		TCPSocket* p = *it;
		p->close();
		delete p;
	}
	pthread_mutex_unlock(&peersLock);
	delete dispatcher;
}

Dispatcher * LoginAndRegister::getDispatcher() {
	return dispatcher;
}

void LoginAndRegister::saveToFile() {
	Global::SaveMap(&userPassMap);
}

LoginAndRegister::~LoginAndRegister() {
}

}
