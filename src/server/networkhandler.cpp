#include "debug.h"
#include "server/networkhandler.h"
#include "server/eventhandler.h"
#include "data/networkpackets.h"
#include "data/bitstream.h"
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

/****************************
 * parseDouble              *
 * Reads a double from data *
 ****************************/
Double parseDouble(Byte* data)
{
	Long num = parseLong(data);
	return reinterpret_cast<Double&>(num);
}

/***************************
 * parseFloat              *
 * Reads a float from data *
 ***************************/
Float parseFloat(Byte* data)
{
	Int num = parseInt(data);
	return reinterpret_cast<Float&>(num);
}

/*************************
 * writeLong             *
 * Writes a long to data *
 *************************/
void writeLong(String& data, Long num)
{
	data.append(1, (num >> 56) & 0xFF);
	data.append(1, (num >> 48) & 0xFF);
	data.append(1, (num >> 40) & 0xFF);
	data.append(1, (num >> 32) & 0xFF);
	data.append(1, (num >> 24) & 0xFF);
	data.append(1, (num >> 16) & 0xFF);
	data.append(1, (num >> 8) & 0xFF);
	data.append(1, num & 0xFF);
}

/*************************
 * writeLong             *
 * Writes a long to data *
 *************************/
void writeLongB(String& data, Long num)
{
	data.append(1, (num >> 8) & 0xFF);
	data.append(1, (num >> 16) & 0xFF);
	data.append(1, (num >> 24) & 0xFF);
	data.append(1, (num >> 32) & 0xFF);
	data.append(1, (num >> 40) & 0xFF);
	data.append(1, (num >> 48) & 0xFF);
	data.append(1, (num >> 56) & 0xFF);
	data.append(1, num & 0xFF);
}

/*************************
 * writeInt              *
 * Writes an int to data *
 *************************/
void writeInt(String& data, Int num)
{
	data.append(1, (num >> 24) & 0xFF);
	data.append(1, (num >> 16) & 0xFF);
	data.append(1, (num >> 8) & 0xFF);
	data.append(1, num & 0xFF);
}

/**************************
 * writeShort             *
 * Writes a short to data *
 **************************/
void writeShort(String& data, Short num)
{
	data.append(1, (num >> 8) & 0xFF);
	data.append(1, num & 0xFF);
}

/***************************
 * writeDouble             *
 * Writes a double to data *
 ***************************/
// TODO: Verify cross-platform validity
void writeDouble(String& data, Double num)
{
	writeLong(data, reinterpret_cast<const Long&>(num));
}

/**************************
 * writeFloat             *
 * Writes a float to data *
 **************************/
// TODO: Verify cross-platform validity
void writeFloat(String& data, Float num)
{
	writeInt(data, reinterpret_cast<const Int&>(num));
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
	std::cout << client->getName() << ": 0x" << std::hex << packid << " - " << std::dec << len << " bytes\n";
	switch (client->getState())
	{
	case ServerState::Handshaking:
		// Check which packet the client was referring to
		switch (packid)
		{
		case (int)ClientHandshakePacket::Handshake:
			handShake(client, buf, len);
			break;
		case (int)ClientHandshakePacket::LegacyServerPing:
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
		case (int)ClientStatusPacket::Request:
			request(client, buf, len);
			break;
		case (int)ClientStatusPacket::Ping:
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
		case (int)ClientLoginPacket::LoginStart:
			loginStart(client, buf, len);
			break;
		case (int)ClientLoginPacket::EncryptionResponse:
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
		case (int)ClientPlayPacket::TeleportConfirm:
			teleportConfirm(client, buf, len);
			break;
		case (int)ClientPlayPacket::TabComplete:
			tabComplete(client, buf, len);
			break;
		case (int)ClientPlayPacket::ChatMessage:
			chatMessage(client, buf, len);
			break;
		case (int)ClientPlayPacket::ClientStatus:
			clientStatus(client, buf, len);
			break;
		case (int)ClientPlayPacket::ClientSettings:
			clientSettings(client, buf, len);
			break;
		case (int)ClientPlayPacket::ConfirmTransaction:
			confirmTransaction(client, buf, len);
			break;
		case (int)ClientPlayPacket::EnchantItem:
			enchantItem(client, buf, len);
			break;
		case (int)ClientPlayPacket::ClickWindow:
			clickWindow(client, buf, len);
			break;
		case (int)ClientPlayPacket::CloseWindow:
			closeWindow(client, buf, len);
			break;
		case (int)ClientPlayPacket::PluginMessage:
			pluginMessage(client, buf, len);
			break;
		case (int)ClientPlayPacket::UseEntity:
			useEntity(client, buf, len);
			break;
		case (int)ClientPlayPacket::KeepAlive:
			keepAlive(client, buf, len);
			break;
		case (int)ClientPlayPacket::PlayerPosition:
			playerPosition(client, buf, len);
			break;
		case (int)ClientPlayPacket::PlayerPositionAndLook:
			playerPositionAndLook(client, buf, len);
			break;
		case (int)ClientPlayPacket::PlayerLook:
			playerLook(client, buf, len);
			break;
		case (int)ClientPlayPacket::Player:
			playerOnGround(client, buf, len);
			break;
		case (int)ClientPlayPacket::VehicleMove:
			vehicleMove(client, buf, len);
			break;
		case (int)ClientPlayPacket::SteerBoat:
			steerBoat(client, buf, len);
			break;
		case (int)ClientPlayPacket::CraftRecipeRequest:
			craftRecipeRequest(client, buf, len);
			break;
		case (int)ClientPlayPacket::PlayerAbilities:
			playerAbilities(client, buf, len);
			break;
		case (int)ClientPlayPacket::PlayerDigging:
			playerDigging(client, buf, len);
			break;
		case (int)ClientPlayPacket::EntityAction:
			entityAction(client, buf, len);
			break;
		case (int)ClientPlayPacket::SteerVehicle:
			steerVehicle(client, buf, len);
			break;
		case (int)ClientPlayPacket::CraftingBookData:
			craftingBookData(client, buf, len);
			break;
		case (int)ClientPlayPacket::ResourcePackStatus:
			resourcePackStatus(client, buf, len);
			break;
		case (int)ClientPlayPacket::AdvancementTab:
			advancementTab(client, buf, len);
			break;
		case (int)ClientPlayPacket::HeldItemChange:
			heldItemChange(client, buf, len);
			break;
		case (int)ClientPlayPacket::CreativeInventoryAction:
			creativeInventoryAction(client, buf, len);
			break;
		case (int)ClientPlayPacket::UpdateSign:
			updateSign(client, buf, len);
			break;
		case (int)ClientPlayPacket::Animation:
			animation(client, buf, len);
			break;
		case (int)ClientPlayPacket::Spectate:
			spectate(client, buf, len);
			break;
		case (int)ClientPlayPacket::PlayerBlockPlacement:
			playerBlockPlacement(client, buf, len);
			break;
		case (int)ClientPlayPacket::UseItem:
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
		std::cout << "Error: Erroneous packet from " << client->getName()
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
	eventHandler->runOnServerThread(&EventHandler::invalidPacket, eventHandler, e);
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
	e.state = client->getState();
	eventHandler->runOnServerThread(&EventHandler::invalidState, eventHandler, e);
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
	eventHandler->runOnServerThread(&EventHandler::handshake, eventHandler, e);
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
	eventHandler->runOnServerThread(&EventHandler::legacyServerListPing, eventHandler, e);
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
	eventHandler->runOnServerThread(&EventHandler::request, eventHandler, e);
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
	eventHandler->runOnServerThread(&EventHandler::ping, eventHandler, e);
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
	e.name = name.str();

	// Prompt the server to let the client in
	eventHandler->runOnServerThread(&EventHandler::loginStart, eventHandler, e);
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
	EventHandler* handler = eventHandler;
	eventHandler->runOnServerThread([handler, e] () {
		handler->encryptionResponse(e);
		delete[] e.sharedSecret;
		delete[] e.verifyToken;
	});
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
	eventHandler->runOnServerThread(&EventHandler::teleportConfirm, eventHandler, e);
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
	eventHandler->runOnServerThread(&EventHandler::tabComplete, eventHandler, e);
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
	eventHandler->runOnServerThread(&EventHandler::chatMessage, eventHandler, e);
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
	eventHandler->runOnServerThread(&EventHandler::clientStatus, eventHandler, e);
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
	eventHandler->runOnServerThread(&EventHandler::clientSettings, eventHandler, e);
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
	eventHandler->runOnServerThread(&EventHandler::confirmTransaction, eventHandler, e);
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
	eventHandler->runOnServerThread(&EventHandler::enchantItem, eventHandler, e);
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
	eventHandler->runOnServerThread(&EventHandler::closeWindow, eventHandler, e);
}

/**********************************************************************************
 * NetworkHandler :: pluginMessage                                                *
 * The client is sending a special message that may or may not be related to mods *
 **********************************************************************************/
void NetworkHandler::pluginMessage(Client* client, Byte* buffer, Int length)
{
	// Get the channel of the plugin message
	PluginMessageEventArgs e;
	e.client = client;
	SerialString channel = SerialString(buffer);
	e.channel = channel.str();
	buffer += channel.getSize();
	e.length = length - channel.getSize();

	// If the length is not long enough for the data then notify the server of an error
	if (e.length < 0)
	{
		InvalidLengthEventArgs e2;
		e2.client = client;
		e2.eventCause = "pluginMessage";
		e2.e = &e;
		e2.length = e.length;
		eventHandler->runOnServerThread(&EventHandler::invalidLength, eventHandler, e2);
		return;
	}

	// Get the message's data
	e.data = copyBuffer(buffer, e.length);

	// Tell the server the client is sending a plugin message
	EventHandler* handler = eventHandler;
	eventHandler->runOnServerThread([handler, e]()
	{
		handler->pluginMessage(e);
		delete[] e.data;
	});
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
	// Parse the packet, a single int
	KeepAliveEventArgs e;
	e.client = client;
	e.id = parseInt(buffer);
	eventHandler->runOnServerThread(&EventHandler::keepAlive, eventHandler, e);
}

/*********************************************
 * NetworkHandler :: playerPositon           *
 * The client wants to update their position *
 *********************************************/
void NetworkHandler::playerPosition(Client* client, Byte* buffer, Int length)
{
	// Get the position
	PlayerPositionEventArgs e;
	e.client = client;
	e.position.x = parseDouble(buffer);
	e.position.y = parseDouble(buffer += 8);
	e.position.z = parseDouble(buffer += 8);

	// Determine if the client is on the floor
	e.onGround = !!*(buffer += 8);

	// Trigger the event
	eventHandler->runOnServerThread(&EventHandler::playerPosition, eventHandler, e);
}

/*******************************************************
 * NetworkHandler :: playerPositionAndLook             *
 * The client wants to update their position and angle *
 *******************************************************/
void NetworkHandler::playerPositionAndLook(Client* client, Byte* buffer, Int length)
{
	// Get the position
	PlayerPositionAndLookEventArgs e;
	e.client = client;
	e.position.x = parseDouble(buffer);
	e.position.y = parseDouble(buffer += 8);
	e.position.z = parseDouble(buffer += 8);

	// Get the head angle
	e.yaw = parseFloat(buffer += 8);
	e.pitch = parseFloat(buffer += 4);

	// Determine if the client is on the floor
	e.onGround = !!*(buffer += 4);

	// Trigger the event
	eventHandler->runOnServerThread(&EventHandler::playerPositionAndLook, eventHandler, e);
}

/**************************************************
 * NetworkHandler :: playerLook                   *
 * The client wants to change their viewing angle *
 **************************************************/
void NetworkHandler::playerLook(Client* client, Byte* buffer, Int length)
{
	// Get the head angle
	PlayerLookEventArgs e;
	e.client = client;
	e.yaw = parseFloat(buffer);
	e.pitch = parseFloat(buffer += 4);

	// Determine if the client is on the floor
	e.onGround = !!*(buffer += 4);

	// Trigger the event
	eventHandler->runOnServerThread(&EventHandler::playerLook, eventHandler, e);
}

/**************************************************************
 * NetworkHandler :: playerOnGround                           *
 * The client is stating whether it's in air or on/in a block *
 **************************************************************/
void NetworkHandler::playerOnGround(Client* client, Byte* buffer, Int length)
{
	// Determine if the client is on the floor
	PlayerOnGroundEventArgs e;
	e.client = client;
	e.onGround = !!*buffer;

	// Trigger the event
	eventHandler->runOnServerThread(&EventHandler::playerOnGround, eventHandler, e);
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

/******************************************
 * NetworkHandler :: craftRecipeRequest   *
 * The client is asking to craft a recipe *
 ******************************************/
void NetworkHandler::craftRecipeRequest(Client* client, Byte* buffer, Int length)
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

/*******************************************
 * NetworkHandler :: craftingBookData      *
 * The client is reading the crafting book *
 *******************************************/
void NetworkHandler::craftingBookData(Client* client, Byte* buffer, Int length)
{

}

/*********************************************************************************
 * NetworkHandler :: resourcePackStatus                                          *
 * The client is giving back the status of a supposedly downloaded resource pack *
 *********************************************************************************/
void NetworkHandler::resourcePackStatus(Client* client, Byte* buffer, Int length)
{

}

/************************************************
 * NetworkHandler :: advancementTab             *
 * The client is using the advancements tab...? *
 ************************************************/
void NetworkHandler::advancementTab(Client* client, Byte* buffer, Int length)
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

/***************************************************
 * NetworkHandler :: sendLoginSuccess              *
 * Tell the client that their login was successful *
 ***************************************************/
void NetworkHandler::sendLoginSuccess(Client* client, UUID uuid, String username)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerLoginPacket::LoginSuccess);
	SerialString suuid = SerialString(uuid.str());
	SerialString susername = SerialString(username);
	VarInt length = VarInt(packid.getSize() + suuid.getSize() + susername.getSize());
	data.reserve(length.toInt() + length.getSize());

	// Append the data to the string
	data.append((char*)length.getData(), length.getSize());
	data.append((char*)packid.getData(), packid.getSize());
	data.append(suuid.makeData(), suuid.getSize());
	data.append(susername.makeData(), susername.getSize());

	// Send the packet
	send(client->getSocket(), data.c_str(), data.size(), NULL);
}

/*************************************
 * NetworkHandler :: sendChatMessage *
 * Send a message to the client      *
 *************************************/
void NetworkHandler::sendChatMessage(Client* client, String message, ChatMessageType type, Boolean isJson)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::ChatMessage);
	SerialString smessage;

	// If the text is in JSON format then send it directly
	if (isJson)
		smessage = SerialString(message);
	// If the text is not in JSON then make it so.
	else
		smessage = SerialString(String("{ \"text\": \"") + message + String("\" }"));

	VarInt length = VarInt(packid.getSize() + smessage.getSize() + 1);
	data.reserve(length.toInt() + length.getSize());

	// Append the data to the string
	data.append((char*)length.getData(), length.getSize());
	data.append((char*)packid.getData(), packid.getSize());
	data.append(smessage.makeData(), smessage.getSize());
	data.append(1, (char)type);

	// Send the packet
	send(client->getSocket(), data.c_str(), data.size(), NULL);
}

/************************************
 * NetworkHandler :: sendJoinGame   *
 * Tell the client to join the game *
 ************************************/
void NetworkHandler::sendJoinGame(Client* client, Int entityID, Gamemode gamemode, Dimension dimension, Difficulty difficulty, Byte maxPlayers, LevelType levelType, Boolean reducedDebugInfo)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::JoinGame);
	SerialString slevelType = SerialString(levelType.str());
	VarInt length = VarInt(12 + slevelType.getSize() + packid.getSize());
	data.reserve(length.toInt() + length.getSize());

	// Append the data to the string
	data.append((char*)length.getData(), length.getSize());
	data.append((char*)packid.getData(), packid.getSize());
	writeInt(data, entityID);
	data.append(1, (char)gamemode);
	writeInt(data, (Int)dimension);
	data.append(1, (char)difficulty);
	data.append(1, maxPlayers);
	data.append(slevelType.makeData(), slevelType.getSize());
	data.append(1, (char)reducedDebugInfo);

	// Send the packet
	send(client->getSocket(), data.c_str(), data.size(), NULL);
}

/***************************************
 * NetworkHandler :: sendPluginMessage *
 * Send a plugin message to the client *
 ***************************************/
void NetworkHandler::sendPluginMessage(Client* client, String channel, Byte* data2, Int dataLen)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::PluginMessage);
	SerialString schannel = SerialString(channel);
	VarInt length = VarInt(packid.getSize() + schannel.getSize() + dataLen);
	data.reserve(length.toInt() + length.getSize());

	// Append the data to the string
	data.append((char*)length.getData(), length.getSize());
	data.append((char*)packid.getData(), packid.getSize());
	data.append(schannel.makeData(), schannel.getSize());
	data.append((char*)data2, dataLen);

	// Send the packet
	send(client->getSocket(), data.c_str(), data.size(), NULL);
}

/******************************************
 * NetworkHandler :: sendChunkColumn      *
 * Sends a column of chunks to the client *
 ******************************************/
void NetworkHandler::sendChunk(Client* client, Int x, Int z, ChunkColumn& column, Boolean createChunk, Boolean inOverworld)
{
	// Figure out which chunks are not empty and serialize their data
	int bitmask = 0;
	int counter = 0;
	String chunkdata = "";
	chunkdata.reserve(0x20000);
	for (int ch = 0; ch < 16; ++ch)
	{
		// If the chunk is filled then serialize its data using the global palette
		if (!column.chunks[ch].empty())
		{
			bitmask |= 1 << ch;
			counter++;

			/* TODO: Create and send a palette
			VarInt paletteLength = VarInt(0);
			chunkdata.append(1, MAX_BITS_PER_BLOCK);
			chunkdata.append((char*)paletteLength.getData(), paletteLength.getSize()); */

			/* Stuff the blocks into a global palette
			VarInt chunkdataSize = VarInt(64 * MAX_BITS_PER_BLOCK); // 64 * bitsize
			chunkdata.append((char*)chunkdataSize.getData(), chunkdataSize.getSize());
			BitStream bs;
			for (int i = 0; i < 4096; ++i)
				bs.push_back(column.chunks[ch].getBlockData(i), MAX_BITS_PER_BLOCK); */

			// Create and send a 1-block palette
			VarInt paletteLength = VarInt(1);
			VarInt cobblePalette = VarInt((Int)BlockID::TNT << 4);
			chunkdata.append(1, 1);
			chunkdata.append((char*)paletteLength.getData(), paletteLength.getSize());
			chunkdata.append((char*)cobblePalette.getData(), cobblePalette.getSize());

			// Send all of the blocks from that palette
			VarInt chunkdataSize = VarInt(64); // 64 * bitsize
			chunkdata.append((char*)chunkdataSize.getData(), chunkdataSize.getSize());

			// Insert the block data
//			BitStream bs;
//			for (int i = 0; i < 64; ++i)
//				bs << (Long)0xFFFFFFFFFFFFFFFF;
			chunkdata.append(512, '\xff');
//			chunkdata.append(bs.str());

			// Send the block light data
			for (int i = 0; i < 4096; i += 2)
				chunkdata.append(1, (char)(column.chunks[ch].getBlockLighting(i + 1) << 4 | column.chunks[ch].getBlockLighting(i)));

			// Send the sky light data if we're sending an overworld chunk
			if (inOverworld)
				for (int i = 0; i < 4096; i += 2)
					chunkdata.append(1, (char)(column.chunks[ch].getSkyLighting(i + 1) << 4 | column.chunks[ch].getSkyLighting(i)));
		}
	}

	// If we're creating a chunk then send the biome data
	if (createChunk)
	{
		// If the biome data is empty then initialize it to the void
		if (column.noBiomes())
			column.fillBiomes();

		// Send the biome data
		chunkdata.append((char*)&column.getBiome(0), 256);
	}

	// TODO: serialize block entities

	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::ChunkData);
	VarInt sbitmask = VarInt(bitmask);
	VarInt columndatasize = VarInt(chunkdata.size());
	VarInt numBlockEntities = VarInt(0);
	VarInt length = VarInt(packid.getSize() + sbitmask.getSize() + columndatasize.getSize() + numBlockEntities.getSize() + chunkdata.size() + 9);
	data.reserve(length.toInt() + length.getSize());
	
	// Append the data to a string
	data.append((char*)length.getData(), length.getSize());
	data.append((char*)packid.getData(), packid.getSize());
	writeInt(data, x);
	writeInt(data, z);
	data.append(1, (char)createChunk);
	data.append((char*)sbitmask.getData(), sbitmask.getSize());
	data.append((char*)columndatasize.getData(), columndatasize.getSize());
	data.append(chunkdata);
	data.append((char*)numBlockEntities.getData(), numBlockEntities.getSize());

	std::cout << length.toInt() << ", " << data.size() << ", " << counter << "\n";

	// Send the data
	send(client->getSocket(), data.c_str(), data.size(), NULL);

	// Register the loaded chunk into the client's data
	eventHandler->runOnServerThread([client, x, z]() {
		client->loadedChunks.insert(std::pair<Int, Int>(x, z));
	});
}

/******************************************
 * NetworkHandler :: sendChunkColumn      *
 * Sends a column of chunks to the client *
 ******************************************/
void NetworkHandler::sendChunk(Client* client, Int x, Int z, Boolean createChunk, Boolean inOverworld)
{
	// Grab the chunk on the server's clock thread
	EventHandler* handler = eventHandler;
	NetworkHandler* nethandler = this;
	handler->runOnServerThread([handler, nethandler, client, x, z, createChunk, inOverworld]
	{
		// Get the chunk column before sending it
		GetChunkEventArgs e;
		e.client = client;
		e.x = x;
		e.z = z;
		handler->getChunk(e);

		// If we're creating a chunk then grab its biome data
		if (createChunk)
		{
			// Create the biome array if needed
			if (e.chunk.noBiomes())
				e.chunk.fillBiomes();

			// Get the biome data
			GetBiomeEventArgs e2;
			e2.client = client;
			e2.x = x;
			e2.z = z;
			e2.biomes = &e.chunk.getBiome(0);
			handler->getBiomes(e2);
		}

		// Send the chunk column
		nethandler->sendChunk(client, x, z, e.chunk, createChunk, inOverworld);
	});
}


/*******************************************
 * NetworkHandler :: sendServerDifficulty  *
 * Tell the client the server's difficulty *
 *******************************************/
void NetworkHandler::sendServerDifficulty(Client* client, Difficulty difficulty)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::Difficulty);
	VarInt length = VarInt(packid.getSize() + 1);
	data.reserve(length.toInt() + length.getSize());

	// Append the data to the string
	data.append((char*)length.getData(), length.getSize());
	data.append((char*)packid.getData(), packid.getSize());
	data.append(1, (char)difficulty);

	// Send the packet
	send(client->getSocket(), data.c_str(), data.size(), NULL);
}

/*********************************************
 * NetworkHandler :: sendSpawnPosition       *
 * Notify the client where the spawnpoint is *
 *********************************************/
void NetworkHandler::sendSpawnPosition(Client* client, Position pos)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::SpawnPosition);
	SerialPosition spos = SerialPosition(pos);
	VarInt length = VarInt(packid.getSize() + 8);
	data.reserve(length.toInt() + length.getSize());

	// Append the data to the string
	data.append((char*)length.getData(), length.getSize());
	data.append((char*)packid.getData(), packid.getSize());
	writeLong(data, spos.getData());

	// Send the packet
	send(client->getSocket(), data.c_str(), data.size(), NULL);
}

/********************************************
 * NetworkHandler :: sendPlayerAbilities    *
 * Tells the client what it's allowed to do *
 ********************************************/
void NetworkHandler::sendPlayerAbilities(Client* client, PlayerAbilities abilities, Float flyingSpeed, Float fovModifier)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::PlayerAbilities);
	VarInt length = VarInt(packid.getSize() + 9);
	data.reserve(length.toInt() + length.getSize());

	// Append the data to the string
	data.append((char*)length.getData(), length.getSize());
	data.append((char*)packid.getData(), packid.getSize());
	data.append(1, abilities.getFlags());
	writeFloat(data, flyingSpeed);
	writeFloat(data, fovModifier);

	// Send the packet
	send(client->getSocket(), data.c_str(), data.size(), NULL);
}

/********************************************
 * NetworkHandler :: sendPlayerAbilities    *
 * Tells the client what it's allowed to do *
 ********************************************/
void NetworkHandler::sendPlayerPositionAndLook(Client* client, PositionF pos, Float yaw, Float pitch, PlayerPositionAndLookFlags flags, Int teleportID)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::PlayerPositionAndLook);
	VarInt steleportID = VarInt(teleportID);
	VarInt length = VarInt(packid.getSize() + steleportID.getSize() + 33);
	data.reserve(length.toInt() + length.getSize());

	// Append the data to the string
	data.append((char*)length.getData(), length.getSize());
	data.append((char*)packid.getData(), packid.getSize());
	writeDouble(data, pos.x);
	writeDouble(data, pos.y);
	writeDouble(data, pos.z);
	writeFloat(data, yaw);
	writeFloat(data, pitch);
	data.append(1, flags.getFlags());
	data.append((char*)steleportID.getData(), steleportID.getSize());

	// Send the packet
	send(client->getSocket(), data.c_str(), data.size(), NULL);
}

/*********************************************
 * NetworkHandler :: sendUnloadChunk         *
 * Tell the client to unload the given chunk *
 *********************************************/
void NetworkHandler::sendUnloadChunk(Client* client, Int x, Int z)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::UnloadChunk);
	VarInt length = VarInt(packid.getSize() + 8);
	data.reserve(length.toInt() + length.getSize());

	// Append the data to the string
	data.append((char*)length.getData(), length.getSize());
	data.append((char*)packid.getData(), packid.getSize());
	writeInt(data, x);
	writeInt(data, z);

	// Send the packet
	send(client->getSocket(), data.c_str(), data.size(), NULL);

	// Unregister the chunk from the client's data
	eventHandler->runOnServerThread([client, x, z]() {
		client->loadedChunks.erase(std::pair<Int, Int>(x, z));
	});
}

/***********************************
 * NetworkHandler :: sendKeepAlive *
 * Prompt the client to respond    *
 ***********************************/
void NetworkHandler::sendKeepAlive(Client* client, Long id)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::KeepAlive);
	VarInt length = VarInt(packid.getSize() + 8);
	data.reserve(length.toInt() + length.getSize());

	// Append the data to the string
	data.append((char*)length.getData(), length.getSize());
	data.append((char*)packid.getData(), packid.getSize());
	writeLong(data, id);

	// Send the packet
	send(client->getSocket(), data.c_str(), data.size(), NULL);
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
	// TODO: Make thread-safe
	eventHandler->clients.insert(new Client(newClient));
}

/**************************************
 * NetworkHandler :: disconnectClient *
 * Disconnects and deletes a client   *
 **************************************/
void NetworkHandler::disconnectClient(Client* client)
{
	// Disconnect the client's socket
	shutdown(client->getSocket(), SD_BOTH);
	closesocket(client->getSocket());

	// Trigger the client disconnected event so that the event handler can clean up the client's data.
	ClientDisconnectEventArgs e;
	e.client = client;
	eventHandler->runOnServerThread(&EventHandler::clientDisconnect, eventHandler, e);
}

/****************************************************
 * NetworkHandler :: getClientFromSocket            *
 * Searches for a client using the given socket     *
 * It returns null if it could not find any clients *
 ****************************************************/
Client* NetworkHandler::getClientFromSocket(SOCKET socket)
{
	// TODO: Make thread-safe
	Client temp = Client(socket);
	return *eventHandler->clients.find(&temp);
}

/***********************************
 * NetworkHandler :: start         *
 * Starts up the client event loop *
 ***********************************/
#define BUFFER_SIZE 65536
void NetworkHandler::start()
{
	running = true;
	char buf[BUFFER_SIZE];

	while (running)
	{
		// Create a list of every client to listen to
		// TODO: Make eventHandler->clients thread-safe
		fd_set clientList;
		if (eventHandler->clients.size() > 0)
		{
			int i = 0;
			for (AtomicSet<Client*, ClientComparator>::iterator it = eventHandler->clients.begin(); it != eventHandler->clients.end(); it++, i++)
				clientList.fd_array[i] = (*it)->getSocket();
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
					int dataRead = recv(client->getSocket(), buf, BUFFER_SIZE, NULL);
					if (dataRead == SOCKET_ERROR)
					{
						std::cout << "Error reading data from " << client->getName()
							<< ": " << WSAGetLastError() << "\n";
						disconnectClient(client);
					}
					else if (dataRead > 0)
					{
						// Attempt to read the varPack
						readPacket(client, (Byte*)buf, dataRead);
					}
					// The client is not sending bytes... it disconnected.
					else
					{
						disconnectClient(client);
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
