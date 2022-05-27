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
Server::Server(EventHandler* eventHandler, NetworkHandler* networkHandler, const Int port)
	: _constructedEventHandler(false), _constructedNetworkHandler(false),
	  listenSocket(INVALID_SOCKET), running(false), networkHandler(networkHandler), eventHandler(eventHandler), port(port)
{
	// Create a default server event handler if none was given
	if (eventHandler == NULL) {
		eventHandler = new EventHandler();
		_constructedEventHandler = true;
	}

	// Create a default client event handler if none was given
	if (networkHandler == NULL) {
		networkHandler = new NetworkHandler();
		_constructedNetworkHandler = true;
	}

	// Give the event handler and network handler a reference to one another
	eventHandler->networkHandler = networkHandler;
	networkHandler->eventHandler = eventHandler;

	// Initialize the network sockets
	sockInit();

	// Start up the server tick timer
//	eventHandler->startTickClock();
}

/********************
 * Server :: Server *
 * Destructor       *
 ********************/
Server::~Server()
{
	stop();

	// Delete the client event handler
	if (_constructedNetworkHandler)
		delete networkHandler;

	// Delete the server event handler
	if (_constructedEventHandler)
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
	sockaddr_in server = *new sockaddr_in;
	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = INADDR_ANY;
	server.sin_port = htons(port);
	if (::bind(listenSocket, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR)
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

	std::cout << "Now listening to port " << port << "...\n";

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
	std::cout << "Found client: " << client << ".\n";

	// Set up the client event network
	networkHandler->addClient(client);
}

/*********************
 * Server :: start   *
 * Starts the server *
 *********************/
void Server::start(double tps)
{
	// Begin listening for clients
	running = true;
	listenForClients();

	// Start up the client handler on a different thread
	networkHandler->startAsync();

	// Start up the server event handler's clock on a different thread
//	eventHandler->startTickClock( /* 1.0 / tps */ );

	// Begin accepting clients
	acceptClients();
}

/*******************************************
 * Server :: startAsync                    *
 * Starts the server on a different thread *
 *******************************************/
void Server::startAsync(double tps) { new std::thread(&Server::start, this, tps); }

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
