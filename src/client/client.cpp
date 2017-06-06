#include "client.h"
#include <sstream>

/***********************
 * Client :: Client    *
 * Default constructor *
 ***********************/
Client::Client(SOCKET newClient) : socket(newClient), state(Handshaking)
{
	// Calculate the name
	std::stringstream ss;
	ss << "Player " << socket;
	name = ss.str();

	// Calculate the UUID
	uuid = UUID(this);
}

/********************
 * Client :: Client *
 * Destructor       *
 ********************/
Client::~Client() 
{
	// Close the socket if the client is destroyed
	if (socket != NULL)
	{
		closesocket(socket);
		socket = NULL;
	}
}