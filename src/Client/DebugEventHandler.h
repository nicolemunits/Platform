#ifndef DEBUGEVENTHANDLER_H_
#define DEBUGEVENTHANDLER_H_

#include <string>
#include "Client.h"
#include "EventHandler.h"

using std::string;

namespace npl {

class DebugEventHandler: public EventHandler {
public:
	void onMessageRecvFromServer(Command cmd);
	void onMessageSentToServer(Command cmd);
	void onGameEnded();
};

}

#endif /* DEBUGEVENTHANDLER_H_ */
