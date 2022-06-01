#include "networkhandler.h"
#include "eventhandler.h"
#include "networkpackets.h"
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

/**************************************************
 * copyBuffer                                     *
 * Creates a byte array from another array        *
 **************************************************/
std::unique_ptr<Byte[]> copyBuffer(const Byte* data, Int length)
{
	// Copy the contents of the buffer to a new array
	Byte* buf = new Byte[length];
	for (int i = 0; i < length; i++)
		buf[i] = data[i];
	return std::unique_ptr<Byte[]>(buf);
}

/**************************
 * parseLong              *
 * Reads a long from data *
 **************************/
Long parseLong(const Byte* data)
{
	return (((Long)data[0]) << 56) + (((Long)data[1]) << 48) + (((Long)data[2]) << 40) +
		   (((Long)data[3]) << 32) + (data[4] << 24) + (data[5] << 16) + (data[6] << 8) + data[7];
}

/**************************
 * parseInt               *
 * Reads an int from data *
 **************************/
Int parseInt(const Byte* data)
{
	return (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];
}

/***************************
 * parseShort              *
 * Reads a short from data *
 ***************************/
Short parseShort(const Byte* data)
{
	return (data[0] << 8) + data[1];
}

/****************************
 * parseDouble              *
 * Reads a double from data *
 ****************************/
Double parseDouble(const Byte* data)
{
	Long num = parseLong(data);
	return reinterpret_cast<Double&>(num);
}

/***************************
 * parseFloat              *
 * Reads a float from data *
 ***************************/
Float parseFloat(const Byte* data)
{
	Int num = parseInt(data);
	return reinterpret_cast<Float&>(num);
}

/*************************
 * writeLong             *
 * Writes a long to data *
 *************************/
void writeLong(String& data, const Long num)
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
void writeLongB(String& data, const Long num)
{
	data.append(1, num & 0xFF);
	data.append(1, (num >> 8) & 0xFF);
	data.append(1, (num >> 16) & 0xFF);
	data.append(1, (num >> 24) & 0xFF);
	data.append(1, (num >> 32) & 0xFF);
	data.append(1, (num >> 40) & 0xFF);
	data.append(1, (num >> 48) & 0xFF);
	data.append(1, (num >> 56) & 0xFF);
}

/*************************
 * writeInt              *
 * Writes an int to data *
 *************************/
void writeInt(String& data, const Int num)
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
void writeShort(String& data, const Short num)
{
	data.append(1, (num >> 8) & 0xFF);
	data.append(1, num & 0xFF);
}

/***************************
 * writeDouble             *
 * Writes a double to data *
 ***************************/
// TODO: Verify cross-platform validity
void writeDouble(String& data, const Double num)
{
	writeLong(data, reinterpret_cast<const Long&>(num));
}

/**************************
 * writeFloat             *
 * Writes a float to data *
 **************************/
// TODO: Verify cross-platform validity
void writeFloat(String& data, const Float num)
{
	writeInt(data, reinterpret_cast<const Int&>(num));
}

/*************************************************************
 * NetworkHandler :: readPacket                              *
 * Reads the packet and fires off any events it gets from it *
 *************************************************************/
void NetworkHandler::readPacket(LockedClient& client, std::unique_ptr<Byte[]> buffer, Int length)
{
	// Read the length of the packet
	const Byte* buf = buffer.get();
	VarInt varLen = VarInt(buf);

	// Read the packet's id
	buf += varLen.size();
	VarInt varPack = VarInt(buf);

	// Calculate the length of the buffer minus the length
	// of the length and packet id variables themselves
	Int len = varLen.toInt() - varLen.size() - varPack.size();
	buf += varPack.size();

	// Check which state the client is currently in
	// Assumes nothing is editing the client's state so it reads without blocking
	// (This'll cause a data race if that is not actually true)
	int packid = varPack.toInt();
	std::cout << client.client->name << ": 0x" << std::hex << packid << " - " << std::dec << len << " bytes\n";
	switch (client.client->state)
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
		invalidState(client, buf, len, packid);
		break;
	}

	// Assume what's left over in the packet is another packet
	Int tempLen = len + varPack.size() + (varLen.size() * 2);

	// If the packet's length is smaller than reality allows then complain
	// TODO: Verify that this is correct vs. waiting for the next packet??
	if (tempLen < 1)
	{
		invalidLength(client, buf, tempLen, length, packid);
	}
	else if (tempLen < length)
	{
		// Create another array for the next packet
		// TODO: Save on copying data maybe?
		buf = buffer.get() + tempLen;
		length -= tempLen;
		Byte* tempBuf = new Byte[length];
		for (int i = 0; i < length; i++)
			tempBuf[i] = buf[i];

		// Read the next packet
		readPacket(client, std::unique_ptr<Byte[]>(tempBuf), length);
	}
}

/*************************************
 * NetworkHandler :: invalidPacket   *
 * The client sent an invalid packet *
 *************************************/
void NetworkHandler::invalidPacket(LockedClient& client, const Byte* buffer, Int length, Int packet)
{
	// Tell the server that the client is in an invalid state
	eventHandler->invalidPacket(InvalidPacketEventArgs(client.client, client.mutex.get(), packet, length, buffer));
}

/*************************************
 * NetworkHandler :: invalidState    *
 * The client is in an invalid state *
 *************************************/
void NetworkHandler::invalidState(LockedClient& client, const Byte* buffer, Int length, Int packet)
{
	// Tell the server that the client is in an invalid state
	eventHandler->invalidState(InvalidPacketEventArgs(client.client, client.mutex.get(), packet, length, buffer));
}

/***************************************************
 * NetworkHandler :: invalidLength                 *
 * The client sent a packet with an invalid length *
 ***************************************************/
void NetworkHandler::invalidLength(LockedClient& client, const Byte* buffer, Int length, Int actualLength, Int packet)
{
	// Tell the server that the client is in an invalid state
	eventHandler->invalidLength(InvalidLengthEventArgs(client.client, client.mutex.get(), length, actualLength, packet, buffer));
}

/***********************************
 * NetworkHandler :: handShake     *
 * Greet an oncoming client        *
 ***********************************/
void NetworkHandler::handShake(LockedClient& client, const Byte* buffer, Int length)
{
	// Read the protocol from the buffer
	VarInt protocol = VarInt(buffer);
	buffer += protocol.size();

	// Read the server address and port
	SerialString address = SerialString(buffer);
	buffer += address.size();
	UShort serverPort = parseShort(buffer);
	buffer += 2; // 2 bytes (short)

	// Read the current state and trigger the server's handshake event
	ServerState state = (ServerState)VarInt(buffer).toInt();
	eventHandler->handshake(HandShakeEventArgs(client.client, client.mutex.get(), state, serverPort, protocol.toInt(), address.str()));
}

/****************************************
* NetworkHandler :: handShakeLegacy     *
* Greet an old oncoming client          *
*****************************************/
void NetworkHandler::handShakeLegacy(LockedClient& client, const Byte* buffer, Int length)
{
	// Trigger the server's legacy server list ping event (buffer is just a pointer to a byte)
	eventHandler->legacyServerListPing(LegacyServerListPingEventArgs(client.client, client.mutex.get(), *buffer));
}

/*****************************************
 * NetworkHandler :: request             *
 * The client wants the server's details *
 *****************************************/
void NetworkHandler::request(LockedClient& client, const Byte* buffer, Int length)
{
	// Alert the server of the client's request
	eventHandler->request(RequestEventArgs(client.client, client.mutex.get()));
}

/************************************
 * NetworkHandler :: ping           *
 * The client is pinging the server *
 ************************************/
void NetworkHandler::ping(LockedClient& client, const Byte* buffer, Int length)
{
	// Alert the server of the client's ping
	eventHandler->ping(PingEventArgs(client.client, client.mutex.get(), parseLong(buffer)));
}


/***************************************
 * NetworkHandler :: loginStart        *
 * The client wants to join the server *
 ***************************************/
void NetworkHandler::loginStart(LockedClient& client, const Byte* buffer, Int length)
{
	// Get the player's username
	SerialString name = SerialString(buffer);

	// Prompt the server to let the client in
	eventHandler->loginStart(LoginStartEventArgs(client.client, client.mutex.get(), name.str()));
}


/************************************************************************
 * NetworkHandler :: encryptionResponse                                 *
 * The client is either accepting or denying the request for encryption *
 ************************************************************************/
void NetworkHandler::encryptionResponse(LockedClient& client, const Byte* buffer, Int length)
{
	// Get the shared secret's length
	VarInt sharedSecretLen = VarInt(buffer);
	Int sharedSecretLength = sharedSecretLen.toInt();
	buffer += sharedSecretLen.size();

	// Get the shared secret's data
	std::unique_ptr<Byte[]> secret = copyBuffer(buffer, sharedSecretLength);
	buffer += sharedSecretLength;

	// Get the verify token's length
	VarInt verifyTokenLen = VarInt(buffer);
	Int verifyTokenLength = verifyTokenLen.toInt();
	buffer += verifyTokenLen.size();

	// Get the verify token's data
	std::unique_ptr<Byte[]> token = copyBuffer(buffer, verifyTokenLength);
	
	// Notify the server of the client's response
	EventHandler* handler = eventHandler;
	eventHandler->encryptionResponse(EncryptionResponseEventArgs(client.client, client.mutex.get(), secret.get(), sharedSecretLength, token.get(), verifyTokenLength));
}

/*****************************************
 * NetworkHandler :: teleportConfirm     *
 * The client knows we teleported it     *
 *****************************************/
void NetworkHandler::teleportConfirm(LockedClient& client, const Byte* buffer, Int length)
{
	// Alert the server of the client's confirmation
	eventHandler->teleportConfirm(TeleportConfirmEventArgs(client.client, client.mutex.get(), VarInt(buffer).toInt()));
}

/****************************************************
 * NetworkHandler :: tabComplete                    *
 * The client wants us to complete the chat command *
 ****************************************************/
void NetworkHandler::tabComplete(LockedClient& client, const Byte* buffer, Int length)
{
	// Get the text
	SerialString text = SerialString(buffer);
	buffer += text.size();

	// Get assumeCommand and hasPosition
	Boolean assumeCommand = !!*(buffer++);
	Boolean hasPosition = !!*(buffer++);
	Position lookedAtBlock;

	// If there was a position given, get the coordinates
	if (hasPosition)
		lookedAtBlock = SerialPosition(parseLong(buffer)).toPosition();

	// Send the information to the server
	eventHandler->tabComplete(TabCompleteEventArgs(client.client, client.mutex.get(), assumeCommand, hasPosition, text.str(), lookedAtBlock));
}

/*****************************************
 * NetworkHandler :: chatMessage         *
 * The client is sending a chat commmand *
 *****************************************/
void NetworkHandler::chatMessage(LockedClient& client, const Byte* buffer, Int length)
{
	// Get the text
	String message = SerialString(buffer).str();

	// Send the data to the server for interpreting / broadcasting
	eventHandler->chatMessage(ChatMessageEventArgs(client.client, client.mutex.get(), message));
}

/************************************************************************
 * NetworkHandler :: clientStatus                                       *
 * The client is respawning, opening its inventory, or requesting stats *
 ************************************************************************/
void NetworkHandler::clientStatus(LockedClient& client, const Byte* buffer, Int length)
{
	// Get the client's intent
	ClientStatusAction action = (ClientStatusAction)VarInt(buffer).toInt();

	// Send the event to the server
	eventHandler->clientStatus(ClientStatusEventArgs(client.client, client.mutex.get(), action));
}

/*****************************************************
 * NetworkHandler :: clientSettings                  *
 * The client is connecting or changing its settings *
 *****************************************************/
void NetworkHandler::clientSettings(LockedClient& client, const Byte* buffer, Int length)
{
	// Get the client's locale
	SerialString locale = SerialString(buffer);
	buffer += locale.size();

	// Get the client's view distance
	Byte viewDistance = *(buffer++);

	// Get the client's chat mode
	VarInt chatMode = VarInt(buffer);
	buffer += chatMode.size();

	// Get the client's chat colors setting
	Boolean chatColors = !!*(buffer++);

	// Get the client's displayed skin parts
	DisplayedSkinParts displayedSkinParts = DisplayedSkinParts(*(buffer++));

	// Get the client's main hand
	MainHand mainHand = (MainHand)VarInt(buffer).toInt();

	// Notify the server of the client's requested settings
	eventHandler->clientSettings(ClientSettingsEventArgs(client.client, client.mutex.get(), chatColors, mainHand, viewDistance, (ChatMode)chatMode.toInt(), displayedSkinParts, locale.str()));
}

/*******************************************************************
 * NetworkHandler :: confirmTransaction                            *
 * The client is apologizing for going against the server's wishes *
 *******************************************************************/
void NetworkHandler::confirmTransaction(LockedClient& client, const Byte* buffer, Int length)
{
	// Get the window id
	Byte windowID = *(buffer++);

	// Get the action number
	Short actionNum = parseShort(buffer);
	buffer += 2;

	// Get whether the transaction was accepted
	Boolean accepted = !!*buffer;

	// Notify the server of the client's confirmation
	eventHandler->confirmTransaction(ConfirmTransactionEventArgs(client.client, client.mutex.get(), accepted, windowID, actionNum));
}

/***************************************
 * NetworkHandler :: enchantItem       *
 * The client wants to enchant an item *
 ***************************************/
void NetworkHandler::enchantItem(LockedClient& client, const Byte* buffer, Int length)
{
	// Get the window id and enchantment
	Byte windowID = *(buffer++);
	EnchantmentPos position = (EnchantmentPos)*buffer;

	// Notify the server that the client wants to enchant an item
	eventHandler->enchantItem(EnchantItemEventArgs(client.client, client.mutex.get(), position, windowID));
}

/*******************************************
 * NetworkHandler :: clickWindow           *
 * The client is interacting with a window *
 *******************************************/
void NetworkHandler::clickWindow(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement clickWindow
}

/**************************************
 * NetworkHandler :: closeWindow      *
 * The client wants to close a window *
 **************************************/
void NetworkHandler::closeWindow(LockedClient& client, const Byte* buffer, Int length)
{
	// Tell the server the client wants to close the window (buffer is just a pointer to a byte)
	eventHandler->closeWindow(CloseWindowEventArgs(client.client, client.mutex.get(), *buffer));
}

/**********************************************************************************
 * NetworkHandler :: pluginMessage                                                *
 * The client is sending a special message that may or may not be related to mods *
 **********************************************************************************/
void NetworkHandler::pluginMessage(LockedClient& client, const Byte* buffer, Int length)
{
	// Get the channel of the plugin message
	SerialString channel = SerialString(buffer);
	Int channelLength = length - channel.size();

	// If the length is not long enough for the data then notify the server of an error
	if (channelLength < 0)
	{
		eventHandler->invalidLength(InvalidLengthEventArgs(client.client, client.mutex.get(), length - channelLength, length, (Int)ClientPlayPacket::PluginMessage, buffer));
		return;
	}

	// Get the message's data
	buffer += channel.size();
	auto data = copyBuffer(buffer, channelLength);

	// Tell the server the client is sending a plugin message
	eventHandler->pluginMessage(PluginMessageEventArgs(client.client, client.mutex.get(), channelLength, channel.str(), data.get()));
}

/*******************************************
 * NetworkHandler :: useEntity             *
 * The client right-clicked another entity *
 *******************************************/
void NetworkHandler::useEntity(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement useEntity
}

/***********************************************************
 * NetworkHandler :: keepAlive                             *
 * The client is responding to the server's keepAlive ping *
 ***********************************************************/
void NetworkHandler::keepAlive(LockedClient& client, const Byte* buffer, Int length)
{
	// Inform the server that the client is trying to remain connected
	eventHandler->keepAlive(KeepAliveEventArgs(client.client, client.mutex.get(), parseLong(buffer)));
}

/*********************************************
 * NetworkHandler :: playerPositon           *
 * The client wants to update their position *
 *********************************************/
void NetworkHandler::playerPosition(LockedClient& client, const Byte* buffer, Int length)
{
	// Get the position
	Double x = parseDouble(buffer);
	Double y = parseDouble(buffer += 8);
	Double z = parseDouble(buffer += 8);

	// Determine if the client is on the floor
	Boolean onGround = !!*(buffer += 8);

	// Inform the server
	eventHandler->playerMoved(PlayerMovedEventArgs(client.client, client.mutex.get(), onGround, PositionF(x, y, z)));
}

/*******************************************************
 * NetworkHandler :: playerPositionAndLook             *
 * The client wants to update their position and angle *
 *******************************************************/
void NetworkHandler::playerPositionAndLook(LockedClient& client, const Byte* buffer, Int length)
{
	// Get the position
	Double x = parseDouble(buffer);
	Double y = parseDouble(buffer += 8);
	Double z = parseDouble(buffer += 8);

	// Get the head angle
	Float yaw = parseFloat(buffer += 8);
	Float pitch = parseFloat(buffer += 4);

	// Determine if the client is on the floor
	Boolean onGround = !!*(buffer += 4);

	// Inform the server
	eventHandler->playerMoved(PlayerMovedEventArgs(client.client, client.mutex.get(), onGround, PositionF(x, y, z), yaw, pitch));
}

/**************************************************
 * NetworkHandler :: playerLook                   *
 * The client wants to change their viewing angle *
 **************************************************/
void NetworkHandler::playerLook(LockedClient& client, const Byte* buffer, Int length)
{
	// Get the head angle
	Float yaw = parseFloat(buffer);
	Float pitch = parseFloat(buffer += 4);

	// Determine if the client is on the floor
	Boolean onGround = !!*(buffer += 4);

	// Inform the server
	eventHandler->playerMoved(PlayerMovedEventArgs(client.client, client.mutex.get(), onGround, yaw, pitch));
}

/**************************************************************
 * NetworkHandler :: playerOnGround                           *
 * The client is stating whether it's in air or on/in a block *
 **************************************************************/
void NetworkHandler::playerOnGround(LockedClient& client, const Byte* buffer, Int length)
{
	// Inform the server
	eventHandler->playerMoved(PlayerMovedEventArgs(client.client, client.mutex.get(), !!*buffer));
}

/**************************************
 * NetworkHandler :: vehicleMove      *
 * The client wants to move a vehicle *
 **************************************/
void NetworkHandler::vehicleMove(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement vehicleMove
}

/***********************************************************
 * NetworkHandler :: steerBoat                             *
 * The client is telling us which boat paddles are turning *
 ***********************************************************/
void NetworkHandler::steerBoat(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement steerBoat
}

/******************************************
 * NetworkHandler :: craftRecipeRequest   *
 * The client is asking to craft a recipe *
 ******************************************/
void NetworkHandler::craftRecipeRequest(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement craftRecipeRequest
}

/*******************************************************************
 * NetworkHandler :: playerAbilities                               *
 * The client wants to take no damage, fly, or enter creative mode *
 *******************************************************************/
void NetworkHandler::playerAbilities(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement playerAbilities
}

/******************************************************************************
 * NetworkHandler :: playerDigging                                            *
 * The client wants to one of dig, stop digging, finish digging, drop an item *
 * or stack, shoot an arrow, finish eating, or swap an item between its hands *
 ******************************************************************************/
void NetworkHandler::playerDigging(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement playerDigging
}

/*****************************************************************
* NetworkHandler :: entityAction                                 *
* The client wants to interact with an entity (including itself) *
******************************************************************/
void NetworkHandler::entityAction(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement entityAction
}

/**************************************
 * NetworkHandler :: steerVehicle     *
 * The client wants to turn a vehicle *
 **************************************/
void NetworkHandler::steerVehicle(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement steerVehicle
}

/*******************************************
 * NetworkHandler :: craftingBookData      *
 * The client is reading the crafting book *
 *******************************************/
void NetworkHandler::craftingBookData(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement craftinBookData
}

/*********************************************************************************
 * NetworkHandler :: resourcePackStatus                                          *
 * The client is giving back the status of a supposedly downloaded resource pack *
 *********************************************************************************/
void NetworkHandler::resourcePackStatus(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement resourcePackStatus
}

/************************************************
 * NetworkHandler :: advancementTab             *
 * The client is using the advancements tab...? *
 ************************************************/
void NetworkHandler::advancementTab(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement advancementTab
}

/***********************************************
 * NetworkHandler :: heldItemChange            *
 * The client wants to select a different item *
 ***********************************************/
void NetworkHandler::heldItemChange(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement heldItemChange
}

/*****************************************************************
 * NetworkHandler :: creativeInventoryAction                     *
 * The client wants to interact with the creative mode inventory *
 *****************************************************************/
void NetworkHandler::creativeInventoryAction(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement creativeInventoryAction
}

/************************************************
* NetworkHandler :: updateSign                  *
* The client wants to change the text of a sign *
*************************************************/
void NetworkHandler::updateSign(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement updateSign
}

/****************************************
 * NetworkHandler :: animation          *
 * The client wants to animate its arms *
 ****************************************/
void NetworkHandler::animation(LockedClient& client, const Byte* buffer, Int length)
{
	// Interpret the data as an Int
	Hand hand = (Hand)VarInt(buffer).toInt();

	// Inform the server of the animation
	eventHandler->handAnimation(HandAnimationEventArgs(client.client, client.mutex.get(), hand));
}

/*****************************************
* NetworkHandler :: spectate             *
* The client wants to spectate an entity *
******************************************/
void NetworkHandler::spectate(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement spectate
}

/**********************************************
 * NetworkHandler :: playerBlockPlacement     *
 * The client wants to place a block          *
 **********************************************/
void NetworkHandler::playerBlockPlacement(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement playerBlockPlacement
}

/***********************************
 * NetworkHandler :: useItem       *
 * The client wants to use an item *
 ***********************************/
void NetworkHandler::useItem(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implemen useItem
}

/***************************************************
 * NetworkHandler :: sendLoginSuccess              *
 * Tell the client that their login was successful *
 ***************************************************/
void NetworkHandler::sendLoginSuccess(const Client* client, UUID uuid, String username)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerLoginPacket::LoginSuccess);
	SerialString suuid = SerialString(uuid.str());
	SerialString susername = SerialString(username);
	VarInt length = VarInt(packid.size() + suuid.size() + susername.size());
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	data.append((char*)suuid.makeData().get(), suuid.size());
	data.append((char*)susername.makeData().get(), susername.size());

	// Send the packet
	send(client->socket, data.c_str(), data.size(), 0);
}

/*************************************
 * NetworkHandler :: sendChatMessage *
 * Send a message to the client      *
 *************************************/
void NetworkHandler::sendChatMessage(const Client* client, String message, ChatMessageType type, Boolean isJson)
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

	VarInt length = VarInt(packid.size() + smessage.size() + 1);
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	data.append((char*)smessage.makeData().get(), smessage.size());
	data.append(1, (char)type);

	// Send the packet
	send(client->socket, data.c_str(), data.size(), 0);
}

/************************************
 * NetworkHandler :: sendJoinGame   *
 * Tell the client to join the game *
 ************************************/
void NetworkHandler::sendJoinGame(const Client* client, Int entityID, Gamemode gamemode, Dimension dimension, Difficulty difficulty, Byte maxPlayers, LevelType levelType, Boolean reducedDebugInfo)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::JoinGame);
	SerialString slevelType = SerialString(levelType.str());
	VarInt length = VarInt(12 + slevelType.size() + packid.size());
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	writeInt(data, entityID);
	data.append(1, (char)gamemode);
	writeInt(data, (Int)dimension);
	data.append(1, (char)difficulty);
	data.append(1, maxPlayers);
	data.append((char*)slevelType.makeData().get(), slevelType.size());
	data.append(1, (char)reducedDebugInfo);

	// Send the packet
	send(client->socket, data.c_str(), data.size(), 0);
}

/***************************************
 * NetworkHandler :: sendPluginMessage *
 * Send a plugin message to the client *
 ***************************************/
void NetworkHandler::sendPluginMessage(const Client* client, String channel, Byte* data2, Int dataLen)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::PluginMessage);
	SerialString schannel = SerialString(channel);
	VarInt length = VarInt(packid.size() + schannel.size() + dataLen);
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	data.append((char*)schannel.makeData().get(), schannel.size());
	data.append((char*)data2, dataLen);

	// Send the packet
	send(client->socket, data.c_str(), data.size(), 0);
}

/******************************************
 * NetworkHandler :: sendChunkColumn      *
 * Sends a column of chunks to the client *
 ******************************************/
void NetworkHandler::sendChunk(const Client* client, Int x, Int z, ChunkColumn& column, Boolean createChunk, Boolean inOverworld)
{
	
}

/******************************************
 * NetworkHandler :: sendChunkColumn      *
 * Sends a column of chunks to the client *
 ******************************************/
void NetworkHandler::sendChunk(const Client* client, Int x, Int z, Boolean createChunk, Boolean inOverworld)
{
	
}

/*********************************************
 * NetworkHandler :: sendSpawnPosition       *
 * Notify the client where the spawnpoint is *
 *********************************************/
void NetworkHandler::sendSpawnPosition(const Client* client, Position pos)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::SpawnPosition);
	SerialPosition spos = SerialPosition(pos);
	VarInt length = VarInt(packid.size() + 8);
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	writeLong(data, spos.makeData());

	// Send the packet
	send(client->socket, data.c_str(), data.size(), 0);
}

/********************************************
 * NetworkHandler :: sendPlayerAbilities    *
 * Tells the client what it's allowed to do *
 ********************************************/
void NetworkHandler::sendPlayerAbilities(const Client* client, PlayerAbilities abilities, Float flyingSpeed, Float fovModifier)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::PlayerAbilities);
	VarInt length = VarInt(packid.size() + 9);
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	data.append(1, abilities.getFlags());
	writeFloat(data, flyingSpeed);
	writeFloat(data, fovModifier);

	// Send the packet
	send(client->socket, data.c_str(), data.size(), 0);
}

/********************************************
 * NetworkHandler :: sendPlayerAbilities    *
 * Tells the client what it's allowed to do *
 ********************************************/
void NetworkHandler::sendPlayerPositionAndLook(const Client* client, PositionF pos, Float yaw, Float pitch, PlayerPositionAndLookFlags flags, Int teleportID)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::PlayerPositionAndLook);
	VarInt steleportID = VarInt(teleportID);
	VarInt length = VarInt(packid.size() + steleportID.size() + 33);
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	writeDouble(data, pos.x);
	writeDouble(data, pos.y);
	writeDouble(data, pos.z);
	writeFloat(data, yaw);
	writeFloat(data, pitch);
	data.append(1, flags.getFlags());
	data.append((char*)steleportID.makeData(), steleportID.size());

	// Send the packet
	send(client->socket, data.c_str(), data.size(), 0);
}

/*********************************************
 * NetworkHandler :: sendUnloadChunk         *
 * Tell the client to unload the given chunk *
 *********************************************/
void NetworkHandler::sendUnloadChunk(const Client* client, Int x, Int z)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::UnloadChunk);
	VarInt length = VarInt(packid.size() + 8);
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	writeInt(data, x);
	writeInt(data, z);

	// Send the packet
	send(client->socket, data.c_str(), data.size(), 0);
}

/***********************************
 * NetworkHandler :: sendKeepAlive *
 * Prompt the client to respond    *
 ***********************************/
void NetworkHandler::sendKeepAlive(const Client* client, Long id)
{
	// Serialize the data
	String data;
	VarInt packid = VarInt((Int)ServerPlayPacket::KeepAlive);
	VarInt length = VarInt(packid.size() + 8);
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	writeLong(data, id);

	// Send the packet
	send(client->socket, data.c_str(), data.size(), 0);
}

/****************************
 * SERVER -> CLIENT PACKETS *
 ****************************/
 /* Status Packets */
void NetworkHandler::sendResponse(const Client* client, String json) {}
void NetworkHandler::sendPong(const Client* client, Long payload) {}

/* Login Packets */
void NetworkHandler::sendDisconnect(const Client* client, String reason) {}
void NetworkHandler::sendEncryptionRequest(const Client* client, String serverID, Byte* publicKey, Int publicKeyLen, Byte* verifyToken, Int verifyTokenLen) {}
void NetworkHandler::sendSetCompression(const Client* client, Int maxPacketSize) {}

/* Play Packets */
void NetworkHandler::sendSpawnObject(const Client* client, Object object) {}
void NetworkHandler::sendSpawnExperienceOrb(const Client* client, ExperienceOrb xpOrb) {}
void NetworkHandler::sendGlobalEntity(const Client* client, GlobalEntity entity) {}
void NetworkHandler::sendSpawnMob(const Client* client, Entity mob) {}
void NetworkHandler::sendSpawnPainting(const Client* client, Painting painting) {}
void NetworkHandler::sendSpawnPlayer(const Client* client, PlayerEntity player) {}
void NetworkHandler::sendAnimation(const Client* client, Int entityID, Animation animation) {}
void NetworkHandler::sendStatistics(const Client* client, Statistic stat) {}
void NetworkHandler::sendBlockBreakAnimation(const Client* client, Int entityID, Position pos, Byte destroyStage) {}
void NetworkHandler::sendUpdateBlockEntity(const Client* client, BlockEntity entity) {}
void NetworkHandler::sendBlockAction(const Client* client) {} // TODO: Add args
void NetworkHandler::sendBlockChange(const Client* client, Block block) {}
void NetworkHandler::sendBossBar(const Client* client, BossBar bar, BossBarAction action) {}
void NetworkHandler::sendServerDifficulty(const Client* client, Difficulty difficulty) {}
void NetworkHandler::sendTabComplete(const Client* client, String result) {}
void NetworkHandler::sendConfirmTransaction(const Client* client, Byte windowID, Short actionNum, Boolean accepted) {}
void NetworkHandler::sendCloseWindow(const Client* client, Byte windowID) {}
void NetworkHandler::sendOpenWindow(const Client* client, Window window) {}
void NetworkHandler::sendWindowItems(const Client* client) {} // TODO: Add args
void NetworkHandler::sendWindowProperty(const Client* client, Byte windowID, Short Property, Short Value) {}
void NetworkHandler::sendSetSlot(const Client* client) {} // TODO: Add args
void NetworkHandler::sendSetCooldown(const Client* client, Int itemID, Int numTicks) {}
void NetworkHandler::sendNamedSoundEffect(const Client* client, String name, SoundCategory category, Position pos, Float volume, Float pitch) {}
void NetworkHandler::sendEntityStatus(const Client* client, Int entityID, Byte entityStatus) {}
void NetworkHandler::sendChangeGameState(const Client* client, GameStateReason reason, Float value) {} // FLOAT?? TODO: See if value should be int
void NetworkHandler::sendChunkData(const Client* client) {} // TODO: Add args
void NetworkHandler::sendEffect(const Client* client, EffectID effectID, Position pos, Int data, Boolean disableRelativeVolume) {}
void NetworkHandler::sendParticle(const Client* client, Particle particle, Int num, Byte* data, Int dataLen) {}
void NetworkHandler::sendMap(const Client* client, Map map) {}
void NetworkHandler::sendEntityRelativeMove(const Client* client, Int entityID, PositionF deltaPos, Boolean isOnGround) {}
void NetworkHandler::sendEntityLookAndRelativeMove(const Client* client, Int entityID, PositionF deltaPos, Byte yaw, Byte pitch, Boolean isOnGround) {}
void NetworkHandler::sendEntityLook(const Client* client, Int entityID, Byte yaw, Byte pitch, Boolean isOnGround) {}
void NetworkHandler::sendEntity(const Client* client, Int entityID) {}
void NetworkHandler::sendVehicleMove(const Client* client, PositionF position, Float yaw, Float pitch) {}
void NetworkHandler::sendOpenSignEditor(const Client* client, Position pos) {}
void NetworkHandler::sendCraftRecipeResponse(const Client* client, Byte windowID, Int recipeID) {}
void NetworkHandler::sendEnterCombatEvent(const Client* client) {}
void NetworkHandler::sendEndCombatEvent(const Client* client, Int duration, Int entityID) {}
void NetworkHandler::sendEntityKilledEvent(const Client* client, Int killerID, Int killedID, String message) {}
void NetworkHandler::sendPlayerListItem(const Client* client) {} // TODO: Add args
void NetworkHandler::sendUseBed(const Client* client, Int entityID, Position pos) {}
void NetworkHandler::sendRemoveEntityEffect(const Client* client, Int entityID, StatusEffect effect) {}
void NetworkHandler::sendResourcePack(const Client* client, String url, String hash) {}
void NetworkHandler::sendRespawn(const Client* client, Dimension dimension, Difficulty difficulty, Gamemode gamemode, LevelType level) {}
void NetworkHandler::sendEntityHeadLook(const Client* client, Int entityID, Float yaw) {}
void NetworkHandler::sendSelectAdvancementTab(const Client* client, String id) {}
void NetworkHandler::sendWorldBorder(const Client* client) {} // TODO: Add args
void NetworkHandler::sendCamera(const Client* client, Int entityID) {}
void NetworkHandler::sendHeldItemChange(const Client* client, Byte hotbarSlot) {}
void NetworkHandler::sendDisplayScoreboard(const Client* client, ScoreboardPosition position, String name) {}
void NetworkHandler::sendEntityMetadata(const Client* client) {} // TODO: Add args
void NetworkHandler::sendAttachEntity(const Client* client, Int leashedEntityID, Int holdingEntityID) {}
void NetworkHandler::sendEntityVelocity(const Client* client, Int entityID, PositionF velocity) {}
void NetworkHandler::sendEntityEquipment(const Client* client, Int entityID, EntityEquipmentSlot slot, Slot item) {} // TODO: Change item var type
void NetworkHandler::sendSetExperience(const Client* client, Float xpBar/* Between 0 & 1 */, Int level) {} // Converts to (Float xpBar, Int level, Int totalXP)
void NetworkHandler::sendSetExperience(const Client* client, Int totalXP) {} // Converts to (Float xpBar, Int level, Int totalXP)
void NetworkHandler::sendUpdateHealth(const Client* client, Float hp, Int food, Float foodSaturation) {}
void NetworkHandler::sendScoreboardObjective(const Client* client, String name, ScoreboardMode mode, String displayName, ObjectiveType type) {}
void NetworkHandler::sendRemoveTeam(const Client* client, String name) {}
void NetworkHandler::sendUpdateScore(const Client* client, String entityName, String objectiveName, Int value) {}
void NetworkHandler::sendRemoveScore(const Client* client, String entityName, String objectiveName) {}
void NetworkHandler::sendTimeUpdate(const Client* client, Long worldAge, Long timeOfDay) {}
void NetworkHandler::sendSetTitle(const Client* client, String title) {}
void NetworkHandler::sendSetSubtitle(const Client* client, String subtitle) {}
void NetworkHandler::sendSetActionBar(const Client* client, String text) {}
void NetworkHandler::sendTitleFadeout(const Client* client, Int timeBeforeFade, Int fadeInLength, Int fadeOutLength) {}
void NetworkHandler::sendHideTitle(const Client* client) {}
void NetworkHandler::sendResetTitle(const Client* client) {}
void NetworkHandler::sendSoundEffect(const Client* client, Sound sound, SoundCategory category, Position position, Float volume, Float pitch) {}
void NetworkHandler::sendPlayerListHeaderAndFooter(const Client* client, String header, String footer) {}
void NetworkHandler::sendCollectItem(const Client* client, Int collectedID, Int collectorID, Int count) {}
void NetworkHandler::sendEntityTeleport(const Client* client, Int entityID, PositionF pos, Float yaw, Float pitch, Boolean onGround) {}
void NetworkHandler::sendAdvancements(const Client* client) {} // TODO: Add args
void NetworkHandler::sendEntityProperties(const Client* client) {} // TODO: Add args
void NetworkHandler::sendEntityEffect(const Client* client, Int entityID, StatusEffect effect, Int level, Int duration, StatusEffectFlags flags) {}
void NetworkHandler::sendStatistics(const Client* client, List<Int> statistics) {} // TODO: Create Statistic class
void NetworkHandler::sendTabComplete(const Client* client, List<String> results) {}
void NetworkHandler::sendBlockChanges(const Client* client, List<Block> blocks) {}
void NetworkHandler::sendExplosion(const Client* client, PositionF pos, Float radius, PositionF newClientVelocity, List<Position> blocksToRemove) {}
void NetworkHandler::sendUnlockRecipes(const Client* client, List<Int> recipes1, List<Int> recipes2, Boolean craftingBookOpen, Boolean filteringCraftable) {} // TODO: Implement Recipe class!
void NetworkHandler::sendDestroyEntities(const Client* client, List<Int> entities) {}
void NetworkHandler::sendSetPassengers(const Client* client, Int vehicleID, List<Int> passengers) {}
void NetworkHandler::sendCreateTeam(const Client* client, List<String> team) {}
void NetworkHandler::sendUpdateTeamInfo(const Client* client, Team team) {} // Ignores member list
void NetworkHandler::sendAddTeamMembers(const Client* client, Team team, List<String> members) {}
void NetworkHandler::sendRemoveTeamMembers(const Client* client, Team team, List<String> members) {}

/********************************************
 * NetworkHandler :: NetworkHandler         *
 * Default Constructor                      *
 ********************************************/
NetworkHandler::NetworkHandler()
	: running(false), eventHandler(nullptr) {}

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
void NetworkHandler::addClient(SOCKET& newClient)
{
	// Add the client to the list of clients
	std::unique_lock<std::shared_mutex> writeLock(eventHandler->clientsMutex);
	eventHandler->clients.push_back(std::move(LockedClient(std::shared_ptr<Client>(new Client(newClient)))));
}

/**************************************
 * NetworkHandler :: disconnectClient *
 * Disconnects and deletes a client   *
 **************************************/
void NetworkHandler::disconnectClient(LockedClient& client)
{
	// Disconnect the client's socket
	ClientDisconnectedEventArgs event(client.client, client.mutex.get());
	event.getWriteAccess(); // Also triggers getReadAccess()
	shutdown(client.client->socket, SD_BOTH);
	closesocket(client.client->socket);
	event.unblockReadAccess(); // Also triggers unblockWriteAccess()

	// Trigger the client disconnected event so that the event handler can clean up the client's data.
	eventHandler->clientDisconnected(std::move(event));
}

/****************************************************
 * NetworkHandler :: getClientFromSocket            *
 * Searches for a client using the given socket     *
 * It returns null if it could not find any clients *
 ****************************************************/
LockedClient* NetworkHandler::getClientFromSocket(SOCKET& socket)
{
	std::shared_lock<std::shared_mutex> readLock(eventHandler->clientsMutex);
	for (auto it = eventHandler->clients.begin(); it != eventHandler->clients.end(); ++it) {
		if (it->client->socket == socket)
			return &*it;
	}

	return nullptr;
}

/***********************************
 * NetworkHandler :: start         *
 * Starts up the client event loop *
 ***********************************/
#define BUFFER_SIZE 65536
void NetworkHandler::start()
{
	running = true;

	while (running)
	{
		// Create a list of every client to listen to
		// TODO: Make eventHandler->clients thread-safe
		fd_set clientList;
		if (eventHandler->clients.size() > 0)
		{
			int i = 0;
			for (auto it = eventHandler->clients.begin(); it != eventHandler->clients.end(); it++, i++)
				clientList.fd_array[i] = it->client->socket;
			clientList.fd_count = i;
			timeval timeout;
			timeout.tv_usec = 100000;
			int returnVal;

			// Listen to every client on the list
			if ((returnVal = select(0, &clientList, nullptr, nullptr, &timeout)) > 0)
			{
				// Receive some data from the clients
				for (int i = 0; i < returnVal; i++)
				{
					// Find the client in our list
					LockedClient* client = getClientFromSocket(clientList.fd_array[i]);

					// Read some data from the client
					char* buf = new char[BUFFER_SIZE];
					int dataRead = recv(clientList.fd_array[i], buf, BUFFER_SIZE, 0);
					if (dataRead == SOCKET_ERROR)
					{
						std::cout << "Error reading data from " << client->client->name
							<< ": " << WSAGetLastError() << "\n";
						disconnectClient(*client);
						delete[] buf; // Make sure no memory is leaked
					}
					else if (dataRead > 0)
					{
						// Attempt to read the varPack, it is up to readPacket to delete buf
						eventHandler->exec([client, buf, dataRead, this]() { this->readPacket(*client, std::unique_ptr<Byte[]>((Byte*)buf), dataRead); });
					}
					// The client is not sending bytes... it disconnected.
					else
					{
						disconnectClient(*client);
						delete[] buf; // Make sure no memory is leaked
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
