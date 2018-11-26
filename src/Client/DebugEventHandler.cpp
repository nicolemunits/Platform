#include "DebugEventHandler.h"

namespace npl {

void DebugEventHandler::onMessageRecvFromServer(Command cmd) {
	Global::debug("Got cmd from server = " + Global::convertTostring(cmd));
}
void DebugEventHandler::onMessageSentToServer(Command cmd) {
	Global::debug("Send cmd to server = " + Global::convertTostring(cmd));

}
void DebugEventHandler::onGameEnded() {
	Global::debug("Game is ended");

}

}
