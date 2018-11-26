/*
 * TCPClient.h
 *
 *  Created on: May 25, 2016
 *      Author: user
 */

#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include "../Shared/MThread.h"
#include "../Shared/TCPSocket.h"
#include "../Shared/Protocol.h"
#include "Global.h"
#include <string>

using namespace std;
namespace npl {

class TCPClient: public MThread {
public:
	class Notify {
	public:
		virtual void onReadCommandFromServer(TCPClient* tcpClient,
				Command command, map<string, string> data)=0;
	};
private:
	TCPSocket* tcpSocket;
	Notify * notify;

public:
	void run();
	void close();
	TCPClient(Notify * notify, string ip, int port);
	virtual ~TCPClient();

	TCPSocket* getTcpSocket() {
		return tcpSocket;
	}
};

} /* namespace npl */

#endif /* TCPCLIENT_H_ */
