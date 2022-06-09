#include "eventhandler.h"
#include "networkhandler.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <boost/asio/thread_pool.hpp>

constexpr float DISCONNECT_TIME = 10.0;

/**********************************
 * toBytes                        *
 * Converts an integer to 4 bytes *
 **********************************/
void toBytes(const Int value, char* bytes)
{
	bytes[0] = (value >> 24) & 0xFF;
	bytes[1] = (value >> 16) & 0xFF;
	bytes[2] = (value >> 8) & 0xFF;
	bytes[3] = value & 0xFF;
}

/******************************
 * toBytes                    *
 * Converts a long to 8 bytes *
 ******************************/
void toBytes(const Long value, char* bytes)
{
	bytes[0] = (value >> 56) & 0xFF;
	bytes[1] = (value >> 48) & 0xFF;
	bytes[2] = (value >> 40) & 0xFF;
	bytes[3] = (value >> 32) & 0xFF;
	bytes[4] = (value >> 24) & 0xFF;
	bytes[5] = (value >> 16) & 0xFF;
	bytes[6] = (value >> 8) & 0xFF;
	bytes[7] = value & 0xFF;
}

/********************************
 * toBytes                      *
 * Converts a double to 8 bytes *
 ********************************/
void toBytes(const double value, char* bytes)
{
	Long val = reinterpret_cast<const Long&>(value);
	toBytes(val, bytes);
}

/*******************************
 * toBytes                     *
 * Converts a float to 4 bytes *
 *******************************/
void toBytes(const float value, char* bytes)
{
	Int val = reinterpret_cast<const Int&>(value);
	toBytes(val, bytes);
}

/**********************************
 * toBytes                        *
 * Converts a position to 8 bytes *
 **********************************/
void toBytes(const SerialPosition value, char* bytes)
{
	toBytes(value.makeData(), bytes);
}

/* Turns a position into a chunk position */
Position toChunkPosition(PositionF position)
{
	Position pos;
	pos.x = (Int)floor(position.x / 16.0);
	pos.y = (Int)floor(position.y / 16.0);
	pos.z = (Int)floor(position.z / 16.0);
	return pos;
}

/**********************************
 * EventHandler :: onTick         *
 * The code that's run every tick *
 **********************************/
void EventHandler::onTick(Double dt, Int ticksSkipped) {

}


/*************************************************
 * EventHandler :: invalidPacket                 *
 * The client sent an invalid packet             *
 *************************************************/
void EventHandler::invalidPacket(InvalidPacketEventArgs e)
{
	// TODO: Disconnect the user
}

/*************************************************
 * EventHandler :: invalidPacket                 *
 * The client sent an invalid packet             *
 *************************************************/
void EventHandler::invalidState(InvalidPacketEventArgs e)
{
	// TODO: Wonder why the client had an invalid state to begin with?...
}

/***************************************************
 * EventHandler :: invalidLength                   *
 * The client sent a packet with an invalid length *
 ***************************************************/
void EventHandler::invalidLength(InvalidLengthEventArgs e)
{
	// TODO: Chastise the client for being reckless
}

/*************************************
 * EventHandler :: clientDisconnect  *
 * The client has disconnected       *
 *************************************/
void EventHandler::clientDisconnected(const Client* client)
{
	// TODO: Alert all other players of the disconnect
	std::cout << client->name << " has disconnected.\n";
}

/*************************************************
 * EventHandler :: handshake                     *
 * The client is greeting the server             *
 *************************************************/
void EventHandler::handshake(HandShakeEventArgs e)
{
	// Store some info on the client
	auto client = e.getWriteAccess();
	client->state = e.state;
	client->protocolVersion = e.protocolVersion;
}

/*************************************************
 * EventHandler :: legacyServerListPing          *
 * A legacy client is greeting the server        *
 *************************************************/
void EventHandler::legacyServerListPing(LegacyServerListPingEventArgs e)
{
	auto client = e.getReadAccess();
	networkHandler->sendLegacyKick(client);

	// TODO: Get the shared_ptr directly from LegacyServerListPingEventArgs
	networkHandler->disconnectClient(std::shared_ptr<const Client>(client));
}

/*************************************************
 * EventHandler :: ping                          *
 * The client is pinging the server              *
 *************************************************/
void EventHandler::ping(PingEventArgs e)
{

}

/**********************************************
 * EventHandler :: request                    *
 * The client is requesting a response packet *
 **********************************************/
void EventHandler::request(RequestEventArgs e)
{
	const Client* client = e.getReadAccess();
	const char* data = "{\n"
		"    \"version\": {\n"
		"        \"name\": \"1.18.2\",\n"
		"        \"protocol\": 758\n"
		"    },\n"
		"    \"players\": {\n"
		"        \"max\": 10,\n"
		"        \"online\": 1,\n"
		"        \"sample\": [\n"
        "             {\n"
        "                 \"name\": \"thinkofdeath\",\n"
        "                 \"id\": \"4566e69f-c907-48ee-8d71-d7ba5aa00d20\"\n"
        "             }\n"
        "        ]\n"
		"    },\n"
		"    \"description\": {\n"
		"        \"text\": \"Hello C++ world!\"\n"
		"    }\n"
		"}\n";
	networkHandler->sendResponse(client, data);
}

/*************************************************
 * EventHandler :: encryptionResponse            *
 * The client has accepted the data encryption   *
 *************************************************/
void EventHandler::encryptionResponse(EncryptionResponseEventArgs e)
{

}

/*************************************************
 * EventHandler :: loginStart                    *
 * The client is attempting to log in            *
 *************************************************/
void EventHandler::loginStart(LoginStartEventArgs e)
{
	UUID uuid = UUID::generate::fromNameSHA1(e.name);
	auto client = e.getWriteAccess();
	client->name = e.name;
	client->uuid = uuid;
	client->yaw = 0.0;
	client->pitch = 0.0;
	client->position = PositionF(0.0, 0.0, 0.0);
	e.unblockWriteAccess(); // Does not unblock read access
	networkHandler->sendLoginSuccess(client, uuid, e.name);
	
	// Send every chunk to the player
	// TODO: Actually do this

	// As a temporary solution, send exactly one hardcoded chunk.


	// Tell the client that the map has finished loading
	networkHandler->sendPlayerPositionAndLook(client, PositionF(0.0, 0.0, 0.0), 0.0, 0.0, PlayerPositionAndLookFlags(false, false, false, false, false), 0, false);
}

/*************************************************
 * EventHandler :: animation                     *
 * The client wants to animate its hand          *
 *************************************************/
void EventHandler::handAnimation(HandAnimationEventArgs e)
{

}

/*************************************************
 * EventHandler :: chatMessage                   *
 * The client is sending a message or command    *
 *************************************************/
void EventHandler::chatMessage(ChatMessageEventArgs e)
{
	// Forward the message to every client that is in play mode
}

/*************************************************
 * EventHandler :: clickWindow                   *
 * The client is interacting with a window       *
 *************************************************/
void EventHandler::clickWindow(ClickWindowEventArgs e)
{

}

/**************************************************
 * EventHandler :: clientSettings                 *
 * The client wants to set or change its settings *
 **************************************************/
void EventHandler::clientSettings(ClientSettingsEventArgs e)
{

}

/*************************************************
 * EventHandler :: clientStatus                  *
 * The client wants to (re)spawn                 *
 *************************************************/
void EventHandler::clientStatus(ClientStatusEventArgs e)
{

}

/*************************************************
 * EventHandler :: closeWindow                   *
 * The client wants to close a window            *
 *************************************************/
void EventHandler::closeWindow(CloseWindowEventArgs e)
{

}

/********************************************************
 * EventHandler :: confirmTransaction                   *
 * The client is apologizing for a rejected transaction *
 ********************************************************/
void EventHandler::confirmTransaction(ConfirmTransactionEventArgs e)
{

}

/*************************************************
 * EventHandler :: creativeInventoryAction       *
 * The client is interacting with creative mode  *
 *************************************************/
void EventHandler::creativeInventoryAction(CreativeInventoryActionEventArgs e)
{

}

/*************************************************
 * EventHandler :: enchantItem                   *
 * The client wants to enchant an item           *
 *************************************************/
void EventHandler::enchantItem(EnchantItemEventArgs e)
{

}

/*************************************************
 * EventHandler :: entityAction                  *
 * The client wants to interact with an entity   *
 *************************************************/
void EventHandler::entityAction(EntityActionEventArgs e)
{

}

/**************************************************
 * EventHandler :: heldItemChange                 *
 * The client is switching its active hotbar slot *
 **************************************************/
void EventHandler::heldItemChange(HeldItemChangeEventArgs e)
{

}

/*************************************************
 * EventHandler :: keepAlive                     *
 * The client sends this to remain connected     *
 *************************************************/
void EventHandler::keepAlive(KeepAliveEventArgs e)
{
	
}

/*************************************************
 * EventHandler :: playerAbilities               *
 * The client wants to change its capabilities   *
 *************************************************/
void EventHandler::playerAbilities(PlayerAbilitiesEventArgs e)
{

}

/*************************************************
 * EventHandler :: playerBlockPlacement          *
 * The client wants to place a block             *
 *************************************************/
void EventHandler::playerBlockPlacement(PlayerBlockPlacementEventArgs e)
{

}

/*************************************************
 * EventHandler :: playerDigging                 *
 * The client wants to dig up a block            *
 *************************************************/
void EventHandler::playerDigging(PlayerDiggingEventArgs e)
{

}

/********************************************************
 * EventHandler :: playerMoved                          *
 * The client may be changing its position and/or angle *
 ********************************************************/
void EventHandler::playerMoved(PlayerMovedEventArgs e)
{
	// Store the data passed by the event
	auto client = e.getWriteAccess();
	client->onGround = e.onGround;
	if (e.playerChangedPosition) { client->position = e.position; }
	if (e.playerTurnedHead) {
		client->yaw = e.yaw;
		client->pitch = e.pitch;
	}
}

/*************************************************
 * EventHandler :: pluginMessage                 *
 * The client is sending a modded message        *
 *************************************************/
void EventHandler::pluginMessage(PluginMessageEventArgs e)
{

}

/********************************************************
 * EventHandler :: resourcePackStatus                   *
 * The client is accepting or rejecting a resource pack *
 ********************************************************/
void EventHandler::resourcePackStatus(ResourcePackStatusEventArgs e)
{

}

/*************************************************
 * EventHandler :: spectate                      *
 * The client wants to spectate an entity        *
 *************************************************/
void EventHandler::spectate(SpectateEventArgs e)
{

}

/*************************************************
 * EventHandler :: steerBoat                     *
 * The client is visually turning the paddles    *
 * Use steerVehicle event to capture steering    *
 *************************************************/
void EventHandler::steerBoat(SteerBoatEventArgs e)
{

}

/*************************************************
 * EventHandler :: steerVehicle                  *
 * The client is steering a vehicle                *
 *************************************************/
void EventHandler::steerVehicle(SteerVehicleEventArgs e)
{

}

/****************************************************************
 * EventHandler :: tabComplete                                  *
 * The client wants the server to guess the rest of the command *
 ****************************************************************/
void EventHandler::tabComplete(TabCompleteEventArgs e)
{

}

/****************************************************
 * EventHandler :: teleportConfirm                  *
 * The client is confirming a playerPositionAndLook *
 ****************************************************/
void EventHandler::teleportConfirm(TeleportConfirmEventArgs e)
{

}

/*************************************************
 * EventHandler :: updateSign                    *
 * The client wants to change a sign's message   *
 *************************************************/
void EventHandler::updateSign(UpdateSignEventArgs e)
{

}

/****************************************************
 * EventHandler :: useEntity                        *
 * The client wants to interact with another entity *
 ****************************************************/
void EventHandler::useEntity(UseEntityEventArgs e)
{

}

/*************************************************
 * EventHandler :: useItem                       *
 * The client wants to use an item               *
 *************************************************/
void EventHandler::useItem(UseItemEventArgs e)
{

}

/*************************************************
 * EventHandler :: vehicleMove                   *
 * The player is moving in a vehicle             *
 *************************************************/
void EventHandler::vehicleMoved(VehicleMovedEventArgs e)
{

}

/*****************************************
 * EventHandler :: getChunkSection       *
 * Returns a chunk at the given position *
 *****************************************/
ChunkSection& EventHandler::getChunkSection(GetChunkEventArgs& e)
{
//	if (e.pos.x > -2 && e.pos.x < 2 && e.pos.y == 12 && e.pos.z > -2 && e.pos.z < 2)
//	if (e.pos.x == 0 && e.pos.y == 12 && e.pos.z == 0)
/*	if (e.pos.y == 12)
	{
		e.chunk->fillBlocks(BlockID::Cobblestone, 0);
		e.chunk->fillLighting(1, 15);
	}

	return *e.chunk; */
	return ChunkSection(); // TODO: Replace with code that doesn't crash the server
}

/************************************************
 * EventHandler :: getChunk                     *
 * Returns a chunk column at the given position *
 ************************************************/
ChunkColumn& EventHandler::getChunk(GetChunkEventArgs& e)
{
	// Grab the chunks for all of the z-levels
/*	GetChunkSectionEventArgs e2(std::const_pointer_cast<Client>(e.client), nullptr);
	e2.client = e.client;
	e2.pos = Position(e.x, 0, e.z);
	for (int i = 0; i < 16; ++i)
	{
		e2.pos.y = i;
		e2.chunk = &e.chunk.chunks[i];
		getChunkSection(e2);
	}

	return e.chunk; */
	return ChunkColumn(); // TODO: Replace with code that doesn't crash the server
}

/*****************************************
 * EventHandler :: getBiomes             *
 * Returns the biomes at the given chunk *
 *****************************************/
BiomeID* EventHandler::getBiomes(GetBiomeEventArgs& e)
{
	// Grab the biome for all the given blocks in a chunk
/*	for (int i = 0; i < 256; ++i)
		e.biomes[i] = BiomeID::TheVoid;

	return e.biomes; */
	return nullptr; // TODO: Replace with code that doesn't crash the server
}

/********************************
 * EventHandler :: createClient *
 * Creates a client object      *
 ********************************/
Client* EventHandler::createClient(SOCKET socket)
{
	return new Client(socket);
}

/********************************
 * EventHandler :: EventHandler *
 * Default Constructor          *
 ********************************/
EventHandler::EventHandler() : networkHandler(nullptr), clients() {}

/********************************
 * EventHandler :: EventHandler *
 * Destructor                   *
 ********************************/
EventHandler::~EventHandler() { /* stopTickClock(); */ }

/******************************************
 * EventHandler :: exec                   *
 * Executes a function on a server thread *
 ******************************************/
void EventHandler::exec(std::function<void(void)> func) {
	func();
}