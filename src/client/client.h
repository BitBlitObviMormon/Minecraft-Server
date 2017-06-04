#pragma once
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
	SOCKET socket;
	String name;
	ServerState state;
	INT protocol;
	UUID uuid;
public:
	Client(SOCKET newClient = NULL);
	~Client();
};