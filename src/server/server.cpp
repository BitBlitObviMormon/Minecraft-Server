#include "server.h"
#include <iostream>
#include <thread>

/*************************
 * sockInit              *
 * Starts up the sockets *
 *************************/
#ifdef _WIN32
#define sockInit() WSAStartup(MAKEWORD(2, 2), &WSAData())
#else
#define sockInit() 1
#endif

/************************
 * sockQuit             *
 * Destroys the sockets *
 ************************/
#ifdef _WIN32
#define sockQuit() WSACleanup()
#else
#define sockQuit() 1
#endif

/***********************
 * Server :: Server    *
 * Default Constructor *
 ***********************/
Server::Server(EventHandler* eventHandler, NetworkHandler* networkHandler)
	: listenSocket(INVALID_SOCKET), running(false), networkHandler(networkHandler), eventHandler(eventHandler)
{
	// Create a default server event handler if none was given
	if (eventHandler == NULL)
		eventHandler = new EventHandler();

	// Create a default client event handler if none was given
	if (networkHandler == NULL)
		networkHandler = new NetworkHandler(eventHandler);

	// Seed the event handler with the network handler
	eventHandler->seedNetwork(networkHandler);

	// Initialize the network sockets
	sockInit();

	// Start up the server tick timer
	eventHandler->startTickClock();
}

/********************
 * Server :: Server *
 * Destructor       *
 ********************/
Server::~Server()
{
	stop();

	// Delete the client event handler
	if (networkHandler != NULL)
		delete networkHandler;

	// Delete the server event handler
	if (eventHandler != NULL)
		delete eventHandler;

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
	networkHandler->addClient(client);
}

/*********************
 * Server :: start   *
 * Starts the server *
 *********************/
void Server::start(Double tps)
{
	// Begin listening for clients
	running = true;
	listenForClients();

	// Start up the client handler on a different thread
	networkHandler->startAsync();

	// Start up the server event handler's clock on a different thread
	eventHandler->startTickClock(1.0 / tps);

	// Begin accepting clients
	acceptClients();
}

/*******************************************
 * Server :: startAsync                    *
 * Starts the server on a different thread *
 *******************************************/
void Server::startAsync(Double tps) { new std::thread(&Server::start, this, tps); }

/*************************
 * Server :: stop        *
 * Terminates the server *
 *************************/
void Server::stop()
{
	if (networkHandler != NULL)
		networkHandler->stop();
	running = false;
}
