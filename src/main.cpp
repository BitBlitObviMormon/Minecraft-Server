#include "server/server.h"
#include "data/datatypes.h"
#include <iostream>
using namespace std;

/* Main entry point for the server */
int main()
{
	// Create the server and its handlers then start the server
	EventHandler* eventHandler = new EventHandler();
	NetworkHandler* networkHandler = new NetworkHandler(eventHandler);
	Server server = Server(eventHandler, networkHandler);
	server.start();

	return 0;
}