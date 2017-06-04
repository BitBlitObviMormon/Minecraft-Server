#include "servereventhandler.h"

/************************************
 * ServerEventHandler :: serverTick *
 * A tick in the game's loop        *
 ************************************/
void ServerEventHandler::serverTick()
{

}

/**********************************************
 * ServerEventHandler :: response             *
 * Sends a bunch of server info to the client *
 **********************************************/
void ServerEventHandler::response(Client* client)
{

}

/********************************
* ServerEventHandler :: pong    *
* Responds to the client's ping *
*********************************/
void ServerEventHandler::pong(Client* client)
{

}

/******************************************
 * ServerEventHandler :: disconnect       *
 * Disconnects the client for some reason *
 ******************************************/
void ServerEventHandler::disconnect(Client* client)
{

}

/***********************************************
 * ServerEventHandler :: encryptionRequest     *
 * Asks the client to enable packet encryption *
 ***********************************************/
void ServerEventHandler::encryptionRequest(Client* client)
{

}

/**************************************
 * ServerEventHandler :: loginSuccess *
 * Gives the client the ok to join    *
 **************************************/
void ServerEventHandler::loginSuccess(Client* client)
{
	// Turn the UUID and name into serial strings
	SerialString UUID = SerialString(client->uuid.str());
	SerialString name = SerialString(client->name);

	// Turn the serial strings into data
	VarInt packet = VarInt(0x02);
	char* data1 = UUID.makeData();
	char* data2 = name.makeData();
	VarInt length = VarInt(packet.getSize() + UUID.getSize() + name.getSize());

	// Send that data
	send(client->socket, (const char*)length.getData(), length.getSize(), NULL);
	send(client->socket, (const char*)packet.getData(), packet.getSize(), NULL);
	send(client->socket, data1, UUID.getSize(), NULL);
	send(client->socket, data2, name.getSize(), NULL);

	// Delete the sent data
	delete[] data1;
	delete[] data2;
}

/**************************************************************
 * ServerEventHandler :: setCompression                       *
 * Tells the client to compress and expect compressed packets *
 **************************************************************/
void ServerEventHandler::setCompression(Client* client)
{

}

/********************************************
 * ServerEventHandler :: ServerEventHandler *
 * Default Constructor                      *
 ********************************************/
ServerEventHandler::ServerEventHandler(ClientEventHandler* eventHandler) : clientHandler(eventHandler) {}

/********************************************
 * ServerEventHandler :: ServerEventHandler *
 * Destructor                               *
 ********************************************/
ServerEventHandler::~ServerEventHandler() { stopTick(); }

/**************************************
 * ServerEventHandler :: startTick    *
 * Starts the server's game tick loop *
 **************************************/
void ServerEventHandler::startTick(Int delay)
{
	running = true;
}

/*************************************
 * ServerEventHandler :: stopTick    *
 * Stops the server's game tick loop *
 *************************************/
void ServerEventHandler::stopTick() { running = false; }

