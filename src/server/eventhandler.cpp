#include "debug.h"
#include "server/eventhandler.h"
#include "server/networkhandler.h"
#include <iostream>
#include <ctime>
#include <cmath>

#define DISCONNECT_TIME 10.0

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
	toBytes(value.getData(), bytes);
}

/*****************************************
 * EventHandler :: runTickClock          *
 * Runs the onTick event regularly after *
 * the startTickClock method is run      *
 *****************************************/
void EventHandler::runTickClock()
{
	// Set up the local variables
	clock_t lastClock = clock();
	clock_t newClock = clock();
	Double dt = (newClock - lastClock) / static_cast<Double>(CLOCKS_PER_SEC);
	Int ticksLost = static_cast<Int>(dt / tickDelay);

	// While the tick clock is running, start up the tick
	while (running)
	{
		// Start the tick
		onTick(dt, ticksLost);

		// Calculate the time, difference, and how many ticks were lost
		newClock = clock();
		dt = (newClock - lastClock) / static_cast<Double>(CLOCKS_PER_SEC);
		ticksLost = static_cast<Int>(dt / tickDelay);

		// Sleep for the remainder of the tick
		Int timeRemaining = static_cast<Int>(1000000.0 * (tickDelay - (dt - (ticksLost * tickDelay))));
		std::this_thread::sleep_for(std::chrono::microseconds(timeRemaining));

		// Calculate the time, difference, and how many ticks were lost again
		lastClock = newClock;	// Set the new clock to become the old
		newClock = clock();
		dt = (newClock - lastClock) / static_cast<Double>(CLOCKS_PER_SEC);
		ticksLost = static_cast<Int>(dt / tickDelay);
	}
}

/*************************************************************
 * EventHandler :: seedNetwork                               *
 * Gives the event handler a network to send packets through *
 *************************************************************/
void EventHandler::seedNetwork(NetworkHandler* netHandler) { networkHandler = netHandler; }

/* Turns a position into a chunk position */
Position toChunkPosition(PositionF position)
{
	Position pos;
	pos.x = (Int)floor(position.x / 16.0);
	pos.y = (Int)floor(position.y / 16.0);
	pos.z = (Int)floor(position.z / 16.0);
	return pos;
}

/*****************************/
/* EventHandler :: onTick    */
/* A tick in the game's loop */
/*****************************/
void EventHandler::onTick(Double dt, Int ticksSkipped)
{
	// Finish off the job queue before-hand
	jobQueue.start(false);
	
	// Run this on every client
	for each (Client* client in clients)
	{
		// For clients that are currently in play:
		if (client->getState() == ServerState::Play)
		{
			// Update the client's ticks
			Long uptime = client->incrementUptime(1 + ticksSkipped);
			Int ticksSinceUpdate = client->incrementTicksSinceUpdate(1 + ticksSkipped);

			// TODO: Update the things that need to be updated every tick

			// If the client has been out for too long, disconnect it. :(
			if (ticksSinceUpdate > static_cast<Double>(DISCONNECT_TIME / tickDelay))
			{
				// TODO: Hate to do this, but implement a disconnect.
			}
			// Update the things that are only updated once in a while
			else if (ticksSinceUpdate > 20)
			{
				// Ask if the player is still alive
				// TODO: Prompt for a random number and test for it
				// TODO: Uncomment code when eventhandler no longer runs on several threads
				networkHandler->sendKeepAlive(client, 0);

				// TODO: Give the client the time
				//			timeUpdate(client);

				/* Load and unload chunks when necessary
				// Determine dimensions to check for
				// TODO: Load chunks in circle instead of square
				Position chunkPosition = toChunkPosition(client->position);
				Int minx = chunkPosition.x - client->viewDistance;
				Int maxx = chunkPosition.x + client->viewDistance;
				Int minz = chunkPosition.z - client->viewDistance;
				Int maxz = chunkPosition.z + client->viewDistance;
				Int diameter = client->viewDistance * 2 + 1;
				Boolean* chunkLoaded = new Boolean[diameter * diameter];

				// Set the entire array to false
				for (int i = 0; i < diameter * diameter; ++i)
					chunkLoaded[i] = false;

				// Check if every chunk that needs to be loaded is loaded and unload every other chunk
				for each (std::pair<Int, Int> chunk in client->loadedChunks)
				{
					// Move the chunk to dimensions that should fit the array
					Int x = chunk.first - minx;
					Int z = chunk.second - minz;

					// If the chunk doesn't fit into the array then unload it
					if (x < 0 || z < 0 || x >= diameter || z >= diameter)
					{
						networkHandler->sendUnloadChunk(client, chunk);
					}
					else
						chunkLoaded[x * diameter + z] = true;
				}

				// Load any chunks that have not yet been loaded
				for (Int x = 0; x < diameter; ++x)
					for (Int z = 0; z < diameter; ++z)
						if (!chunkLoaded[x * diameter + z])
							networkHandler->sendChunk(client, x, z, true, true);

				// Free the memory
				delete[] chunkLoaded; */
			}
		}
	}
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
void EventHandler::invalidState(InvalidStateEventArgs e)
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
void EventHandler::clientDisconnect(ClientDisconnectEventArgs e)
{
	// Erase the client
	// TODO: Alert all other players of the disconnect
	std::cout << e.client->getName() << " has disconnected.\n";
//	clients.erase(e.client);
}

/*************************************************
 * EventHandler :: handshake                     *
 * The client is greeting the server             *
 *************************************************/
void EventHandler::handshake(HandShakeEventArgs e)
{
	// Store some info on the client
	e.client->setState(e.state);
	e.client->setProtocolVersion(e.protocolVersion);
}

/*************************************************
 * EventHandler :: legacyServerListPing          *
 * A legacy client is greeting the server        *
 *************************************************/
void EventHandler::legacyServerListPing(LegacyServerListPingEventArgs e)
{

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
	// Set some information for the client
	e.client->setName(e.name);
	e.client->setState(ServerState::Play);
	e.client->setGamemode(Gamemode::Survival);
	e.client->setDimension(Dimension::Overworld);
	e.client->setAbilities(PlayerAbilities(true, false, false, false)); // Invulnerable, but not creative nor flying
	e.client->setPosition(PositionF(0.0, -999.0, 0.0)); // TODO: Remove this in favor of respawn flag
	e.client->resetUptime();
	e.client->resetTicksSinceUpdate();

	// Don't doubt the client, just let them in. ;-)
	// TODO: Create a hash from the client's name
	networkHandler->sendLoginSuccess(e.client, UUID(e.client), e.name);

	// Let the client join the game
	networkHandler->sendJoinGame(e.client, e.client->getEntityID(), Gamemode::Survival, Dimension::Overworld, Difficulty::Peaceful, 8, LevelType::Default);

	// Tell the client the server's brand name
	SerialString brand = SerialString("BareBonesMC");
	networkHandler->sendPluginMessage(e.client, "MC|Brand", (Byte*)brand.makeData(), brand.getSize());

	// Tell the client the map's difficulty again (in case the client is deaf)
	networkHandler->sendServerDifficulty(e.client, Difficulty::Peaceful);

	// Tell the client where the respawn point is (not necessarily where the server will spawn him)
	networkHandler->sendSpawnPosition(e.client, Position(0, 64, 0));

	// Tell the client what his capabilities are
	networkHandler->sendPlayerAbilities(e.client, PlayerAbilities(true, false, false, false), 1.0, 1.0);
}

/*************************************************
 * EventHandler :: animation                     *
 * The client wants to animate its hand          *
 *************************************************/
void EventHandler::animation(AnimationEventArgs e)
{

}

/*************************************************
 * EventHandler :: chatMessage                   *
 * The client is sending a message or command    *
 *************************************************/
void EventHandler::chatMessage(ChatMessageEventArgs e)
{
	// Forward the message to every client that is in play mode
	for (AtomicSet<Client*, ClientComparator>::iterator it = clients.begin(); it != clients.end(); ++it)
		if ((*it)->getState() == ServerState::Play)
			networkHandler->sendChatMessage(*it, e.client->getName() + String(": ") + e.message);
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
	// Save the settings
	e.client->setChatColors(e.chatColors);
	e.client->setChatMode(e.chatMode);
	e.client->setSkinParts(e.displayedSkinParts);
	e.client->setLocale(e.locale);
	e.client->setMainHand(e.mainHand);
	e.client->setViewDistance(e.viewDistance);

	// If the player hasn't spawned yet then spawn them in
	// TODO: Create playerSpawned event
	if (e.client->loadedChunks.empty())
	{
		// Send the chunks over
		for (int x = -3; x <= 3; ++x)
			for (int z = -3; z <= 3; ++z)
				networkHandler->sendChunk(e.client, x, z, true, true);

		// TODO: Use an actual keep alive and teleport id
		networkHandler->sendKeepAlive(e.client, 0);
		networkHandler->sendPlayerPositionAndLook(e.client, PositionF(0.0, 255.0, 0.0), 0.0, 0.0, PlayerPositionAndLookFlags(false, false, false, false, false), 0);
		networkHandler->sendChatMessage(e.client, "Welcome to \\u00a74Super \\u00a76\\u00a7lSMASH \\u00a74Craft\\u00a7r!", ChatMessageType::GameInfo);
	}
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
	e.client->resetTicksSinceUpdate();
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

/*************************************************
 * EventHandler :: playerLook                    *
 * The client is changing its direction (angle)  *
 *************************************************/
void EventHandler::playerLook(PlayerLookEventArgs e)
{
	// Store the data passed by the event
	e.client->setYaw(e.yaw);
	e.client->setPitch(e.pitch);
	e.client->setOnGround(e.onGround);
}

/*************************************************
 * EventHandler :: playerOnGround                *
 * The client is updating the player             *
 *************************************************/
void EventHandler::playerOnGround(PlayerOnGroundEventArgs e)
{
	// Store the data passed by the event
	e.client->setOnGround(e.onGround);
}

/*************************************************
 * EventHandler :: playerPosition                *
 * The client is changing its position           *
 *************************************************/
void EventHandler::playerPosition(PlayerPositionEventArgs e)
{
	// Store the data passed by the event
	e.client->setPosition(e.position);
	e.client->setOnGround(e.onGround);
}

/*************************************************
 * EventHandler :: playerPositionAndLook         *
 * The client is changing its position and angle *
 *************************************************/
void EventHandler::playerPositionAndLook(PlayerPositionAndLookEventArgs e)
{
	// Store the data passed by the event
	e.client->setPosition(e.position);
	e.client->setYaw(e.yaw);
	e.client->setPitch(e.pitch);
	e.client->setOnGround(e.onGround);
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
	// TODO: Use actual teleport id and check whether they are spawning, respawning, or teleporting using a flag
	if (e.client->getPosition().y == -999.0)
	{
		PositionF pos = e.client->getPosition();
		pos.y = 255.0;
		e.client->setPosition(pos);
		e.client->setYaw(0.0);
		e.client->setPitch(90.0);
		networkHandler->sendPlayerPositionAndLook(e.client, pos, 0.0, 90.0, PlayerPositionAndLookFlags(false, false, false, false, false), 0);
	}
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
void EventHandler::vehicleMove(VehicleMoveEventArgs e)
{

}

/*****************************************
 * EventHandler :: getChunkSection       *
 * Returns a chunk at the given position *
 *****************************************/
ChunkSection& EventHandler::getChunkSection(GetChunkSectionEventArgs& e)
{
//	if (e.pos.x > -2 && e.pos.x < 2 && e.pos.y == 12 && e.pos.z > -2 && e.pos.z < 2)
//	if (e.pos.x == 0 && e.pos.y == 12 && e.pos.z == 0)
	if (e.pos.y == 12)
	{
		e.chunk->fillBlocks(BlockID::Cobblestone, 0);
		e.chunk->fillLighting(1, 15);
	}

	return *e.chunk;
}

/************************************************
 * EventHandler :: getChunk                     *
 * Returns a chunk column at the given position *
 ************************************************/
ChunkColumn& EventHandler::getChunk(GetChunkEventArgs& e)
{
	// Grab the chunks for all of the z-levels
	GetChunkSectionEventArgs e2;
	e2.client = e.client;
	e2.pos = Position(e.x, 0, e.z);
	for (int i = 0; i < 16; ++i)
	{
		e2.pos.y = i;
		e2.chunk = &e.chunk.chunks[i];
		getChunkSection(e2);
	}

	return e.chunk;
}

/*****************************************
 * EventHandler :: getBiomes             *
 * Returns the biomes at the given chunk *
 *****************************************/
BiomeID* EventHandler::getBiomes(GetBiomeEventArgs& e)
{
	// Grab the biome for all the given blocks in a chunk
	for (int i = 0; i < 256; ++i)
		e.biomes[i] = BiomeID::TheVoid;

	return e.biomes;
}

/********************************************
 * EventHandler :: EventHandler             *
 * Default Constructor                      *
 ********************************************/
EventHandler::EventHandler() : running(false), networkHandler(NULL), clients() {}

/********************************************
 * EventHandler :: EventHandler             *
 * Destructor                               *
 ********************************************/
EventHandler::~EventHandler() { stopTickClock(); }

/**************************************
 * EventHandler :: startTick          *
 * Starts the server's game tick loop *
 * The delay is in micro seconds      *
 **************************************/
void EventHandler::startTickClock(Double delay)
{
	// If the tick hasn't started already then start it
	if (!running)
	{
		tickDelay = delay;
		running = true;
		tickClock = std::thread(&EventHandler::runTickClock, this);
	}
}

/*************************************
 * EventHandler :: stopTick          *
 * Stops the server's game tick loop *
 *************************************/
void EventHandler::stopTickClock()
{
	// Turn off the tick clock and wait for it to finish up
	running = false;
	if (tickClock.joinable())
		tickClock.join();
}

