#include "server/server.h"
#include <cassert>
#include <climits>

int main()
{
	/* TODO: Fix memory leak so that this test can run
	for (unsigned int i = 0; i < UINT_MAX; ++i)
	{
		VarInt v = VarInt(i);
		assert(v.toInt() == i);
	}

	for (unsigned long long int i = 0; i < ULLONG_MAX; ++i)
	{
		VarLong v = VarLong(i);
		assert(v.toLong() == i);
	} */

	// Create the server and its handlers then start the server
	EventHandler* eventHandler = new EventHandler();
	NetworkHandler* networkHandler = new NetworkHandler(eventHandler);
	Server server = Server(eventHandler, networkHandler);
	server.start();

	return 0;
}
