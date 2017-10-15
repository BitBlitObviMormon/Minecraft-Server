#pragma once
#include "clientevents.h"
#include "../data/datatypes.h"
#include "../server/serverstate.h"

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
	MainHand mainHand;			  // Which hand is the player's main hand
	Boolean chatColors;			  // Whether to send chat colors or not
	ChatMode chatMode;			  // What kind of chat / command output the client receives
	ServerState state;			  // Whether the player is connecting or playing
	Byte viewDistance;			  // How far the player's client is set to view
	DisplayedSkinParts skinParts; // Which skin parts are visible on the player model
	Int ticksSinceUpdate;		  // How many ticks it's been since a ping
	Int protocolVersion;		  // The version of the network protocol the client is using
	Int entityID;				  // The player's unique entity id
	SOCKET socket;				  // The socket the player is currently connected on
	Float yaw;					  // The player camera's yaw angle
	Float pitch;				  // The player camera's pitch angle
	Double x;					  // The player's absolute x position
	Double y;					  // The player's absolute y position
	Double z;					  // The player's absolute z position
	Long uptime;				  // How many ticks the player was connected
	UUID uuid;					  // The unique id assigned to the player
	String name;				  // The name of the player
	String locale;				  // The language the player is using (ie. en_US)
	String brand;				  // The type of client (usually vanilla unless modded)
public:
	Client(SOCKET newClient = NULL);
	~Client();
};