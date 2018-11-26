#ifndef UNITTEST_H_
#define UNITTEST_H_

#include "../Client/Client.h"
#include "../Server/Server.h"
#include <iostream>

using namespace std;

namespace npl {

class TestHandler: public EventHandler {
public:
	Command lastCmdRecv;
	Command lastCmdSent;
	TestHandler();
	~TestHandler();
	virtual void onMessageRecvFromServer(Command cmd);
	virtual void onMessageSentToServer(Command cmd);
};

class Test {
public:
	Test();
	virtual ~Test();

	bool test();
};

} /* namespace npl */

#endif /* UNITTEST_H_ */
