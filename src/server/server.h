#pragma once
#include "../data/datatypes.h"
#include "../client/clienteventhandler.h"

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
	ClientEventHandler* clientHandler;
	ServerEventHandler* serverHandler;
protected:
	int listenForClients();
	int acceptClients();
	void addClient();
public:
	Server(ClientEventHandler* clientEventHandler = NULL, ServerEventHandler* serverEventHandler = NULL);
	~Server();
	void start();
	void startAsync();
	void stop();
};