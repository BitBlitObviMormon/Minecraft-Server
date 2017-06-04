#include "clienteventhandler.h"
#include <iostream>
#include <thread>

/*************************************************************
 * ClientEventHandler :: readPacket                          *
 * Reads the packet and fires off any events it gets from it *
 *************************************************************/
void ClientEventHandler::readPacket(Client* client, Byte* buffer, Int length)
{
	// Read the length of the packet
	Byte* buf = buffer;
	VarInt varLen = VarInt(buf);

	// Read the packet's id
	buf += varLen.getSize();
	VarInt varPack = VarInt(buf);

	// Calculate the length of the buffer minus the length
	// of the length and packet id variables themselves
	Int len = varLen.toInt() - varLen.getSize() - varPack.getSize();
	buf += varPack.getSize();

	// Check which state the client is currently in
	int packid = varPack.toInt();
	std::cout << packid << ": " << len << " bytes\n";
	switch (client->state)
	{
	case Handshaking:
		// Check which packet the client was referring to
		switch (packid)
		{
		case 0x00:	// Handshaking
			handShake(client, buf, len);
			break;
		case 0xFE:	// Legacy Handshaking
			handShakeLegacy(client, buf, len);
			break;
		}
		break;
	case Status:
		switch (packid)
		{
		case 0x00:	// Request
			request(client, buf, len);
			break;
		case 0x01:	// Ping
			ping(client, buf, len);
			break;
		}
		break;
	case Login:
		switch (packid)
		{
		case 0x00:	// Login Start
			loginStart(client, buf, len);
			break;
		case 0x01:	// Encryption Response
			encryptionResponse(client, buf, len);
			break;
		}
		break;
	case Play:
		switch (packid)
		{
		case 0x00:	// Teleport Confirm
			teleportConfirm(client, buf, len);
			break;
		case 0x01:	// Tab Complete
			tabComplete(client, buf, len);
			break;
		case 0x02:	// Chat Message
			chatMessage(client, buf, len);
			break;
		case 0x03:	// Client Status
			// 0: Perform respawn
			// 1: Request stats
			// 2: Open inventory
			clientStatus(client, buf, len);
			break;
		case 0x04:	// Client Settings
			clientSettings(client, buf, len);
			break;
		case 0x05:	// Confirm Transaction (apology)
			confirmTransaction(client, buf, len);
			break;
		case 0x06:	// Enchant Item
			enchantItem(client, buf, len);
			break;
		case 0x07:	// Click Window
			clickWindow(client, buf, len);
			break;
		case 0x08:	// Close Window
			closeWindow(client, buf, len);
			break;
		case 0x09:	// Plugin Message (Mod Message)
			pluginMessage(client, buf, len);
			break;
		case 0x0a:	// Use Entity
			useEntity(client, buf, len);
			break;
		case 0x0b:	// Keep Alive
			keepAlive(client, buf, len);
			break;
		case 0x0c:	// Player Position
			playerPosition(client, buf, len);
			break;
		case 0x0d:	// Player Position and Look
			playerPositionAndLook(client, buf, len);
			break;
		case 0x0e:	// Player Look
			playerLook(client, buf, len);
			break;
		case 0x0f:	// Player on Ground
			playerOnGround(client, buf, len);
			break;
		case 0x10:	// Vehicle Move
			vehicleMove(client, buf, len);
			break;
		case 0x11:	// Steer Boat
			steerBoat(client, buf, len);
			break;
		case 0x12:	// Player Abilities
			playerAbilities(client, buf, len);
			break;
		case 0x13:	// Player Digging (Drop Item, Shoot Arrow, Finish Eating, Swap Item in Hand)
			playerDigging(client, buf, len);
			break;
		case 0x14:	// Entity Action
			entityAction(client, buf, len);
			break;
		case 0x15:	// Steer Vehicle
			steerVehicle(client, buf, len);
			break;
		case 0x16:	// Resource Pack Status
			resourcePackStatus(client, buf, len);
			break;
		case 0x17:	// Held Item Change
			heldItemChange(client, buf, len);
			break;
		case 0x18:	// Creative Inventory Action
			creativeInventoryAction(client, buf, len);
			break;
		case 0x19:	// Update Sign
			updateSign(client, buf, len);
			break;
		case 0x1a:	// Animation
			animation(client, buf, len);
			break;
		case 0x1b:	// Spectate
			spectate(client, buf, len);
			break;
		case 0x1c:	// Player Block Placement
			playerBlockPlacement(client, buf, len);
			break;
		case 0x1d:	// Use Item
			useItem(client, buf, len);
			break;
		}
		break;
	}

	// Assume what's left over in the packet is another packet
	Int tempLen = len + varPack.getSize() + (varLen.getSize() * 2);

	// If the packet's length is smaller than reality allows then complain
	if (tempLen < 1)
	{
		std::cout << "Error: Erroneous packet from " << client->name
			      << ": Has a length of " << tempLen << " bytes.\n"
				  << "\tExpected a length of " << length << " bytes.\n";
	}
	else if (tempLen < length)
	{
		// Create another array for the next packet
		buf = buffer + tempLen;
		length -= tempLen;
		Byte* tempBuf = new Byte[length];
		for (int i = 0; i < length; i++)
			tempBuf[i] = buf[i];

		// Read the next packet
		readPacket(client, tempBuf, length);
	}

	// Delete the buffer passed
	delete[] buffer;
}

/***********************************
 * ClientEventHandler :: handShake *
 * Greet an oncoming client        *
 ***********************************/
void ClientEventHandler::handShake(Client* client, Byte* buffer, Int length)
{
	// Read the protocol from the buffer
	VarInt protocol = VarInt(buffer);
	client->protocol = protocol.toInt();
	buffer += protocol.getSize();

	// Read the server address and port only to discard it (we already know this stuff)
	SerialString address = SerialString(buffer);
	buffer += address.getSize() + 2; // The address (string) plus the port (short)

	// Determine the client's next state
	Int state = VarInt(buffer).toInt();
	if (state == 1)
		client->state = Status;
	else
		client->state = Login;
}

/****************************************
* ClientEventHandler :: handShakeLegacy *
* Greet an old oncoming client          *
*****************************************/
void ClientEventHandler::handShakeLegacy(Client* client, Byte* buffer, Int length)
{
	// TODO: Implement legacy api
}

/*****************************************
 * ClientEventHandler :: request         *
 * The client wants the server's details *
 *****************************************/
void ClientEventHandler::request(Client* client, Byte* buffer, Int length)
{

}

/************************************
 * ClientEventHandler :: ping       *
 * The client is pinging the server *
 ************************************/
void ClientEventHandler::ping(Client* client, Byte* buffer, Int length)
{

}


/***************************************
 * ClientEventHandler :: loginStart    *
 * The client wants to join the server *
 ***************************************/
void ClientEventHandler::loginStart(Client* client, Byte* buffer, Int length)
{
	// Get the player's username
	SerialString name = SerialString(buffer);
	client->name = name.str();
	client->uuid.recalculate(client);
	std::cout << client->name << " has connected. ("
		      << client->uuid.str() << ")\n";

	// Go ahead and let the client in
	serverHandler->loginSuccess(client);
}


/************************************************************************
 * ClientEventHandler :: encryptionResponse                             *
 * The client is either accepting or denying the request for encryption *
 ************************************************************************/
void ClientEventHandler::encryptionResponse(Client* client, Byte* buffer, Int length)
{

}

/*****************************************
 * ClientEventHandler :: teleportConfirm *
 * The client knows we teleported it     *
 *****************************************/
void ClientEventHandler::teleportConfirm(Client* client, Byte* buffer, Int length)
{

}

/****************************************************
 * ClientEventHandler :: tabComplete                *
 * The client wants us to complete the chat command *
 ****************************************************/
void ClientEventHandler::tabComplete(Client* client, Byte* buffer, Int length)
{

}

/*****************************************
 * ClientEventHandler :: chatMessage     *
 * The client is sending a chat commmand *
 *****************************************/
void ClientEventHandler::chatMessage(Client* client, Byte* buffer, Int length)
{

}

/************************************************************************
 * ClientEventHandler :: clientStatus                                   *
 * The client is respawning, opening its inventory, or requesting stats *
 ************************************************************************/
void ClientEventHandler::clientStatus(Client* client, Byte* buffer, Int length)
{

}

/*****************************************************
 * ClientEventHandler :: clientSettings              *
 * The client is connecting or changing its settings *
 *****************************************************/
void ClientEventHandler::clientSettings(Client* client, Byte* buffer, Int length)
{

}

/*******************************************************************
 * ClientEventHandler :: confirmTransaction                        *
 * The client is apologizing for going against the server's wishes *
 *******************************************************************/
void ClientEventHandler::confirmTransaction(Client* client, Byte* buffer, Int length)
{

}

/***************************************
 * ClientEventHandler :: enchantItem   *
 * The client wants to enchant an item *
 ***************************************/
void ClientEventHandler::enchantItem(Client* client, Byte* buffer, Int length)
{

}

/*******************************************
 * ClientEventHandler :: clickWindow       *
 * The client is interacting with a window *
 *******************************************/
void ClientEventHandler::clickWindow(Client* client, Byte* buffer, Int length)
{

}

/**************************************
 * ClientEventHandler :: closeWindow  *
 * The client wants to close a window *
 **************************************/
void ClientEventHandler::closeWindow(Client* client, Byte* buffer, Int length)
{

}

/**********************************************************************************
 * ClientEventHandler :: pluginMessage                                            *
 * The client is sending a special message that may or may not be related to mods *
 **********************************************************************************/
void ClientEventHandler::pluginMessage(Client* client, Byte* buffer, Int length)
{

}

/*******************************************
 * ClientEventHandler :: useEntity         *
 * The client right-clicked another entity *
 *******************************************/
void ClientEventHandler::useEntity(Client* client, Byte* buffer, Int length)
{

}

/***********************************************************
 * ClientEventHandler :: keepAlive                         *
 * The client is responding to the server's keepAlive ping *
 ***********************************************************/
void ClientEventHandler::keepAlive(Client* client, Byte* buffer, Int length)
{

}

/*********************************************
 * ClientEventHandler :: playerPositon       *
 * The client wants to update their position *
 *********************************************/
void ClientEventHandler::playerPosition(Client* client, Byte* buffer, Int length)
{

}

/*******************************************************
 * ClientEventHandler :: playerPositionAndLook         *
 * The client wants to update their position and angle *
 *******************************************************/
void ClientEventHandler::playerPositionAndLook(Client* client, Byte* buffer, Int length)
{

}

/**************************************************
 * ClientEventHandler :: playerLook               *
 * The client wants to change their viewing angle *
 **************************************************/
void ClientEventHandler::playerLook(Client* client, Byte* buffer, Int length)
{

}

/*******************************************
 * ClientEventHandler :: playerOnGround    *
 * The client is either jumping or landing *
 *******************************************/
void ClientEventHandler::playerOnGround(Client* client, Byte* buffer, Int length)
{

}

/**************************************
 * ClientEventHandler :: vehicleMove  *
 * The client wants to move a vehicle *
 **************************************/
void ClientEventHandler::vehicleMove(Client* client, Byte* buffer, Int length)
{

}

/***********************************************************
 * ClientEventHandler :: steerBoat                         *
 * The client is telling us which boat paddles are turning *
 ***********************************************************/
void ClientEventHandler::steerBoat(Client* client, Byte* buffer, Int length)
{

}

/*******************************************************************
 * ClientEventHandler :: playerAbilities                           *
 * The client wants to take no damage, fly, or enter creative mode *
 *******************************************************************/
void ClientEventHandler::playerAbilities(Client* client, Byte* buffer, Int length)
{

}

/******************************************************************************
 * ClientEventHandler :: playerDigging                                        *
 * The client wants to one of dig, stop digging, finish digging, drop an item *
 * or stack, shoot an arrow, finish eating, or swap an item between its hands *
 ******************************************************************************/
void ClientEventHandler::playerDigging(Client* client, Byte* buffer, Int length)
{

}

/*****************************************************************
* ClientEventHandler :: entityAction                             *
* The client wants to interact with an entity (including itself) *
******************************************************************/
void ClientEventHandler::entityAction(Client* client, Byte* buffer, Int length)
{

}

/**************************************
 * ClientEventHandler :: steerVehicle *
 * The client wants to turn a vehicle *
 **************************************/
void ClientEventHandler::steerVehicle(Client* client, Byte* buffer, Int length)
{

}

/*********************************************************************************
 * ClientEventHandler :: resourcePackStatus                                      *
 * The client is giving back the status of a supposedly downloaded resource pack *
 *********************************************************************************/
void ClientEventHandler::resourcePackStatus(Client* client, Byte* buffer, Int length)
{

}

/***********************************************
 * ClientEventHandler :: heldItemChange        *
 * The client wants to select a different item *
 ***********************************************/
void ClientEventHandler::heldItemChange(Client* client, Byte* buffer, Int length)
{

}

/*****************************************************************
 * ClientEventHandler :: creativeInventoryAction                 *
 * The client wants to interact with the creative mode inventory *
 *****************************************************************/
void ClientEventHandler::creativeInventoryAction(Client* client, Byte* buffer, Int length)
{

}

/************************************************
* ClientEventHandler :: updateSign              *
* The client wants to change the text of a sign *
*************************************************/
void ClientEventHandler::updateSign(Client* client, Byte* buffer, Int length)
{

}

/****************************************
 * ClientEventHandler :: animation      *
 * The client wants to animate its arms *
 ****************************************/
void ClientEventHandler::animation(Client* client, Byte* buffer, Int length)
{

}

/*****************************************
* ClientEventHandler :: spectate         *
* The client wants to spectate an entity *
******************************************/
void ClientEventHandler::spectate(Client* client, Byte* buffer, Int length)
{

}

/**********************************************
 * ClientEventHandler :: playerBlockPlacement *
 * The client wants to place a block          *
 **********************************************/
void ClientEventHandler::playerBlockPlacement(Client* client, Byte* buffer, Int length)
{

}

/***********************************
 * ClientEventHandler :: useItem   *
 * The client wants to use an item *
 ***********************************/
void ClientEventHandler::useItem(Client* client, Byte* buffer, Int length)
{

}

/********************************************
 * ClientEventHandler :: ClientEventHandler *
 * Default Constructor                      *
 ********************************************/
ClientEventHandler::ClientEventHandler() : running(false) {}

/********************************************
 * ClientEventHandler :: ClientEventHandler *
 * Destructor                               *
 ********************************************/
ClientEventHandler::~ClientEventHandler() 
{
	// Stop the event handler
	stop();

	// Destroy the server's event handler
	delete serverHandler;
}

/*************************************
 * ClientEventHandler :: addClient   *
 * Handshakes with and adds a client *
 *************************************/
void ClientEventHandler::addClient(SOCKET newClient)
{
	// Add the client to the list of clients
	clients.insert(std::make_pair(newClient, new Client(newClient)));
}

/****************************************************
 * ClientEventHandler :: getClientFromSocket        *
 * Searches for a client using the given socket     *
 * It returns null if it could not find any clients *
 ****************************************************/
Client* ClientEventHandler::getClientFromSocket(SOCKET socket)
{
	return (*clients.find(socket)).second;
}

/***********************************
 * ClientEventHandler :: start     *
 * Starts up the client event loop *
 ***********************************/
void ClientEventHandler::start()
{
	running = true;

	while (running)
	{
		// Create a list of every client to listen to
		fd_set clientList;
		if (clients.size() > 0)
		{
			int i = 0;
			for (std::map<SOCKET, Client*>::iterator it = clients.begin(); it != clients.end(); it++, i++)
				clientList.fd_array[i] = it->first;
			clientList.fd_count = i;
			timeval timeout;
			timeout.tv_usec = 100000;
			int returnVal;

			// Listen to every client on the list
			if ((returnVal = select(NULL, &clientList, NULL, NULL, &timeout)) > 0)
			{
				// Receive some data from the clients
				for (int i = 0; i < returnVal; i++)
				{
					// Find the client in our list
					Client* client = getClientFromSocket(clientList.fd_array[i]);

					// Read some data from the client
					char* buf = new char[256];
					int dataRead = recv(client->socket, buf, 256, NULL);
					if (dataRead == SOCKET_ERROR)
					{
						std::cout << "Error reading data from " << client->name
							      << ": " << WSAGetLastError() << "\n";
					}
					else if (dataRead > 0)
					{
						std::cout << "Read " << dataRead << " bytes from "
							      << client->name << ".\n";

						// Attempt to read the varPack
						readPacket(client, (Byte*)buf, dataRead);
					}
				}
			}
			else if (returnVal == SOCKET_ERROR)
			{
				std::cout << "Error receiving client message: " << WSAGetLastError() << "\n";
			}
		}
	}
}

/*********************************************************
 * ClientEventHandler :: startAsync                      *
 * Starts up the client event loop on a different thread *
 *********************************************************/
void ClientEventHandler::startAsync() { new std::thread(&ClientEventHandler::start, this); }

/******************************
 * ClientEventHandler :: stop *
 * Terminates the event loop  *
 ******************************/
void ClientEventHandler::stop() { running = false; }
