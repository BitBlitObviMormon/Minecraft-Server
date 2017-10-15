#pragma once
#include "../data/datatypes.h"
#include "networkhandler.h"

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

class Server
{
protected:
	volatile Boolean running;	// Used to tell the server to shut down
	SOCKET listenSocket;
	NetworkHandler* networkHandler;
	EventHandler* eventHandler;
protected:
	int listenForClients();
	int acceptClients();
	void addClient();
public:
	Server(EventHandler* eventHandler = NULL, NetworkHandler* networkHandler = NULL);
	~Server();
	void start(Double tps = 20.0);
	void startAsync(Double tps = 20.0);
	void stop();
};