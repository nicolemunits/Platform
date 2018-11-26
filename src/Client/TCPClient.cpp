/*
 * TCPClient.cpp
 *
 *  Created on: May 25, 2016
 *      Author: user
 */

#include "TCPClient.h"

namespace npl {

TCPClient::TCPClient(Notify * notify, string ip, int port) {
	this->notify = notify;
	this->tcpSocket = new TCPSocket(ip, port);
	start();

}
void TCPClient::close() {

}

TCPClient::~TCPClient() {
}

void TCPClient::run() {
	Command cmd = NONE;
	map<string, string> data;
	while (true) {
		Protocol::readMSG(tcpSocket, cmd, data);
		if (cmd != 0) {
			notify->onReadCommandFromServer(this, cmd, data);
		}
	}
}

} /* namespace npl */

