#include "servereventhandler.h"
#include <ctime>

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

/*******************************
 * toBytes                     *
 * Converts a float to 4 bytes *
 *******************************/
void toBytes(const float value, char* bytes)
{
	Int val = reinterpret_cast<const Int&>(value);
	toBytes(val, bytes);
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

/**********************************
 * toBytes                        *
 * Converts a position to 8 bytes *
 **********************************/
void toBytes(const Position value, char* bytes)
{
	toBytes(value.getData(), bytes);
}

/*****************************************
 * ServerEventHandler :: runTickClock    *
 * Runs the onTick event regularly after *
 * the startTickClock method is run      *
 *****************************************/
void ServerEventHandler::runTickClock()
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

/********************************
 * ServerEventHandler :: onTick *
 * A tick in the game's loop    *
 ********************************/
void ServerEventHandler::onTick(Double dt, Int ticksSkipped)
{
	// Run this on every client
	for each (std::pair<SOCKET, Client*> var in clients)
	{
		// Set up the local variables
		SOCKET socket = var.first;
		Client* client = var.second;

		// Update the client's ticks
		client->uptime += 1 + ticksSkipped;
		client->ticksSinceUpdate += 1 + ticksSkipped;

		// TODO: Update the things that need to be updated every tick


		// If the client has been out for too long, disconnect it. :(
		if (client->ticksSinceUpdate > static_cast<Double>(DISCONNECT_TIME / tickDelay))
		{
			// TODO: Hate to do this, but implement a disconnect.
		}
		// TODO: Update the things that are only updated once in a while
		else if (client->ticksSinceUpdate > 20)
		{
			// Give the client the time
			timeUpdate(client);
		}
	}
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
 * ServerEventHandler :: loginFailed      *
 * Disconnects the client for some reason *
 ******************************************/
void ServerEventHandler::loginFailed(Client* client)
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

	// Send the loginSuccess packet
	send(client->socket, (const char*)length.getData(), length.getSize(), NULL);
	send(client->socket, (const char*)packet.getData(), packet.getSize(), NULL);
	send(client->socket, data1, UUID.getSize(), NULL);
	send(client->socket, data2, name.getSize(), NULL);

	// Delete the sent data
	delete[] data1;
	delete[] data2;

	// Tell the client to join the gamme
	joinGame(client);}

/**************************************************************
 * ServerEventHandler :: setCompression                       *
 * Tells the client to compress and expect compressed packets *
 **************************************************************/
void ServerEventHandler::setCompression(Client* client)
{

}

/**********************************************
* ServerEventHandler :: spawnObject           *
* Tells the client that an object was created *
***********************************************/
void ServerEventHandler::spawnObject(Client* client)
{

}

/*******************************************************
 * ServerEventHandler :: spawnExperienceOrb            *
 * Tells the client that an experience orb was created *
 *******************************************************/
void ServerEventHandler::spawnExperienceOrb(Client* client)
{

}

/*******************************************
 * ServerEventHandler :: spawnGlobalEntity *
 * Tells the client about a thunderstrike  *
 *******************************************/
void ServerEventHandler::spawnGlobalEntity(Client* client)
{

}

/***************************************
 * ServerEventHandler :: spawnMob      *
 * Tells the client that a mob spawned *
 ***************************************/
void ServerEventHandler::spawnMob(Client* client)
{

}

/******************************************
 * ServerEventHandler :: spawnPainting    *
 * Tells the client to display a painting *
 ******************************************/
void ServerEventHandler::spawnPainting(Client* client)
{

}

/*********************************************
 * ServerEventHandler :: spawnPlayer         *
 * Tells the client that a player is visible *
 *********************************************/
void ServerEventHandler::spawnPlayer(Client* client)
{

}

/************************************
 * ServerEventHandler :: statistics *
 * Gives the client its statistcs   *
 ************************************/
void ServerEventHandler::statistics(Client* client)
{

}

/***************************************************************
 * ServerEventHandler :: blockBreakAnimation                   *
 * Tells the client to display a breaking animation on a block *
 ***************************************************************/
void ServerEventHandler::blockBreakAnimation(Client* client)
{

}

/****************************************************************
 * ServerEventHandler :: updateBlockEntity                      *
 * Tells the client to set the tile entity with the given block *
 ****************************************************************/
void ServerEventHandler::updateBlockEntity(Client* client)
{

}

/***************************************
 * ServerEventHandler :: blockAction   *
 * Tells the client to animate a block *
 ***************************************/
void ServerEventHandler::blockAction(Client* client)
{

}

/****************************************************************
 * ServerEventHandler :: blockChange                            *
 * Tells the client that a block within view has changed        *
 * Must NOT be used to change blocks in unloaded client chunks! *
 ****************************************************************/
void ServerEventHandler::blockChange(Client* client)
{

}

/********************************************************
 * ServerEventHandler :: bossBar                        *
 * Tells the client to display/update a boss health bar *
 ********************************************************/
void ServerEventHandler::bossBar(Client* client)
{

}

/**************************************************
 * ServerEventHandler :: serverDifficulty         *
 * Tells the client to change the game difficulty *
 **************************************************/
void ServerEventHandler::serverDifficulty(Client* client)
{
	// Set up the data
	VarInt packet = VarInt(0x0D);
	VarInt length = VarInt(packet.getSize() + 1);
	char difficulty = 3; // Hard difficulty
	Int size = packet.getSize() + length.getSize() + 1;
	char* data = new char[size];

	// Copy the length over
	int i;
	for (i = 0; i < length.getSize(); i++)
		data[i] = length.getData()[i];

	// Copy the packet over
	for (int x = 0; x < packet.getSize(); x++, i++)
		data[i] = packet.getData()[x];

	// Copy the difficulty over
	data[i];

	// Send the data
	send(client->socket, data, size, NULL);

	// Delete the data now that we're done with it
	delete[] data;
}

/*************************************************************
 * ServerEventHandler :: tabComplete                         *
 * Tells the client the results of auto-completing a command *
 *************************************************************/
void ServerEventHandler::tabComplete(Client* client)
{

}

/*************************************
 * ServerEventHandler :: chatMessage *
 * Sends a message to the client     *
 *************************************/
void ServerEventHandler::chatMessage(Client* client)
{

}

/****************************************************************
 * ServerEventHandler :: multiBlockChange                       *
 * Tells the client to update several blocks within its view    *
 * Must NOT be used to change blocks in unloaded client chunks! *
 ****************************************************************/
void ServerEventHandler::multiBlockChange(Client* client)
{

}

/**********************************************************
 * ServerEventHandler :: confirmTransaction               *
 * Tells the client that a request was accepted or denied *
 **********************************************************/
void ServerEventHandler::confirmTransaction(Client* client)
{

}

/******************************************
 * ServerEventHandler :: closeWindow      *
 * Tells the client to close a gui window *
 ******************************************/
void ServerEventHandler::closeWindow(Client* client)
{

}

/*****************************************
 * ServerEventHandler :: openWindow      *
 * Tells the client to open a gui window *
 *****************************************/
void ServerEventHandler::openWindow(Client* client)
{

}

/***********************************************************
 * ServerEventHandler :: windowItems                       *
 * Tells the client to update certain slots within the gui *
 ***********************************************************/
void ServerEventHandler::windowItems(Client* client)
{
	// Set up the variable integers
	VarInt packet = VarInt(0x14);
	VarInt length = VarInt(packet.getSize() + 93);
	Int size = length.getSize() + length.toInt();

	// Create the data array
	char* data = new char[size];

	// Copy the length into the byte array
	Int i;
	const char* data2 = (const char*)length.getData();
	for (i = 0; i < length.getSize(); i++)
		data[i] = data2[i];

	// Copy the packet into the byte array
	data2 = (const char*)packet.getData();
	for (int a = 0; a < packet.getSize(); a++, i++)
		data[i] = data2[a];
	
	// Copy the window id into the byte array
	data[i++] = 0;

	// Copy the count into the byte array
	data[i++] = 0;
	data[i++] = 45;

	// Copy the slot data into the byte array
	for (int x = 0; x < 90; x++, i++)
		data[i] = -1;

	// Send the data
	send(client->socket, data, size, NULL);

	// Delete the array now that we're done
	delete[] data;
}

/*****************************************************************
 * ServerEventHandler :: windowProperty                          *
 * Tells the client to update an animation or picture in the gui *
 *****************************************************************/
void ServerEventHandler::windowProperty(Client* client)
{

}

/*******************************************************************
 * ServerEventHandler :: setSlot                                   *
 * Tells the client that an item slot in a window has been changed *
 *******************************************************************/
void ServerEventHandler::setSlot(Client* client)
{

}

/***********************************************************************
 * ServerEventHandler :: setCooldown                                   *
 * Tells the client to apply a cooldown to all items of the given type *
 ***********************************************************************/
void ServerEventHandler::setCooldown(Client* client)
{

}

/*******************************************************************
 * ServerEventHandler :: pluginMessage                             *
 * Sends data to the client that may or may not be related to mods *
 *******************************************************************/
void ServerEventHandler::pluginMessage(Client* client, SerialString name, char* data, Int dataLen)
{
	// Compute the serial strings and its length
	VarInt packet = VarInt(0x18);
	char* pluginData = name.makeData();
	VarInt length = packet.getSize() + name.getSize() + dataLen;

	// Send the data
	send(client->socket, (const char*)length.getData(), length.getSize(), NULL);
	send(client->socket, (const char*)packet.getData(), packet.getSize(), NULL);
	send(client->socket, pluginData, name.getSize(), NULL);
	send(client->socket, data, dataLen, NULL);

	// Delete the data now that we're done with it
	delete[] pluginData;
}

/*********************************************
 * ServerEventHandler :: namedSoundEffect    *
 * Tells the client to play a specific sound *
 *********************************************/
void ServerEventHandler::namedSoundEffect(Client* client)
{

}

/************************************
 * ServerEventHandler :: disconnect *
 * Kicks the client for reasons     *
 ************************************/
void ServerEventHandler::disconnect(Client* client)
{

}

/*****************************************************
 * ServerEventHandler :: entityStatus                *
 * Tells the client to trigger an entity's animation *
 *****************************************************/
void ServerEventHandler::entityStatus(Client* client)
{

}

/***************************************
 * ServerEventHandler :: explosion     *
 * Tells the client to explode an area *
 ***************************************/
void ServerEventHandler::explosion(Client* client)
{

}

/**************************************
 * ServerEventHandler :: unloadChunk  *
 * Tells the client to unload a chunk *
 **************************************/
void ServerEventHandler::unloadChunk(Client* client)
{

}

/*******************************************************************
 * ServerEventHandler :: changeGameState                           *
 * Tells the client to change the weather, change the gamemode,    *
 * display a demo message, show the credits, fade the sky, display *
 * an elder guardian, hit the client with an arrow from another    *
 * client, or to tell the client it selected an invalid bed.       *
 *******************************************************************/
void ServerEventHandler::changeGameState(Client* client)
{

}

/*************************************************************
 * ServerEventHandler :: keepAlive                           *
 * Tells the client to reply or be botted from the server    *
 * The client also expects this command or will believe that *
 * the server has timed out.                                 *
 *************************************************************/
void ServerEventHandler::keepAlive(Client* client)
{

}

/*************************************************
 * ServerEventHandler :: chunkData               *
 * Sends data of a specific chunk to the client  *
 *************************************************/
void ServerEventHandler::chunkData(Client* client, Int x, Int y)
{
	// Common variables
	VarInt packet = VarInt(0x20);
	char* data;

	// Generate a stone platform at chunk (0, 0) and air anywhere else
	if (false/*x == 0 && y == 0*/)
	{

	}
	else	// Generate air here
	{
		// Set up the variable integers
		VarInt bitmask = VarInt(0);
		VarInt dataSize = VarInt(256);
		VarInt numEntities = VarInt(0);
		VarInt length = VarInt(packet.getSize() + bitmask.getSize()
			+ dataSize.getSize() + numEntities.getSize() + 265);
		Int size = length.getSize() + length.toInt();

		// Create the data array
		data = new char[size];

		// Copy the length into the byte array
		Int i;
		const char* data2 = (const char*)length.getData();
		for (i = 0; i < length.getSize(); i++)
			data[i] = data2[i];

		// Copy the packet into the byte array
		data2 = (const char*)packet.getData();
		for (int a = 0; a < packet.getSize(); a++, i++)
			data[i] = data2[a];

		// Copy the x coordinate into the byte array
		toBytes(x, data + i);
		i += 4;

		// Copy the y coordinate into the byte array
		toBytes(y, data + i);
		i += 4;

		// Copy the ground-up continuous flag into the byte array
		data[i++] = true;

		// Copy the primary bit mask into the byte array
		data2 = (const char*)bitmask.getData();
		for (int a = 0; a < bitmask.getSize(); a++, i++)
			data[i] = data2[a];

		// Copy the chunk data size into the byte array
		data2 = (const char*)dataSize.getData();
		for (int a = 0; a < dataSize.getSize(); a++, i++)
			data[i] = data2[a];

		// Skip the actual chunk data - it's all air

		// Copy the biome data into the byte array, make it plains for now
		for (int a = 0; a < 256; a++, i++)
			data[i] = 1;

		// Copy the number of entities into the byte array
		data2 = (const char*)numEntities.getData();
		for (int a = 0; a < numEntities.getSize(); a++, i++)
			data[i] = data2[a];

		// Skip the block entities for now, there are none.

		// Send the data
		send(client->socket, data, size, NULL);
	}

	// Delete the data now that we're done with it
	delete[] data;
}

/************************************************************
 * ServerEventHandler :: chunkData                          *
 * Sends data of all chunks in a 7x7 area around the client *
 ************************************************************/
void ServerEventHandler::chunkData(Client* client)
{
	// Calculate the chunk area around the client
	Int beginx = static_cast<Int>(client->x / 16) - 3;
	Int endx = static_cast<Int>(client->x / 16) + 3;
	Int beginy = static_cast<Int>(client->y / 16) - 3;
	Int endy = static_cast<Int>(client->y / 16) + 3;

	// Send all of those chunks to the client
	for (int x = beginx; x <= endx; x++)
		for (int y = beginy; y <= endy; y++)
			chunkData(client, x, y);
}

/**********************************************************
 * ServerEventHandler :: effect                           *
 * Tells the client to play a sound or display a particle *
 **********************************************************/
void ServerEventHandler::effect(Client* client)
{

}

/******************************************
 * ServerEventHandler :: particle         *
 * Tells the client to display a particle *
 ******************************************/
void ServerEventHandler::particle(Client* client)
{

}

/********************************************
 * ServerEventHandler :: joinGame           *
 * Gives the client the ok to join the game *
 ********************************************/
void ServerEventHandler::joinGame(Client* client)
{
	// Give the client an entity ID
	client->entityID = client->socket;

	// Make some data
	VarInt packet = VarInt(0x23); // Join Game packet
	UByte gamemode = 0;			  // Survival
	Int dimension = 0;			  // Overworld
	UByte difficulty = 3;		  // Hard
	UByte maxPlayers = 255;		  // The more the merrier!
	SerialString levelType = SerialString("default");
	Boolean reducedDebugInfo = false;

	// Create the byte array
	Int length = packet.getSize() + levelType.getSize() + 12;
	VarInt varLength = VarInt(length);
	Int size = length + varLength.getSize();
	char* data = new char[size];

	// Copy the length into the byte array
	Int i;
	const char* data2 = (const char*)varLength.getData();
	for (i = 0; i < varLength.getSize(); i++)
		data[i] = data2[i];

	// Copy the packet into the byte array
	data2 = (const char*)packet.getData();
	for (int x = 0; x < packet.getSize(); x++, i++)
		data[i] = data2[x];

	// Copy the entity id into the byte array
	toBytes(client->entityID, data + i);
	i += 4;

	// Copy the gamemode into the byte array
	data[i++] = gamemode;

	//Copy the dimension into the byte array
	toBytes(dimension, data + i);
	i += 4;

	// Copy the difficulty into the byte array
	data[i++] = difficulty;

	// Copy the maximum players into the byte array
	data[i++] = maxPlayers;

	// Copy the level type into the byte array
	char* levelData = levelType.makeData();
	for (int x = 0; x < levelType.getSize(); x++, i++)
		data[i] = levelData[x];

	// Copy the debug info flag into the byte array
	data[i] = reducedDebugInfo;

	// Send that data
	send(client->socket, data, size, NULL);

	// Delete the used data afterwards
	delete[] levelData;
	delete[] data;

	/*
	// Tell the client the coordinates of the spawn point
	spawnPosition(client);

	// Tell the client to respawn
	respawn(client);

	// Give the client the chunks and entities around it
	chunkData(client);

	// Tell the client its position
	playerPositionAndLook(client);
	*/

	// Give the client the name of our server
	SerialString str = SerialString("vanilla");
	data = str.makeData();
	pluginMessage(client, SerialString("MC|Brand"), data, str.getSize());
	delete[] data;

	// Reiterate the difficulty to the client
	serverDifficulty(client);

	// Give the spawnpoint position to the client
	spawnPosition(client);

	// Give the client its abilities
	playerAbilities(client);

	// Remember that the client is now playing
	client->state = Play;
}

/*****************************************
 * ServerEventHandler :: map             *
 * Tells the client to update a map item *
 *****************************************/
void ServerEventHandler::map(Client* client)
{

}

/*******************************************************
 * ServerEventHandler :: entityRelativeMove            *
 * Tells the client to move an entity at most 8 blocks *
 *******************************************************/
void ServerEventHandler::entityRelativeMove(Client* client)
{

}

/****************************************************************
 * ServerEventHandler :: entityLookAndRelativeMove              *
 * Tells the client to move an entity and change its view angle *
 ****************************************************************/
void ServerEventHandler::entityLookAndRelativeMove(Client* client)
{

}

/*****************************************************
 * ServerEventHandler :: entityLook                  *
 * Tells the client to change an entity's view angle *
 *****************************************************/
void ServerEventHandler::entityLook(Client* client)
{

}

/**************************************
 * ServerEventHandler :: vehicleMove  *
 * Tells the client to move a vehicle *
 **************************************/
void ServerEventHandler::vehicleMove(Client* client)
{

}

/****************************************************
 * ServerEventHandler :: openSignEditor             *
 * Tells the client to open the sign editing window *
 ****************************************************/
void ServerEventHandler::openSignEditor(Client* client)
{

}

/********************************************************
 * ServerEventHandler :: playerAbilities                *
 * Tells the client to enable god mode, flying mode,    *
 * creative mode, or change its speed and field of view *                      *
 ********************************************************/
void ServerEventHandler::playerAbilities(Client* client)
{
	// Set up the data
	VarInt packet = VarInt(0x2B);
	VarInt length = VarInt(packet.getSize() + 9);
	Byte flags = 0;	// No extra abilities set
	Float flyingSpeed = 0.0f;
	Float fovMod = 1.0f;
	Int size = packet.getSize() + length.getSize() + 9;
	char* data = new char[size];

	// Copy the length over
	int i;
	for (i = 0; i < length.getSize(); i++)
		data[i] = length.getData()[i];

	// Copy the packet over
	for (int x = 0; x < packet.getSize(); x++, i++)
		data[i] = packet.getData()[x];

	// Copy the flags over
	data[i++] = flags;

	// Copy the flying speed over
	toBytes(flyingSpeed, data + i);
	i += 4;

	// Copy the field of view modifier over
	toBytes(fovMod, data + i);

	// Send the data
	send(client->socket, data, size, NULL);

	// Delete the data now that its used
	delete[] data;
}

/*********************************************
 * ServerEventHandler :: combatEvent         *
 * Supposedly used for combat...             *
 * TODO: Research server->client combatEvent *
 *********************************************/
void ServerEventHandler::combatEvent(Client* client)
{

}

/********************************************
 * ServerEventHandler :: playerListItem     *
 * Tells the client to update the user list *
 ********************************************/
void ServerEventHandler::playerListItem(Client* client)
{

}

/**************************************************
 * ServerEventHandler :: playerPositionAndLook    *
 * Tells the client update its position and angle *
 **************************************************/
void ServerEventHandler::playerPositionAndLook(Client* client, Double x, Double y, Double z, Float yaw, Float pitch, Byte flags)
{
	// Use the client's socket xor'ed by its protocol as its confirmation id
	VarInt id = VarInt(client->socket ^ client->protocol);

	// Create an array to store all the data
	VarInt packet = VarInt(0x2e);
	VarInt length = VarInt(id.getSize() + packet.getSize() + 33);
	Int size = length.getSize() + length.toInt();
	char* data = new char[size];

	// Copy the length over
	int i;
	for (i = 0; i < length.getSize(); i++)
		data[i] = length.getData()[i];

	// Copy the packet over
	for (int a = 0; a < packet.getSize(); a++, i++)
		data[i] = packet.getData()[a];

	// Copy the x coordinate over
	toBytes(x, data + i);
	i += 8;

	// Copy the y coordinate over
	toBytes(y, data + i);
	i += 8;

	// Copy the z coordinate over
	toBytes(z, data + i);
	i += 8;

	// Copy the yaw over
	toBytes(yaw, data + i);
	i += 4;

	// Copy the pitch over
	toBytes(pitch, data + i);
	i += 4;

	// Copy the flags over
	data[i++] = flags;

	// Copy the id over
	for (int a = 0; a < id.getSize(); a++, i++)
		data[i] = id.getData()[a];
}

/**********************************************************
 * ServerEventHandler :: useBed                           *
 * Tells the client that it or another player went to bed *
 **********************************************************/
void ServerEventHandler::useBed(Client* client)
{

}

/***************************************************************
 * ServerEventHandler :: destroyEntities                       *
 * Tells the client that a list of entities has been destroyed *
 ***************************************************************/
void ServerEventHandler::destroyEntities(Client* client)
{

}

/*************************************************
 * ServerEventHandler :: removeEntityEffect      *
 * Tells the client to stop displaying an effect *
 *************************************************/
void ServerEventHandler::removeEntityEffect(Client* client)
{

}

/*******************************************************
 * ServerEventHandler :: resourcePackSend              *
 * Sends a resource pack and asks the client to use it *
 *******************************************************/
void ServerEventHandler::resourcePackSend(Client* client)
{

}

/***********************************************************************
 * ServerEventHandler :: entityHeadLook                                *
 * Tells the client to change the direction an entity's head is facing *
 ***********************************************************************/
void ServerEventHandler::entityHeadLook(Client* client)
{

}

/**********************************************
 * ServerEventHandler :: worldBorder          *
 * Tells the client to display a world border *
 **********************************************/
void ServerEventHandler::worldBorder(Client* client)
{

}

/**************************************************************
 * ServerEventHandler :: camera                               *
 * Tells the client to change its viewpoint to another entity *
 **************************************************************/
void ServerEventHandler::camera(Client* client)
{

}

/************************************************************
 * ServerEventHandler :: heldItemChange                     *
 * Tells the client that its selected item slot has changed *
 ************************************************************/
void ServerEventHandler::heldItemChange(Client* client)
{

}

/********************************************
 * ServerEventHandler :: displayScoreboard  *
 * Tells the client to display a socreboard *
 ********************************************/
void ServerEventHandler::displayScoreBoard(Client* client)
{

}

/********************************************************
 * ServerEventHandler :: entityMetadata                 *
 * Tells the client to update the metadata of an entity *
 ********************************************************/
void ServerEventHandler::entityMetadata(Client* client)
{

}

/****************************************************************
 * ServerEventHandler :: attachEntity                           *
 * Tells the client that an entity is leashed to another entity *
 ****************************************************************/
void ServerEventHandler::attachEntity(Client* client)
{

}

/**********************************************
 * ServerEventHandler :: entityVelocity       *
 * Gives the client the velocity of an entity *
 **********************************************/
void ServerEventHandler::entityVelocity(Client* client)
{

}

/****************************************************
 * ServerEventHandler :: Entity Equipment           *
 * Tells the client to change an entity's equipment *
 ****************************************************/
void ServerEventHandler::entityEquipment(Client* client)
{

}

/*********************************************
 * ServerEventHandler :: setExperience       *
 * Changes the client's experience and level *
 *********************************************/
void ServerEventHandler::setExperience(Client* client)
{

}

/****************************************************
 * ServerEventHandler :: updateHealth               *
 * Tells the client to update its health and hunger *
 ****************************************************/
void ServerEventHandler::updateHealth(Client* client)
{

}

/************************************************************
 * ServerEventHandler :: scoreboardObjective                *
 * Tells the client to add or remove a scoreboard objective *
 ************************************************************/
void ServerEventHandler::scoreboardObjective(Client* client)
{

}

/*******************************************************
 * ServerEventHandler :: setPassengers                 *
 * Tells the client to set the passengers in a vehicle *
 *******************************************************/
void ServerEventHandler::setPassengers(Client* client)
{

}

/***********************************************
 * ServerEventHandler :: teams                 *
 * Tells the client to create or update a team *
 ***********************************************/
void ServerEventHandler::teams(Client* client)
{

}

/************************************************
 * ServerEventHandler :: updateScore            *
 * Tells the client to update a scoreboard item *
 ************************************************/
void ServerEventHandler::updateScore(Client* client)
{

}

/***************************************
 * ServerEventHandler :: spawnPosition *
 * Tells the client where to spawn     *
 ***************************************/
void ServerEventHandler::spawnPosition(Client* client)
{
	// Set up the data
	VarInt packet = VarInt(0x43);
	VarInt length = VarInt(packet.getSize() + 8);
	Long spawnPoint = Position(0, 60, 0).getData();
	Int size = packet.getSize() + length.getSize() + 8;
	char* data = new char[size];

	// Copy the length over
	int i;
	for (i = 0; i < length.getSize(); i++)
		data[i] = length.getData()[i];

	// Copy the packet over
	for (int x = 0; x < packet.getSize(); x++, i++)
		data[i] = packet.getData()[x];

	// Copy the spawn point over
	toBytes(spawnPoint, data + i);

	// Send the data
	send(client->socket, data, size, NULL);

	// Delete the data now that we're done with it
	delete[] data;
}

/************************************
 * ServerEventHandler :: timeUpdate *
 * Updates the client's world time  *
 ************************************/
void ServerEventHandler::timeUpdate(Client* client)
{
	// Create an array to store all of the data
	VarInt packet = VarInt(0x44);
	VarInt length = VarInt(packet.getSize() + 8);
	Int size = length.getSize() + length.toInt();
	char* data = new char[size];

	// Copy the length over
	int i;
	for (i = 0; i < length.getSize(); i++)
		data[i] = length.getData()[i];

	// Copy the packet over
	for (int a = 0; a < packet.getSize(); a++, i++)
		data[i] = packet.getData()[a];

	// Copy the world age
	toBytes(0, data + i);
	i += 8;

	// Copy the time of day
	toBytes(6000, data + i);

	// Send the data
	send(client->socket, data, size, NULL);
	
	// Delete the data now that we're done using it
	delete[] data;
}

/***********************************************
 * ServerEventHandler :: title                 *
 * Tells the client to display a title message *
 ***********************************************/
void ServerEventHandler::title(Client* client)
{

}

/**********************************************
 * ServerEventHandler :: soundEffect          *
 * Tells the client to play a hardcoded sound *
 **********************************************/
void ServerEventHandler::soundEffect(Client* client)
{

}

/*****************************************************************************
 * ServerEventHandler :: playerListHeaderAndFooter                           *
 * Tells the client to display extra text above and/or below the player list *
 *****************************************************************************/
void ServerEventHandler::playerListHeaderAndFooter(Client* client)
{

}

/***********************************************************
 * ServerEventHandler :: collectItem                       *
 * Tells the client to show that someone picked up an item *
 ***********************************************************/
void ServerEventHandler::collectItem(Client* client)
{

}

/************************************************************
 * ServerEventHandler :: entityTeleport                     *
 * Tells the client that an entity moved more than 4 blocks *
 ************************************************************/
void ServerEventHandler::entityTeleport(Client* client)
{

}

/*****************************************************
 * ServerEventHandler :: entityProperties            *
 * Tells the client to change an entity's properties *
 *****************************************************/
void ServerEventHandler::entityProperties(Client* client)
{

}

/******************************************************
 * ServerEventHandler :: entityEffect                 *
 * Tells the client to display an effect on an entity *
 ******************************************************/
void ServerEventHandler::entityEffect(Client* client)
{

}

/********************************************
 * ServerEventHandler :: ServerEventHandler *
 * Default Constructor                      *
 ********************************************/
ServerEventHandler::ServerEventHandler() : running(false) {}

/********************************************
 * ServerEventHandler :: ServerEventHandler *
 * Destructor                               *
 ********************************************/
ServerEventHandler::~ServerEventHandler() { stopTickClock(); }

/**************************************
 * ServerEventHandler :: startTick    *
 * Starts the server's game tick loop *
 * The delay is in micro seconds      *
 **************************************/
void ServerEventHandler::startTickClock(Double delay)
{
	// If the tick hasn't started already then start it
	if (!running)
	{
		tickDelay = delay;
		running = true;
		tickClock = std::thread(&ServerEventHandler::runTickClock, this);
	}
}

/*************************************
 * ServerEventHandler :: stopTick    *
 * Stops the server's game tick loop *
 *************************************/
void ServerEventHandler::stopTickClock()
{
	// Turn off the tick clock and wait for it to finish up
	running = false;
	if (tickClock.joinable())
		tickClock.join();
}

