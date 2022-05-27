#pragma once

#include "datatypes.h"
#include "clientevents.h"
#include "serverevents.h"
#include <utility>
#include <set>

#ifdef _WIN32 // WINDOWS
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <WinSock2.h>
	#include <WS2tcpip.h>
	#pragma comment(lib, "Ws2_32.lib")
#else		  // LINUX, POSIX, OSX
	#include <sys/socket.h>
	#include <arpa/net.h>
	#include <netdb.h>
	#include <unistd.h>
	typedef int SOCKET;
#endif

class Client
{
public:
	MainHand mainHand;							// Which hand is the player's main hand
	Boolean isDead;								// Whether or not the player is dead
	Boolean chatColors;							// Whether to send chat colors or not
	Boolean onGround;							// Whether the player is on the floor or in air
	ChatMode chatMode;							// What kind of chat / command output the client receives
	ServerState state;							// Whether the player is connecting, playing or changing its status
	Gamemode gamemode;							// The gamemode the player is playing with
	Dimension dimension;						// The dimension the player is in
	Byte viewDistance;							// How far the player's client is set to view
	DisplayedSkinParts skinParts;				// Which skin parts are visible on the player model
	PlayerAbilities abilities;					// Whether the client is creative, vulnerable, flying, etc.
	Int ticksSinceUpdate;						// How many ticks it's been since a ping
	Int protocolVersion;						// The version of the network protocol the client is using
	Int entityID;								// The player's unique entity id
	SOCKET socket;								// The socket the player is currently connected on
	Float yaw;									// The player camera's yaw angle
	Float pitch;								// The player camera's pitch angle
	Long uptime;								// How many ticks the player was connected
	UUID uuid;									// The unique id assigned to the player
	PositionF position;							// The player's absolute position
	String name;								// The name of the player
	String locale;					 			// The language the player is using (ie. en_US)
	String brand;					 			// The type of client (usually vanilla unless modded)
	Set< std::pair <Int, Int> > loadedChunks;	// All of the chunks that the client currently has loaded (x, z)

	// Default constructor
	Client(SOCKET newClient);
	virtual ~Client();

	// Allows clients to be copied


	// Boolean operators
	bool operator>(const Client& rhs)  const { return socket > rhs.socket; }
	bool operator<(const Client& rhs)  const { return socket < rhs.socket; }
	bool operator>=(const Client& rhs) const { return socket >= rhs.socket; }
	bool operator<=(const Client& rhs) const { return socket <= rhs.socket; }
	bool operator==(const Client& rhs) const { return socket == rhs.socket; }
	bool operator!=(const Client& rhs) const { return socket != rhs.socket; }
};

// Used to compare Client* by the socket
struct ClientComparator {
	bool operator()(Client* lhs, Client* rhs) const { return lhs->socket > rhs->socket; }
};