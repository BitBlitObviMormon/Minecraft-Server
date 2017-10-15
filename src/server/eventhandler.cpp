#include "eventhandler.h"
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

/********************************
 * EventHandler :: onTick       *
 * A tick in the game's loop    *
 ********************************/
void EventHandler::onTick(Double dt, Int ticksSkipped)
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
			// Ask if the player is still alive
//			keepAlive(client);

			// Give the client the time
//			timeUpdate(client);
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

/*************************************************
 * EventHandler :: handshake                     *
 * The client is greeting the server             *
 *************************************************/
void EventHandler::handshake(HandShakeEventArgs e)
{
	if (e.state != Login && e.state != Status)
		;	// TODO: Disconnect the user
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

/*************************************************
 * EventHandler :: playerLook                    *
 * The client is changing its direction (angle)  *
 *************************************************/
void EventHandler::playerLook(PlayerLookEventArgs e)
{

}

/*************************************************
 * EventHandler :: playerOnGround                *
 * The client is updating the player             *
 *************************************************/
void EventHandler::playerOnGround(PlayerOnGroundEventArgs e)
{

}

/*************************************************
 * EventHandler :: playerPosition                *
 * The client is changing its position           *
 *************************************************/
void EventHandler::playerPosition(PlayerPositionEventArgs e)
{

}

/*************************************************
 * EventHandler :: playerPositionAndLook         *
 * The client is changing its position and angle *
 *************************************************/
void EventHandler::playerPositionAndLook(PlayerPositionAndLookEventArgs e)
{

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
void EventHandler::vehicleMove(VehicleMoveEventArgs e)
{

}

/********************************************
 * EventHandler :: EventHandler             *
 * Default Constructor                      *
 ********************************************/
EventHandler::EventHandler() : running(false) {}

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

