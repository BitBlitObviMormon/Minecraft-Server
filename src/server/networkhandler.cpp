#include "networkhandler.h"
#include <iostream>
#include <thread>

/**************************************************
 * copyBuffer                                     *
 * Creates a byte array from another array        *
 * The new array will need to be deleted manually *
 **************************************************/
Byte* copyBuffer(Byte* data, Int length)
{
	// Copy the contents of the buffer to a new array
	Byte* buf = new Byte[length];
	for (int i = 0; i < length; i++)
		buf[i] = data[i];
	return buf;
}

/**************************
 * parseLong              *
 * Reads a long from data *
 **************************/
Long parseLong(Byte* data)
{
	return (((Long)data[0]) << 56) + (((Long)data[1]) << 48) + (((Long)data[2]) << 40) +
		   (((Long)data[3]) << 32) + (data[4] << 24) + (data[5] << 16) + (data[6] << 8) + data[7];
}

/**************************
 * parseInt               *
 * Reads an int from data *
 **************************/
Int parseInt(Byte* data)
{
	return (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];
}

/***************************
 * parseShort              *
 * Reads a short from data *
 ***************************/
Short parseShort(Byte* data)
{
	return (data[0] << 8) + data[1];
}

/*************************************************************
 * NetworkHandler :: readPacket                              *
 * Reads the packet and fires off any events it gets from it *
 *************************************************************/
void NetworkHandler::readPacket(Client* client, Byte* buffer, Int length)
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
	std::cout << client->name << ": 0x" << std::hex << packid << " - " << std::dec << len << " bytes\n";
	switch (client->state)
	{
	case ServerState::Handshaking:
		// Check which packet the client was referring to
		switch (packid)
		{
		case 0x00:	// Handshaking
			handShake(client, buf, len);
			break;
		case 0xFE:	// Legacy Handshaking
			handShakeLegacy(client, buf, len);
			break;
		default:	// Invalid Packet
			invalidPacket(client, buf, len, packid);
			break;
		}
		break;
	case ServerState::Status:
		switch (packid)
		{
		case 0x00:	// Request
			request(client, buf, len);
			break;
		case 0x01:	// Ping
			ping(client, buf, len);
			break;
		default:	// Invalid Packet
			invalidPacket(client, buf, len, packid);
			break;
		}
		break;
	case ServerState::Login:
		switch (packid)
		{
		case 0x00:	// Login Start
			loginStart(client, buf, len);
			break;
		case 0x01:	// Encryption Response
			encryptionResponse(client, buf, len);
			break;
		default:	// Invalid Packet
			invalidPacket(client, buf, len, packid);
			break;
		}
		break;
	case ServerState::Play:
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
		default:	// Invalid Packet
			invalidPacket(client, buf, len, packid);
			break;
		}
		break;
	default:	// Invalid Client State
		invalidState(client, buf, len);
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

/*************************************
 * NetworkHandler :: invalidPacket   *
 * The client sent an invalid packet *
 *************************************/
void NetworkHandler::invalidPacket(Client* client, Byte* buffer, Int length, Int packet)
{
	// Tell the server that the client sent an invalid packet
	InvalidPacketEventArgs e;
	e.client = client;
	e.packetID = packet;
	e.dataLength = length;
	Byte* data = copyBuffer(buffer, length);
	e.data = data;

	// Trigger the event before deleting the associated data
	// TODO: Implement thread-safe deletion
	eventHandler->triggerEvent(&EventHandler::invalidPacket, eventHandler, e);
	delete[] data;
}

/*************************************
 * NetworkHandler :: invalidState    *
 * The client is in an invalid state *
 *************************************/
void NetworkHandler::invalidState(Client* client, Byte* buffer, Int length)
{
	// Tell the server that the client is in an invalid state
	InvalidStateEventArgs e;
	e.client = client;
	e.state = client->state;
	eventHandler->triggerEvent(&EventHandler::invalidState, eventHandler, e);
}

/***********************************
 * NetworkHandler :: handShake     *
 * Greet an oncoming client        *
 ***********************************/
void NetworkHandler::handShake(Client* client, Byte* buffer, Int length)
{
	// Read the protocol from the buffer
	HandShakeEventArgs e;
	e.client = client;
	VarInt protocol = VarInt(buffer);
	e.protocolVersion = protocol.toInt();
	buffer += protocol.getSize();

	// Read the server address and port
	SerialString address = SerialString(buffer);
	e.serverAddress = address.str();
	buffer += address.getSize();
	e.serverPort = parseShort(buffer);
	buffer += 2; // 2 bytes (short)

	// Trigger the server's handshake event
	e.state = (ServerState)VarInt(buffer).toInt();
	eventHandler->triggerEvent(&EventHandler::handshake, eventHandler, e);
}

/****************************************
* NetworkHandler :: handShakeLegacy     *
* Greet an old oncoming client          *
*****************************************/
void NetworkHandler::handShakeLegacy(Client* client, Byte* buffer, Int length)
{
	LegacyServerListPingEventArgs e;
	e.client = client;
	e.payload = *buffer;

	// Trigger the server's legacy server list ping event
	eventHandler->triggerEvent(&EventHandler::legacyServerListPing, eventHandler, e);
}

/*****************************************
 * NetworkHandler :: request             *
 * The client wants the server's details *
 *****************************************/
void NetworkHandler::request(Client* client, Byte* buffer, Int length)
{
	// Alert the server of the client's request
	RequestEventArgs e;
	e.client = client;
	eventHandler->triggerEvent(&EventHandler::request, eventHandler, e);
}

/************************************
 * NetworkHandler :: ping           *
 * The client is pinging the server *
 ************************************/
void NetworkHandler::ping(Client* client, Byte* buffer, Int length)
{
	// Alert the server of the client's ping
	PingEventArgs e;
	e.client = client;
	e.payload = parseLong(buffer);
	eventHandler->triggerEvent(&EventHandler::ping, eventHandler, e);
}


/***************************************
 * NetworkHandler :: loginStart        *
 * The client wants to join the server *
 ***************************************/
void NetworkHandler::loginStart(Client* client, Byte* buffer, Int length)
{
	// Get the player's username
	LoginStartEventArgs e = LoginStartEventArgs();
	SerialString name = SerialString(buffer);
	e.client = client;
	e.name = client->name;

	// Prompt the server to let the client in
	eventHandler->triggerEvent(&EventHandler::loginStart, eventHandler, e);
}


/************************************************************************
 * NetworkHandler :: encryptionResponse                                 *
 * The client is either accepting or denying the request for encryption *
 ************************************************************************/
void NetworkHandler::encryptionResponse(Client* client, Byte* buffer, Int length)
{
	// Get the shared secret's length
	EncryptionResponseEventArgs e;
	e.client = client;
	VarInt sharedSecretLen = VarInt(buffer);
	e.sharedSecretLen = sharedSecretLen.toInt();
	buffer += sharedSecretLen.getSize();

	// Get the shared secret's data
	e.sharedSecret = copyBuffer(buffer, e.sharedSecretLen);
	buffer += e.sharedSecretLen;

	// Get the verify token's length
	VarInt verifyTokenLen = VarInt(buffer);
	e.verifyTokenLen = verifyTokenLen.toInt();
	buffer += verifyTokenLen.getSize();

	// Get the verify token's data
	e.verifyToken = copyBuffer(buffer, e.verifyTokenLen);
	
	// Notify the server of the client's response before deleting the associated data
	// TODO: Implement thread-safe deletion
	eventHandler->triggerEvent(&EventHandler::encryptionResponse, eventHandler, e);
	delete[] e.sharedSecret;
	delete[] e.verifyToken;
}

/*****************************************
 * NetworkHandler :: teleportConfirm     *
 * The client knows we teleported it     *
 *****************************************/
void NetworkHandler::teleportConfirm(Client* client, Byte* buffer, Int length)
{
	// Get the teleport's id
	TeleportConfirmEventArgs e;
	e.client = client;
	e.teleportID = VarInt(buffer).toInt();

	// Alert the server of the client's confirmation
	eventHandler->triggerEvent(&EventHandler::teleportConfirm, eventHandler, e);
}

/****************************************************
 * NetworkHandler :: tabComplete                    *
 * The client wants us to complete the chat command *
 ****************************************************/
void NetworkHandler::tabComplete(Client* client, Byte* buffer, Int length)
{
	// Get the text
	TabCompleteEventArgs e;
	e.client = client;
	SerialString text = SerialString(buffer);
	e.text = text.str();
	buffer += text.getSize();

	// Get assumeCommand and hasPosition
	e.assumeCommand = !!*(buffer++);
	e.hasPosition = !!*(buffer++);

	// If there was a position given, get the coordinates
	if (e.hasPosition)
		e.lookedAtBlock = SerialPosition(parseLong(buffer)).toPosition();

	// Send the information to the server
	eventHandler->triggerEvent(&EventHandler::tabComplete, eventHandler, e);
}

/*****************************************
 * NetworkHandler :: chatMessage         *
 * The client is sending a chat commmand *
 *****************************************/
void NetworkHandler::chatMessage(Client* client, Byte* buffer, Int length)
{
	// Get the text
	ChatMessageEventArgs e;
	e.client = client;
	e.message = SerialString(buffer).str();

	// Send the data to the server for interpreting / broadcasting
	eventHandler->triggerEvent(&EventHandler::chatMessage, eventHandler, e);
}

/************************************************************************
 * NetworkHandler :: clientStatus                                       *
 * The client is respawning, opening its inventory, or requesting stats *
 ************************************************************************/
void NetworkHandler::clientStatus(Client* client, Byte* buffer, Int length)
{
	// Get the client's intent
	ClientStatusEventArgs e;
	e.client = client;
	e.action = (ClientStatusAction)VarInt(buffer).toInt();

	// Send the event to the server
	eventHandler->triggerEvent(&EventHandler::clientStatus, eventHandler, e);
}

/*****************************************************
 * NetworkHandler :: clientSettings                  *
 * The client is connecting or changing its settings *
 *****************************************************/
void NetworkHandler::clientSettings(Client* client, Byte* buffer, Int length)
{
	// Get the client's locale
	ClientSettingsEventArgs e;
	e.client = client;
	SerialString locale = SerialString(buffer);
	e.locale = locale.str();
	buffer += locale.getSize();

	// Get the client's view distance
	e.viewDistance = *(buffer++);

	// Get the client's chat mode
	VarInt chatMode = VarInt(buffer);
	e.chatMode = (ChatMode)chatMode.toInt();
	buffer += chatMode.getSize();

	// Get the client's chat colors setting
	e.chatColors = !!*(buffer++);

	// Get the client's displayed skin parts
	e.displayedSkinParts = DisplayedSkinParts(*(buffer++));

	// Get the client's main hand
	e.mainHand = (MainHand)VarInt(buffer).toInt();

	// Notify the server of the client's requested settings
	eventHandler->triggerEvent(&EventHandler::clientSettings, eventHandler, e);
}

/*******************************************************************
 * NetworkHandler :: confirmTransaction                            *
 * The client is apologizing for going against the server's wishes *
 *******************************************************************/
void NetworkHandler::confirmTransaction(Client* client, Byte* buffer, Int length)
{
	// Get the window id
	ConfirmTransactionEventArgs e;
	e.client = client;
	e.windowID = *(buffer++);

	// Get the action number
	e.actionNum = parseShort(buffer);
	buffer += 2;

	// Get whether the transaction was accepted
	e.accepted = !!*buffer;

	// Notify the server of the client's confirmation
	eventHandler->triggerEvent(&EventHandler::confirmTransaction, eventHandler, e);
}

/***************************************
 * NetworkHandler :: enchantItem       *
 * The client wants to enchant an item *
 ***************************************/
void NetworkHandler::enchantItem(Client* client, Byte* buffer, Int length)
{
	// Get the window id and enchantment
	EnchantItemEventArgs e;
	e.client = client;
	e.windowID = *(buffer++);
	e.enchantment = (EnchantmentPos)*buffer;

	// Notify the server that the client wants to enchant an item
	eventHandler->triggerEvent(&EventHandler::enchantItem, eventHandler, e);
}

/*******************************************
 * NetworkHandler :: clickWindow           *
 * The client is interacting with a window *
 *******************************************/
void NetworkHandler::clickWindow(Client* client, Byte* buffer, Int length)
{

}

/**************************************
 * NetworkHandler :: closeWindow      *
 * The client wants to close a window *
 **************************************/
void NetworkHandler::closeWindow(Client* client, Byte* buffer, Int length)
{
	// Get the window id
	CloseWindowEventArgs e;
	e.client = client;
	e.windowID = *buffer;

	// Tell the server the client wants to close the window
	eventHandler->triggerEvent(&EventHandler::closeWindow, eventHandler, e);
}

/**********************************************************************************
 * NetworkHandler :: pluginMessage                                                *
 * The client is sending a special message that may or may not be related to mods *
 **********************************************************************************/
void NetworkHandler::pluginMessage(Client* client, Byte* buffer, Int length)
{
	// Get the channel of the plugin message
	PluginMessageEventArgs* e = new PluginMessageEventArgs;
	e->client = client;
	SerialString channel = SerialString(buffer);
	e->channel = channel.str();
	buffer += channel.getSize();
	e->length = length - channel.getSize();

	// If the length is not long enough for the data then notify the server of an error
	if (e->length < 0)
	{
		InvalidLengthEventArgs e2;
		e2.client = client;
		e2.eventCause = "pluginMessage";
		e2.e = e;
		e2.length = e->length;
		eventHandler->triggerEvent(&EventHandler::invalidLength, eventHandler, e2);

		// Delete the event args when no longer used
		// TODO: Figure out a thread-safe way of doing this
		delete e;
		return;
	}

	// Get the message's data
	e->data = copyBuffer(buffer, e->length);

	// Delete the event args and buffer when no longer used
	// TODO: Figure out a thread-safe way of doing this
	delete[] e->data;
	delete e;
}

/*******************************************
 * NetworkHandler :: useEntity             *
 * The client right-clicked another entity *
 *******************************************/
void NetworkHandler::useEntity(Client* client, Byte* buffer, Int length)
{

}

/***********************************************************
 * NetworkHandler :: keepAlive                             *
 * The client is responding to the server's keepAlive ping *
 ***********************************************************/
void NetworkHandler::keepAlive(Client* client, Byte* buffer, Int length)
{

}

/*********************************************
 * NetworkHandler :: playerPositon           *
 * The client wants to update their position *
 *********************************************/
void NetworkHandler::playerPosition(Client* client, Byte* buffer, Int length)
{

}

/*******************************************************
 * NetworkHandler :: playerPositionAndLook             *
 * The client wants to update their position and angle *
 *******************************************************/
void NetworkHandler::playerPositionAndLook(Client* client, Byte* buffer, Int length)
{
	// TODO: Do this here!
	std::cout << client->name << ": Confirmed the spawn!\n";
}

/**************************************************
 * NetworkHandler :: playerLook                   *
 * The client wants to change their viewing angle *
 **************************************************/
void NetworkHandler::playerLook(Client* client, Byte* buffer, Int length)
{

}

/*******************************************
 * NetworkHandler :: playerOnGround        *
 * The client is either jumping or landing *
 *******************************************/
void NetworkHandler::playerOnGround(Client* client, Byte* buffer, Int length)
{

}

/**************************************
 * NetworkHandler :: vehicleMove      *
 * The client wants to move a vehicle *
 **************************************/
void NetworkHandler::vehicleMove(Client* client, Byte* buffer, Int length)
{

}

/***********************************************************
 * NetworkHandler :: steerBoat                             *
 * The client is telling us which boat paddles are turning *
 ***********************************************************/
void NetworkHandler::steerBoat(Client* client, Byte* buffer, Int length)
{

}

/*******************************************************************
 * NetworkHandler :: playerAbilities                               *
 * The client wants to take no damage, fly, or enter creative mode *
 *******************************************************************/
void NetworkHandler::playerAbilities(Client* client, Byte* buffer, Int length)
{

}

/******************************************************************************
 * NetworkHandler :: playerDigging                                            *
 * The client wants to one of dig, stop digging, finish digging, drop an item *
 * or stack, shoot an arrow, finish eating, or swap an item between its hands *
 ******************************************************************************/
void NetworkHandler::playerDigging(Client* client, Byte* buffer, Int length)
{

}

/*****************************************************************
* NetworkHandler :: entityAction                                 *
* The client wants to interact with an entity (including itself) *
******************************************************************/
void NetworkHandler::entityAction(Client* client, Byte* buffer, Int length)
{

}

/**************************************
 * NetworkHandler :: steerVehicle     *
 * The client wants to turn a vehicle *
 **************************************/
void NetworkHandler::steerVehicle(Client* client, Byte* buffer, Int length)
{

}

/*********************************************************************************
 * NetworkHandler :: resourcePackStatus                                          *
 * The client is giving back the status of a supposedly downloaded resource pack *
 *********************************************************************************/
void NetworkHandler::resourcePackStatus(Client* client, Byte* buffer, Int length)
{

}

/***********************************************
 * NetworkHandler :: heldItemChange            *
 * The client wants to select a different item *
 ***********************************************/
void NetworkHandler::heldItemChange(Client* client, Byte* buffer, Int length)
{

}

/*****************************************************************
 * NetworkHandler :: creativeInventoryAction                     *
 * The client wants to interact with the creative mode inventory *
 *****************************************************************/
void NetworkHandler::creativeInventoryAction(Client* client, Byte* buffer, Int length)
{

}

/************************************************
* NetworkHandler :: updateSign                  *
* The client wants to change the text of a sign *
*************************************************/
void NetworkHandler::updateSign(Client* client, Byte* buffer, Int length)
{

}

/****************************************
 * NetworkHandler :: animation          *
 * The client wants to animate its arms *
 ****************************************/
void NetworkHandler::animation(Client* client, Byte* buffer, Int length)
{
	// Interpret the data as an Int
	Int hand = VarInt(buffer).toInt();

	// Tell the client to animate its hand
//	eventHandler->animation(client, client->entityID, hand * 3);
}

/*****************************************
* NetworkHandler :: spectate             *
* The client wants to spectate an entity *
******************************************/
void NetworkHandler::spectate(Client* client, Byte* buffer, Int length)
{

}

/**********************************************
 * NetworkHandler :: playerBlockPlacement     *
 * The client wants to place a block          *
 **********************************************/
void NetworkHandler::playerBlockPlacement(Client* client, Byte* buffer, Int length)
{

}

/***********************************
 * NetworkHandler :: useItem       *
 * The client wants to use an item *
 ***********************************/
void NetworkHandler::useItem(Client* client, Byte* buffer, Int length)
{

}

/********************************************
 * NetworkHandler :: NetworkHandler         *
 * Default Constructor                      *
 ********************************************/
NetworkHandler::NetworkHandler(EventHandler* eventHandler)
	: running(false), eventHandler(eventHandler) {}

/********************************************
 * NetworkHandler :: NetworkHandler         *
 * Destructor                               *
 ********************************************/
NetworkHandler::~NetworkHandler() 
{
	// Stop the event handler
	stop();
}

/*************************************
 * NetworkHandler :: addClient       *
 * Handshakes with and adds a client *
 *************************************/
void NetworkHandler::addClient(SOCKET newClient)
{
	// Add the client to the list of clients
	eventHandler->clients.insert(std::make_pair(newClient, new Client(newClient)));
}

/****************************************************
 * NetworkHandler :: getClientFromSocket            *
 * Searches for a client using the given socket     *
 * It returns null if it could not find any clients *
 ****************************************************/
Client* NetworkHandler::getClientFromSocket(SOCKET socket)
{
	return (*eventHandler->clients.find(socket)).second;
}

/***********************************
 * NetworkHandler :: start         *
 * Starts up the client event loop *
 ***********************************/
void NetworkHandler::start()
{
	running = true;

	while (running)
	{
		// Create a list of every client to listen to
		fd_set clientList;
		if (eventHandler->clients.size() > 0)
		{
			int i = 0;
			for (std::map<SOCKET, Client*>::iterator it = eventHandler->clients.begin(); it != eventHandler->clients.end(); it++, i++)
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
		else
			Sleep(100); // TODO: Replace with conditional wakeup from Server->listenForClients
	}
}

/*********************************************************
 * NetworkHandler :: startAsync                          *
 * Starts up the client event loop on a different thread *
 *********************************************************/
void NetworkHandler::startAsync() { new std::thread(&NetworkHandler::start, this); }

/******************************
 * NetworkHandler :: stop     *
 * Terminates the event loop  *
 ******************************/
void NetworkHandler::stop() { running = false; }
