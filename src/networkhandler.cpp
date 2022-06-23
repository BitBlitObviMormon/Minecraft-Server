#include "bbms/networkhandler.h"
#include "bbms/eventhandler.h"
#include "bbms/networkpackets.h"
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

auto constexpr LEGACY_HANDSHAKE_LENGTH = 2;
/*************************************************************
 * NetworkHandler :: readPacket                              *
 * Reads the packet and fires off any events it gets from it *
 *************************************************************/
void NetworkHandler::readPacket(LockedClient& client, std::unique_ptr<Byte[]> buffer, Int length)
{
	// Check for a Pre 1.7 server list ping
	const Byte* buf = buffer.get();
	if (client.client->state == ServerState::Handshaking)
		if (*buf == 0xfe)
			handShakeLegacy(client, buf, LEGACY_HANDSHAKE_LENGTH); // Respond with old packet

	// Read the length of the packet
	VarInt varLen(buf);

	// Read the packet's id
	buf += varLen.size();
	VarInt varPack(buf);

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
		case (int)ClientLoginPacket::LoginPluginResponse:
			loginPluginResponse(client, buf, len);
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
		case (int)ClientPlayPacket::QueryBlockNBT:
			queryBlockNBT(client, buf, len);
			break;
		case (int)ClientPlayPacket::SetDifficulty:
			setDifficulty(client, buf, len);
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
		case (int)ClientPlayPacket::TabComplete:
			tabComplete(client, buf, len);
			break;
		case (int)ClientPlayPacket::ClickWindowButton:
			clickWindowButton(client, buf, len);
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
		case (int)ClientPlayPacket::EditBook:
			editBook(client, buf, len);
			break;
		case (int)ClientPlayPacket::QueryEntityNBT:
			queryEntityNBT(client, buf, len);
			break;
		case (int)ClientPlayPacket::InteractEntity:
			interactEntity(client, buf, len);
			break;
		case (int)ClientPlayPacket::GenerateStructure:
			generateStructure(client, buf, len);
			break;
		case (int)ClientPlayPacket::KeepAlive:
			keepAlive(client, buf, len);
			break;
		case (int)ClientPlayPacket::LockDifficulty:
			lockDifficulty(client, buf, len);
			break;
		case (int)ClientPlayPacket::PlayerPosition:
			playerPosition(client, buf, len);
			break;
		case (int)ClientPlayPacket::PlayerPositionAndRotation:
			playerPositionAndRotation(client, buf, len);
			break;
		case (int)ClientPlayPacket::PlayerRotation:
			playerRotation(client, buf, len);
			break;
		case (int)ClientPlayPacket::PlayerMovement:
			playerMovement(client, buf, len);
			break;
		case (int)ClientPlayPacket::VehicleMove:
			vehicleMove(client, buf, len);
			break;
		case (int)ClientPlayPacket::SteerBoat:
			steerBoat(client, buf, len);
			break;
		case (int)ClientPlayPacket::PickItem:
			pickItem(client, buf, len);
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
		case (int)ClientPlayPacket::Pong:
			pong(client, buf, len);
			break;
		case (int)ClientPlayPacket::SetRecipeBookState:
			setRecipeBookState(client, buf, len);
			break;
		case (int)ClientPlayPacket::SetDisplayedRecipe:
			setDisplayedRecipe(client, buf, len);
			break;
		case (int)ClientPlayPacket::NameItem:
			nameItem(client, buf, len);
			break;
		case (int)ClientPlayPacket::ResourcePackStatus:
			resourcePackStatus(client, buf, len);
			break;
		case (int)ClientPlayPacket::AdvancementTab:
			advancementTab(client, buf, len);
			break;
		case (int)ClientPlayPacket::SelectTrade:
			selectTrade(client, buf, len);
			break;
		case (int)ClientPlayPacket::SetBeaconEffect:
			setBeaconEffect(client, buf, len);
			break;
		case (int)ClientPlayPacket::HeldItemChange:
			heldItemChange(client, buf, len);
			break;
		case (int)ClientPlayPacket::UpdateCommandBlock:
			updateCommandBlock(client, buf, len);
			break;
		case (int)ClientPlayPacket::UpdateCommandBlockMinecart:
			updateCommandBlockMinecart(client, buf, len);
			break;
		case (int)ClientPlayPacket::CreativeInventoryAction:
			creativeInventoryAction(client, buf, len);
			break;
		case (int)ClientPlayPacket::UpdateJigsawBlock:
			updateJigsawBlock(client, buf, len);
			break;
		case (int)ClientPlayPacket::UpdateStructureBlock:
			updateStructureBlock(client, buf, len);
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
	VarInt protocol(buffer);
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
	eventHandler->legacyServerListPing(LegacyServerListPingEventArgs(client.client, client.mutex.get(), buffer[1]));
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
	SerialString name(buffer);

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
	VarInt sharedSecretLen(buffer);
	Int sharedSecretLength = sharedSecretLen.toInt();
	buffer += sharedSecretLen.size();

	// Get the shared secret's data
	std::unique_ptr<Byte[]> secret = copyBuffer(buffer, sharedSecretLength);
	buffer += sharedSecretLength;

	// Get the verify token's length
	VarInt verifyTokenLen(buffer);
	Int verifyTokenLength = verifyTokenLen.toInt();
	buffer += verifyTokenLen.size();

	// Get the verify token's data
	std::unique_ptr<Byte[]> token = copyBuffer(buffer, verifyTokenLength);
	
	// Notify the server of the client's response
	EventHandler* handler = eventHandler;
	eventHandler->encryptionResponse(EncryptionResponseEventArgs(client.client, client.mutex.get(), secret.get(), sharedSecretLength, token.get(), verifyTokenLength));
}

/*****************************************
 * NetworkHandler :: loginPluginResponse *
 *****************************************/
void NetworkHandler::loginPluginResponse(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
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

/*****************************************
 * NetworkHandler :: queryBlockNBT       *
 *****************************************/
void NetworkHandler::queryBlockNBT(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
}

/*****************************************
 * NetworkHandler :: setDifficulty       *
 *****************************************/
void NetworkHandler::setDifficulty(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
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
	VarInt chatMode(buffer);
	buffer += chatMode.size();

	// Get the client's chat colors setting
	Boolean chatColors = !!*(buffer++);

	// Get the client's displayed skin parts
	DisplayedSkinParts displayedSkinParts(*(buffer++));

	// Get the client's main hand
	MainHand mainHand = (MainHand)VarInt(buffer).toInt();

	// Notify the server of the client's requested settings
	eventHandler->clientSettings(ClientSettingsEventArgs(client.client, client.mutex.get(), chatColors, mainHand, viewDistance, (ChatMode)chatMode.toInt(), displayedSkinParts, locale.str()));
}

/*****************************************
 * NetworkHandler :: clickWindowButton   *
 *****************************************/
void NetworkHandler::clickWindowButton(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
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

/*****************************************
 * NetworkHandler :: editBook            *
 *****************************************/
void NetworkHandler::editBook(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
}

/*****************************************
 * NetworkHandler :: queryEntityNBT      *
 *****************************************/
void NetworkHandler::queryEntityNBT(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
}

/*******************************************
 * NetworkHandler :: useEntity             *
 * The client right-clicked another entity *
 *******************************************/
void NetworkHandler::interactEntity(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement interactEntity
}

/*****************************************
 * NetworkHandler :: generateStructure   *
 *****************************************/
void NetworkHandler::generateStructure(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
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

/*****************************************
 * NetworkHandler :: lockDifficulty      *
 *****************************************/
void NetworkHandler::lockDifficulty(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
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
void NetworkHandler::playerPositionAndRotation(LockedClient& client, const Byte* buffer, Int length)
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
void NetworkHandler::playerRotation(LockedClient& client, const Byte* buffer, Int length)
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
void NetworkHandler::playerMovement(LockedClient& client, const Byte* buffer, Int length)
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

/*****************************************
 * NetworkHandler :: pickItem            *
 *****************************************/
void NetworkHandler::pickItem(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
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

/*****************************************
 * NetworkHandler :: pong                *
 *****************************************/
void NetworkHandler::pong(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
}

/*****************************************
 * NetworkHandler :: setRecipeBookState  *
 *****************************************/
void NetworkHandler::setRecipeBookState(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
}

/*****************************************
 * NetworkHandler :: setDisplayedRecipe  *
 *****************************************/
void NetworkHandler::setDisplayedRecipe(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
}

/*****************************************
 * NetworkHandler :: nameItem            *
 *****************************************/
void NetworkHandler::nameItem(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
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

/*****************************************
 * NetworkHandler :: selectTrade         *
 *****************************************/
void NetworkHandler::selectTrade(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
}

/*****************************************
 * NetworkHandler :: setBeaconEffect     *
 *****************************************/
void NetworkHandler::setBeaconEffect(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
}

/***********************************************
 * NetworkHandler :: heldItemChange            *
 * The client wants to select a different item *
 ***********************************************/
void NetworkHandler::heldItemChange(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement heldItemChange
}

/*****************************************
 * NetworkHandler :: updateCommandBlock  *
 *****************************************/
void NetworkHandler::updateCommandBlock(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
}

/************************************************
 * NetworkHandler :: updateCommandBlockMinecart *
 ************************************************/
void NetworkHandler::updateCommandBlockMinecart(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
}

/*****************************************************************
 * NetworkHandler :: creativeInventoryAction                     *
 * The client wants to interact with the creative mode inventory *
 *****************************************************************/
void NetworkHandler::creativeInventoryAction(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement creativeInventoryAction
}

/*****************************************
 * NetworkHandler :: updateJigsawBlock   *
 *****************************************/
void NetworkHandler::updateJigsawBlock(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
}

/******************************************
 * NetworkHandler :: updateStructureBlock *
 ******************************************/
void NetworkHandler::updateStructureBlock(LockedClient& client, const Byte* buffer, Int length)
{
	// TODO: Implement
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

/***********************************
 * SERVER => CLIENT STATUS PACKETS *
 ***********************************/

/****************************************************
 * NetworkHandler :: sendResponse                   *
 * Sends packaged JSON data representing the server *
 ****************************************************/
Int NetworkHandler::sendResponse(const Client* client, String json) {
	// Serialize the data
	String data;
	SerialString sJSON(json);
	VarInt packid((Int)ServerStatusPacket::Pong);
	VarInt length(packid.size() + sJSON.size());

	// Append the data to a string
	data.reserve(length.toInt() + length.size());
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	data.append((char*)sJSON.makeData().get(), sJSON.length());

	std::cout << sJSON.makeData() << "\n";

	// Send the packet
	return send(client->socket, data.c_str(), data.size(), 0);
}


/******************************
 * NetworkHandler :: sendPong *
 * Echoes what a client sent  *
 ******************************/
Int NetworkHandler::sendPong(const Client* client, Long payload) {
	// Serialize the data
	String data;
	VarInt packid((Int)ServerStatusPacket::Pong);
	VarInt length(packid.size() + sizeof(payload));

	// Append the data to a string
	data.reserve(length.toInt() + length.size());
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	writeLong(data, payload);

	// Send the packet
	return send(client->socket, data.c_str(), data.size(), 0);
}

/***************************************************
 * NetworkHandler :: sendLoginSuccess              *
 * Tell the client that their login was successful *
 ***************************************************/
Int NetworkHandler::sendLoginSuccess(const Client* client, UUID uuid, String username)
{
	// Serialize the data
	String data;
	VarInt packid((Int)ServerLoginPacket::LoginSuccess);
	SerialString susername = SerialString(username);
	VarInt length(packid.size() + uuid.size() + susername.size());

	// Append the data to the string
	data.reserve(length.toInt() + length.size());
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	data.append((char*)uuid.makeData(), uuid.size());
	data.append((char*)susername.makeData().get(), susername.size());

	// Send the packet
	return send(client->socket, data.c_str(), data.size(), 0);
}

/*************************************
 * NetworkHandler :: sendChatMessage *
 * Send a message to the client      *
 *************************************/
Int NetworkHandler::sendChatMessage(const Client* client, String message, ChatMessageType type, Boolean isJson)
{
	// Serialize the data
	String data;
	VarInt packid((Int)ServerPlayPacket::ChatMessage);
	SerialString smessage;

	// If the text is in JSON format then send it directly
	if (isJson)
		smessage = SerialString(message);
	// If the text is not in JSON then make it so.
	else
		smessage = SerialString(String("{ \"text\": \"") + message + String("\" }"));

	VarInt length(packid.size() + smessage.size() + 1);
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	data.append((char*)smessage.makeData().get(), smessage.size());
	data.append(1, (char)type);

	// Send the packet
	return send(client->socket, data.c_str(), data.size(), 0);
}

/************************************
 * NetworkHandler :: sendJoinGame   *
 * Tell the client to join the game *
 ************************************/
Int NetworkHandler::sendJoinGame(const Client* client, Int entityID, Gamemode gamemode, Dimension dimension, Difficulty difficulty, Byte maxPlayers, LevelType levelType, Boolean reducedDebugInfo)
{
	// Serialize the data
	String data;
	VarInt packid((Int)ServerPlayPacket::JoinGame);
	SerialString slevelType(levelType.str());
	VarInt length(12 + slevelType.size() + packid.size());
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
	return send(client->socket, data.c_str(), data.size(), 0);
}

/***************************************
 * NetworkHandler :: sendPluginMessage *
 * Send a plugin message to the client *
 ***************************************/
Int NetworkHandler::sendPluginMessage(const Client* client, String channel, Byte* data2, Int dataLen)
{
	// Serialize the data
	String data;
	VarInt packid((Int)ServerPlayPacket::PluginMessage);
	SerialString schannel(channel);
	VarInt length(packid.size() + schannel.size() + dataLen);
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	data.append((char*)schannel.makeData().get(), schannel.size());
	data.append((char*)data2, dataLen);

	// Send the packet
	return send(client->socket, data.c_str(), data.size(), 0);
}

/******************************************
 * NetworkHandler :: sendChunkColumn      *
 * Sends a column of chunks to the client *
 ******************************************/
Int NetworkHandler::sendChunk(const Client* client, Int x, Int z, ChunkColumn& column, Boolean createChunk, Boolean inOverworld)
{
	return 0;
}

/******************************************
 * NetworkHandler :: sendChunkColumn      *
 * Sends a column of chunks to the client *
 ******************************************/
Int NetworkHandler::sendChunk(const Client* client, Int x, Int z, Boolean createChunk, Boolean inOverworld)
{
	return 0;
}

/*********************************************
 * NetworkHandler :: sendSpawnPosition       *
 * Notify the client where the spawnpoint is *
 *********************************************/
Int NetworkHandler::sendSpawnPosition(const Client* client, Position pos)
{
	// Serialize the data
	String data;
	VarInt packid((Int)ServerPlayPacket::SpawnPosition);
	SerialPosition spos = SerialPosition(pos);
	VarInt length(packid.size() + 8);
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	writeLong(data, spos.makeData());

	// Send the packet
	return send(client->socket, data.c_str(), data.size(), 0);
}

/********************************************
 * NetworkHandler :: sendPlayerAbilities    *
 * Tells the client what it's allowed to do *
 ********************************************/
Int NetworkHandler::sendPlayerAbilities(const Client* client, PlayerAbilities abilities, Float flyingSpeed, Float fovModifier)
{
	// Serialize the data
	String data;
	VarInt packid((Int)ServerPlayPacket::PlayerAbilities);
	VarInt length(packid.size() + 9);
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	data.append(1, abilities.getFlags());
	writeFloat(data, flyingSpeed);
	writeFloat(data, fovModifier);

	// Send the packet
	return send(client->socket, data.c_str(), data.size(), 0);
}

/********************************************
 * NetworkHandler :: sendPlayerAbilities    *
 * Tells the client what it's allowed to do *
 ********************************************/
Int NetworkHandler::sendPlayerPositionAndLook(const Client* client, PositionF pos, Float yaw, Float pitch, PlayerPositionAndLookFlags flags, Int teleportID, Boolean dismountVehicle)
{
	// Serialize the data
	String data;
	VarInt packid((Int)ServerPlayPacket::PlayerPositionAndLook);
	VarInt steleportID(teleportID);
	VarInt length(packid.size() + steleportID.size()
	      + sizeof(Double) * 3 + sizeof(Float) * 2 + sizeof(flags) + sizeof(dismountVehicle));
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
	data.append(1, (char)dismountVehicle);

	// Send the packet
	return send(client->socket, data.c_str(), data.size(), 0);
}

/*********************************************
 * NetworkHandler :: sendUnloadChunk         *
 * Tell the client to unload the given chunk *
 *********************************************/
Int NetworkHandler::sendUnloadChunk(const Client* client, Int x, Int z)
{
	// Serialize the data
	String data;
	VarInt packid((Int)ServerPlayPacket::UnloadChunk);
	VarInt length(packid.size() + 8);
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	writeInt(data, x);
	writeInt(data, z);

	// Send the packet
	return send(client->socket, data.c_str(), data.size(), 0);
}

/***********************************
 * NetworkHandler :: sendKeepAlive *
 * Prompt the client to respond    *
 ***********************************/
Int NetworkHandler::sendKeepAlive(const Client* client, Long id)
{
	// Serialize the data
	String data;
	VarInt packid((Int)ServerPlayPacket::KeepAlive);
	VarInt length(packid.size() + 8);
	data.reserve(length.toInt() + length.size());

	// Append the data to the string
	data.append((char*)length.makeData(), length.size());
	data.append((char*)packid.makeData(), packid.size());
	writeLong(data, id);

	// Send the packet
	return send(client->socket, data.c_str(), data.size(), 0);
}

/****************************************
 * NetworkHandler :: sendLegacyKick     *
 * Kicks clients that use 1.6 and below *
 ****************************************/
Int NetworkHandler::sendLegacyKick(const Client* client) {
	// Send a predefined kick packet for legacy clients
	const String data = "\xff\x00\x23\x00\xa7\x00\x31\x00\x00\x00\x7f\x00\x37\x00\x00\x00\x31\x00\x2e\x00\x34\x00\x2e\x00\x32\x00\x00\x00\x41\x00\x20\x00\x4d\x00\x69\x00\x6e\x00\x65\x00\x63\x00\x72\x00\x61\x00\x66\x00\x74\x00\x20\x00\x53\x00\x65\x00\x72\x00\x76\x00\x65\x00\x72\x00\x00\x00\x30\x00\x00\x00\x32\x00\x30";
	return send(client->socket, data.c_str(), data.size(), 0);
}

/****************************
 * SERVER -> CLIENT PACKETS *
 ****************************/
/* Login Packets */
Int NetworkHandler::sendDisconnect(const Client* client, String reason) { return 0; }
Int NetworkHandler::sendEncryptionRequest(const Client* client, String serverID, Byte* publicKey, Int publicKeyLen, Byte* verifyToken, Int verifyTokenLen) { return 0; }
Int NetworkHandler::sendSetCompression(const Client* client, Int maxPacketSize) { return 0; }

/* Play Packets */
Int NetworkHandler::sendSpawnObject(const Client* client, Object object) { return 0; }
Int NetworkHandler::sendSpawnExperienceOrb(const Client* client, ExperienceOrb xpOrb) { return 0; }
Int NetworkHandler::sendGlobalEntity(const Client* client, GlobalEntity entity) { return 0; }
Int NetworkHandler::sendSpawnMob(const Client* client, Entity mob) { return 0; }
Int NetworkHandler::sendSpawnPainting(const Client* client, Painting painting) { return 0; }
Int NetworkHandler::sendSpawnPlayer(const Client* client, PlayerEntity player) { return 0; }
Int NetworkHandler::sendAnimation(const Client* client, Int entityID, Animation animation) { return 0; }
Int NetworkHandler::sendStatistics(const Client* client, Statistic stat) { return 0; }
Int NetworkHandler::sendBlockBreakAnimation(const Client* client, Int entityID, Position pos, Byte destroyStage) { return 0; }
Int NetworkHandler::sendUpdateBlockEntity(const Client* client, BlockEntity entity) { return 0; }
Int NetworkHandler::sendBlockAction(const Client* client) { return 0; } // TODO: Add args
Int NetworkHandler::sendBlockChange(const Client* client, Block block) { return 0; }
Int NetworkHandler::sendBossBar(const Client* client, BossBar bar, BossBarAction action) { return 0; }
Int NetworkHandler::sendServerDifficulty(const Client* client, Difficulty difficulty) { return 0; }
Int NetworkHandler::sendTabComplete(const Client* client, String result) { return 0; }
Int NetworkHandler::sendConfirmTransaction(const Client* client, Byte windowID, Short actionNum, Boolean accepted) { return 0; }
Int NetworkHandler::sendCloseWindow(const Client* client, Byte windowID) { return 0; }
Int NetworkHandler::sendOpenWindow(const Client* client, Window window) { return 0; }
Int NetworkHandler::sendWindowItems(const Client* client) { return 0; } // TODO: Add args
Int NetworkHandler::sendWindowProperty(const Client* client, Byte windowID, Short Property, Short Value) { return 0; }
Int NetworkHandler::sendSetSlot(const Client* client) { return 0; } // TODO: Add args
Int NetworkHandler::sendSetCooldown(const Client* client, Int itemID, Int numTicks) { return 0; }
Int NetworkHandler::sendNamedSoundEffect(const Client* client, String name, SoundCategory category, Position pos, Float volume, Float pitch) { return 0; }
Int NetworkHandler::sendEntityStatus(const Client* client, Int entityID, Byte entityStatus) { return 0; }
Int NetworkHandler::sendChangeGameState(const Client* client, GameStateReason reason, Float value) { return 0; } // FLOAT?? TODO: See if value should be int
Int NetworkHandler::sendChunkData(const Client* client) { return 0; } // TODO: Add args
Int NetworkHandler::sendEffect(const Client* client, EffectID effectID, Position pos, Int data, Boolean disableRelativeVolume) { return 0; }
Int NetworkHandler::sendParticle(const Client* client, Particle particle, Int num, Byte* data, Int dataLen) { return 0; }
Int NetworkHandler::sendMap(const Client* client, Map map) { return 0; }
Int NetworkHandler::sendEntityRelativeMove(const Client* client, Int entityID, PositionF deltaPos, Boolean isOnGround) { return 0; }
Int NetworkHandler::sendEntityLookAndRelativeMove(const Client* client, Int entityID, PositionF deltaPos, Byte yaw, Byte pitch, Boolean isOnGround) { return 0; }
Int NetworkHandler::sendEntityLook(const Client* client, Int entityID, Byte yaw, Byte pitch, Boolean isOnGround) { return 0; }
Int NetworkHandler::sendEntity(const Client* client, Int entityID) { return 0; }
Int NetworkHandler::sendVehicleMove(const Client* client, PositionF position, Float yaw, Float pitch) { return 0; }
Int NetworkHandler::sendOpenSignEditor(const Client* client, Position pos) { return 0; }
Int NetworkHandler::sendCraftRecipeResponse(const Client* client, Byte windowID, Int recipeID) { return 0; }
Int NetworkHandler::sendEnterCombatEvent(const Client* client) { return 0; }
Int NetworkHandler::sendEndCombatEvent(const Client* client, Int duration, Int entityID) { return 0; }
Int NetworkHandler::sendEntityKilledEvent(const Client* client, Int killerID, Int killedID, String message) { return 0; }
Int NetworkHandler::sendPlayerListItem(const Client* client) { return 0; } // TODO: Add args
Int NetworkHandler::sendUseBed(const Client* client, Int entityID, Position pos) { return 0; }
Int NetworkHandler::sendRemoveEntityEffect(const Client* client, Int entityID, StatusEffect effect) { return 0; }
Int NetworkHandler::sendResourcePack(const Client* client, String url, String hash) { return 0; }
Int NetworkHandler::sendRespawn(const Client* client, Dimension dimension, Difficulty difficulty, Gamemode gamemode, LevelType level) { return 0; }
Int NetworkHandler::sendEntityHeadLook(const Client* client, Int entityID, Float yaw) { return 0; }
Int NetworkHandler::sendSelectAdvancementTab(const Client* client, String id) { return 0; }
Int NetworkHandler::sendWorldBorder(const Client* client) { return 0; } // TODO: Add args
Int NetworkHandler::sendCamera(const Client* client, Int entityID) { return 0; }
Int NetworkHandler::sendHeldItemChange(const Client* client, Byte hotbarSlot) { return 0; }
Int NetworkHandler::sendDisplayScoreboard(const Client* client, ScoreboardPosition position, String name) { return 0; }
Int NetworkHandler::sendEntityMetadata(const Client* client) { return 0; } // TODO: Add args
Int NetworkHandler::sendAttachEntity(const Client* client, Int leashedEntityID, Int holdingEntityID) { return 0; }
Int NetworkHandler::sendEntityVelocity(const Client* client, Int entityID, PositionF velocity) { return 0; }
Int NetworkHandler::sendEntityEquipment(const Client* client, Int entityID, EntityEquipmentSlot slot, Slot item) { return 0; } // TODO: Change item var type
Int NetworkHandler::sendSetExperience(const Client* client, Float xpBar/* Between 0 & 1 */, Int level) { return 0; } // Converts to (Float xpBar, Int level, Int totalXP)
Int NetworkHandler::sendSetExperience(const Client* client, Int totalXP) { return 0; } // Converts to (Float xpBar, Int level, Int totalXP)
Int NetworkHandler::sendUpdateHealth(const Client* client, Float hp, Int food, Float foodSaturation) { return 0; }
Int NetworkHandler::sendScoreboardObjective(const Client* client, String name, ScoreboardMode mode, String displayName, ObjectiveType type) { return 0; }
Int NetworkHandler::sendRemoveTeam(const Client* client, String name) { return 0; }
Int NetworkHandler::sendUpdateScore(const Client* client, String entityName, String objectiveName, Int value) { return 0; }
Int NetworkHandler::sendRemoveScore(const Client* client, String entityName, String objectiveName) { return 0; }
Int NetworkHandler::sendTimeUpdate(const Client* client, Long worldAge, Long timeOfDay) { return 0; }
Int NetworkHandler::sendSetTitle(const Client* client, String title) { return 0; }
Int NetworkHandler::sendSetSubtitle(const Client* client, String subtitle) { return 0; }
Int NetworkHandler::sendSetActionBar(const Client* client, String text) { return 0; }
Int NetworkHandler::sendTitleFadeout(const Client* client, Int timeBeforeFade, Int fadeInLength, Int fadeOutLength) { return 0; }
Int NetworkHandler::sendHideTitle(const Client* client) { return 0; }
Int NetworkHandler::sendResetTitle(const Client* client) { return 0; }
Int NetworkHandler::sendSoundEffect(const Client* client, Sound sound, SoundCategory category, Position position, Float volume, Float pitch) { return 0; }
Int NetworkHandler::sendPlayerListHeaderAndFooter(const Client* client, String header, String footer) { return 0; }
Int NetworkHandler::sendCollectItem(const Client* client, Int collectedID, Int collectorID, Int count) { return 0; }
Int NetworkHandler::sendEntityTeleport(const Client* client, Int entityID, PositionF pos, Float yaw, Float pitch, Boolean onGround) { return 0; }
Int NetworkHandler::sendAdvancements(const Client* client) { return 0; } // TODO: Add args
Int NetworkHandler::sendEntityProperties(const Client* client) { return 0; } // TODO: Add args
Int NetworkHandler::sendEntityEffect(const Client* client, Int entityID, StatusEffect effect, Int level, Int duration, StatusEffectFlags flags) { return 0; }
Int NetworkHandler::sendStatistics(const Client* client, List<Int> statistics) { return 0; } // TODO: Create Statistic class
Int NetworkHandler::sendTabComplete(const Client* client, List<String> results) { return 0; }
Int NetworkHandler::sendBlockChanges(const Client* client, List<Block> blocks) { return 0; }
Int NetworkHandler::sendExplosion(const Client* client, PositionF pos, Float radius, PositionF newClientVelocity, List<Position> blocksToRemove) { return 0; }
Int NetworkHandler::sendUnlockRecipes(const Client* client, List<Int> recipes1, List<Int> recipes2, Boolean craftingBookOpen, Boolean filteringCraftable) { return 0; } // TODO: Implement Recipe class!
Int NetworkHandler::sendDestroyEntities(const Client* client, List<Int> entities) { return 0; }
Int NetworkHandler::sendSetPassengers(const Client* client, Int vehicleID, List<Int> passengers) { return 0; }
Int NetworkHandler::sendCreateTeam(const Client* client, List<String> team) { return 0; }
Int NetworkHandler::sendUpdateTeamInfo(const Client* client, Team team) { return 0; } // Ignores member list
Int NetworkHandler::sendAddTeamMembers(const Client* client, Team team, List<String> members) { return 0; }
Int NetworkHandler::sendRemoveTeamMembers(const Client* client, Team team, List<String> members) { return 0; }

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
	eventHandler->clients.push_back(std::move(LockedClient(std::shared_ptr<Client>(eventHandler->createClient(newClient)))));
}

/**************************************
 * NetworkHandler :: disconnectClient *
 * Disconnects and deletes a client   *
 **************************************/
void NetworkHandler::disconnectClient(std::shared_ptr<const Client> client)
{
	// Fetch the client from the eventHandler
	std::unique_lock<std::shared_mutex> writeLock(eventHandler->clientsMutex);
	for (auto it = eventHandler->clients.begin(); it != eventHandler->clients.end(); ++it) {
		if (it->client->socket == client->socket) {
			// Disconnect the client's socket
			shutdown(it->client->socket, SD_BOTH);
			closesocket(it->client->socket);
			eventHandler->clients.erase(it);
			break; // No more than one client should match this one
		}
	}
	writeLock.unlock();

	// Trigger the client disconnected event so that the event handler can clean up the client's data.
	eventHandler->clientDisconnected(client.get());
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

/**************************************
 * NetworkHandler :: send             *
 * Sends data without breaking it up  *
 * Returns an error code if it failed *
 * or zero if nothing went wrong      *
 **************************************/
Int NetworkHandler::send(SOCKET s, const char* buf, Int len, Int flags) {
	Int dataRead = ::send(s, buf, len, flags);

#ifdef _WIN32
	if (dataRead == SOCKET_ERROR) return WSAGetLastError();
#else
	if (dataRead == SOCKET_ERROR) return errno;
#endif

	// Send the rest of the data
	if (dataRead < len)
		return send(s, buf + dataRead, len - dataRead, flags);
	else
		return 0;
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
		std::shared_lock<std::shared_mutex> readLock(eventHandler->clientsMutex);
		if (eventHandler->clients.size() > 0)
		{
			int i = 0;
			for (auto it = eventHandler->clients.begin(); it != eventHandler->clients.end(); it++, i++)
				clientList.fd_array[i] = it->client->socket;
			readLock.unlock();
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
						disconnectClient(client->client);
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
						disconnectClient(client->client);
						delete[] buf; // Make sure no memory is leaked
					}
				}
			}
			else if (returnVal == SOCKET_ERROR)
			{
				std::cout << "Error receiving client message: " << WSAGetLastError() << "\n";
			}
		}
		else {
			readLock.unlock();
			Sleep(100); // TODO: Replace with conditional wakeup from Server->listenForClients
		}
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
