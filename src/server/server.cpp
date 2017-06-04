#include "server.h"
#include <iostream>
#include <thread>

/*************************
 * sockInit              *
 * Starts up the sockets *
 *************************/
int sockInit()
{
#ifdef _WIN32
	return WSAStartup(MAKEWORD(2, 2), &WSAData());
#else
	return 1;
#endif
}

/************************
 * sockQuit             *
 * Destroys the sockets *
 ************************/
int sockQuit()
{
#ifdef _WIN32
	return WSACleanup();
#else
	return 1;
#endif
}

/***********************
 * Server :: Server    *
 * Default Constructor *
 ***********************/
Server::Server(ClientEventHandler* eventHandler) : listenSocket(INVALID_SOCKET), running(false), clientHandler(eventHandler)
{
	// Create a default event handler if none was given
	if (clientHandler == NULL)
		clientHandler = new ClientEventHandler();

	// Initialize the network sockets
	sockInit();
}

/********************
 * Server :: Server *
 * Destructor       *
 ********************/
Server::~Server()
{
	stop();

	// Delete the event handler
	if (clientHandler != NULL)
		delete clientHandler;

	// Destroy the network sockets
	sockQuit();
}

/**********************************************
 * Server :: listenForClients                 *
 * Creates the server and listens for clients *
 **********************************************/
int Server::listenForClients()
{
	// Create the listener socket
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		std::cout << "Error creating listener: " << WSAGetLastError() << "\n";
		return 0;
	}

	// Bind the listener to a port
	int port = 1234;
	sockaddr_in server = *new sockaddr_in;
	server.sin_family = AF_INET;
//	server.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");
	server.sin_addr.S_un.S_addr = INADDR_ANY;
	server.sin_port = htons(port);
	if (bind(listenSocket, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		std::cout << "Failed to bind listener: " << WSAGetLastError() << "\n";
		closesocket(listenSocket);
		return 0;
	}

	// Listen for oncoming connections
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Listen failed with error: " << WSAGetLastError() << "\n";
		closesocket(listenSocket);
		return 0;
	}

	std::cout << "Now listening on port " << port << "...\n";

	return 1;
}

/********************************
 * Server :: acceptClients      *
 * Accepts clients as they come *
 ********************************/
int Server::acceptClients()
{
	timeval timeout;
	fd_set listener;

	// Accept clients until the server is interrupted
	while (running)
	{
		// Prepare to use select syscall
		listener.fd_count = 1;
		listener.fd_array[0] = listenSocket;
		timeout.tv_sec = 0;
		timeout.tv_usec = 100000;
		int returnVal;

		if ((returnVal = select(NULL, &listener, NULL, NULL, &timeout)) > 0)
		{
			new std::thread(&Server::addClient, this);
		}
		else if (returnVal == SOCKET_ERROR)
		{
			std::cout << "Error while listening for clients: " << WSAGetLastError() << "\n";
		}
	}
			

	return 1;
}

/******************************************
 * Server :: addClient                    *
 * Handshakes with the client and adds it *
 ******************************************/
void Server::addClient()
{
	// Get a connection with the client
	SOCKET client = accept(listenSocket, NULL, NULL);
	std::cout << "Found client.\n";

	// Set up the Client event network
	clientHandler->addClient(client);
}

/*********************
 * Server :: start   *
 * Starts the server *
 *********************/
void Server::start()
{
	// Begin listening for clients
	running = true;
	listenForClients();

	// Start up the client handler on a different thread
	clientHandler->startAsync();

	// Begin accepting clients
	acceptClients();
}

/*******************************************
 * Server :: startAsync                    *
 * Starts the server on a different thread *
 *******************************************/
void Server::startAsync() { new std::thread(&Server::start, this); }

/*************************
 * Server :: stop        *
 * Terminates the server *
 *************************/
void Server::stop()
{
	if (clientHandler != NULL)
		clientHandler->stop();
	running = false;
}
