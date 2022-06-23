#include "bbms/client.h"
#include <sstream>
#include <mutex>
using namespace std;

/***********************
 * Client :: Client    *
 * Default constructor *
 ***********************/
Client::Client(SOCKET newClient) : socket(newClient), state(ServerState::Handshaking)
{
	// Calculate the name
	std::stringstream ss;
	ss << "Player " << socket;
	name = ss.str();

	// Calculate the UUID
	uuid = UUID::generate::fromNameSHA1(name);
}

/********************
 * Client :: Client *
 * Destructor       *
 ********************/
Client::~Client() {}
