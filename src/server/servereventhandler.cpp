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
void ServerEventHandler::pluginMessage(Client* client)
{

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

/***********************************
 * ServerEventHandler :: chunkData *
 * Sends chunk data to the client  *
 ***********************************/
void ServerEventHandler::chunkData(Client* client)
{

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
void ServerEventHandler::playerPositionAndLook(Client* client)
{

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

}

/************************************
 * ServerEventHandler :: timeUpdate *
 * Updates the client's world time  *
 ************************************/
void ServerEventHandler::timeUpdate(Client* client)
{

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

