#include "server/server.h"

int main()
{
	// Create the server and its handlers then start the server
	EventHandler* eventHandler = new EventHandler();
	NetworkHandler* networkHandler = new NetworkHandler(eventHandler);
	Server server = Server(eventHandler, networkHandler);
	server.start();

	return 0;
}
