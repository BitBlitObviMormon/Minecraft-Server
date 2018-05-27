#include "debug.h"
#include "client/client.h"
#include <sstream>
#include <mutex>
using namespace std;

/***********************
 * Client :: Client    *
 * Default constructor *
 ***********************/
Client::Client(SOCKET newClient) : socket(newClient), state(ServerState::Handshaking)
{
	// Calculate the name
	std::stringstream ss;
	ss << "Player " << socket;
	name = ss.str();

	// Calculate the UUID
	uuid = UUID(this);
}

/********************
 * Client :: Client *
 * Destructor       *
 ********************/
Client::~Client() 
{
	// Close the socket if the client is destroyed
	if (socket != NULL)
	{
		closesocket(socket);
		socket = NULL;
	}
}

/************************************************
 * Client :: getMainHand                        *
 * Returns which hand is the player's main hand *
 ************************************************/
const MainHand& Client::getMainHand()
{
	lock_guard<mutex> lock(dataLock);
	return mainHand;
}

/*************************************
 * Client :: getIsDead               *
 * Whether or not the player is dead *
 *************************************/
const Boolean& Client::getIsDead()
{
	lock_guard<mutex> lock(dataLock);
	return isDead;
}

/*****************************************************
 * Client :: getChatColors                           *
 * Whether the client wants to receive colors or not *
 *****************************************************/
const Boolean& Client::getChatColors()
{
	lock_guard<mutex> lock(dataLock);
	return chatColors;
}

/****************************************************
 * Client :: getOnGround                            *
 * Whether the player is on the floor or in the air *
 ****************************************************/
const Boolean& Client::getOnGround()
{
	lock_guard<mutex> lock(dataLock);
	return onGround;
}

/******************************************************************
 * Client :: getChatMode                                          *
 * What kind of chat / command output the client wants to receive *
 ******************************************************************/
const ChatMode& Client::getChatMode()
{
	lock_guard<mutex> lock(dataLock);
	return chatMode;
}

/*********************************************************************
 * Client :: getState                                                *
 * Whether the player is connecting, playing, or changing its status *
 *********************************************************************/
const ServerState& Client::getState()
{
	lock_guard<mutex> lock(dataLock);
	return state;
}

/*******************************************
 * Client :: getGamemode                   *
 * The gamemode the player is playing with *
 *******************************************/
const Gamemode& Client::getGamemode()
{
	lock_guard<mutex> lock(dataLock);
	return gamemode;
}

/**********************************
 * Client :: getDimension         *
 * The dimension the player is in *
 **********************************/
const Dimension& Client::getDimension()
{
	lock_guard<mutex> lock(dataLock);
	return dimension;
}

/**********************************************
 * Client :: getViewDistance                  *
 * How far the player's client is set to view *
 **********************************************/
const Byte& Client::getViewDistance()
{
	lock_guard<mutex> lock(dataLock);
	return viewDistance;
}

/**********************************************
 * Client :: getSkinParts                     *
 * Which skin parts the client set to visible *
 **********************************************/
const DisplayedSkinParts& Client::getSkinParts()
{
	lock_guard<mutex> lock(dataLock);
	return skinParts;
}

/************************************************************
 * Client :: getAbilities                                   *
 * Whether the client is creative, vulnerable, flying, etc. *
 ************************************************************/
const PlayerAbilities& Client::getAbilities()
{
	lock_guard<mutex> lock(dataLock);
	return abilities;
}

/******************************************************
 * Client :: getTicksSinceUpdate                      *
 * How many ticks it's been since a ping was received *
 ******************************************************/
const Int& Client::getTicksSinceUpdate()
{
	lock_guard<mutex> lock(dataLock);
	return ticksSinceUpdate;
}

/***********************************************************
 * Client :: getProtocolVersion                            *
 * The version of the network protocol the client is using *
 ***********************************************************/
const Int& Client::getProtocolVersion()
{
	lock_guard<mutex> lock(dataLock);
	return protocolVersion;
}

/*********************************
 * Client :: getEntityID         *
 * The player's unique entity id *
 *********************************/
const Int& Client::getEntityID()
{
	lock_guard<mutex> lock(dataLock);
	return entityID;
}

/*****************************************
 * Client :: getSocket                   *
 * The socket the player is connected on *
 *****************************************/
const SOCKET& Client::getSocket()
{
	lock_guard<mutex> lock(dataLock);
	return socket;
}

/*********************************
 * Client :: getYaw              *
 * The player camera's yaw angle *
 *********************************/
const Float& Client::getYaw()
{
	lock_guard<mutex> lock(dataLock);
	return yaw;
}

/***********************************
 * Client :: getPitch              *
 * The player camera's pitch angle *
 ***********************************/
const Float& Client::getPitch()
{
	lock_guard<mutex> lock(dataLock);
	return pitch;
}

/*********************************************
 * Client :: getUptime                       *
 * How many ticks since the player connected *
 *********************************************/
const Long& Client::getUptime()
{
	lock_guard<mutex> lock(dataLock);
	return uptime;
}

/************************************************
 * Client :: getUUID                            *
 * The unique id that is assigned to the player *
 ************************************************/
const UUID& Client::getUUID()
{
	lock_guard<mutex> lock(dataLock);
	return uuid;
}

/**********************************
 * Client :: getPosition          *
 * The player's absolute position *
 **********************************/
const PositionF& Client::getPosition()
{
	lock_guard<mutex> lock(dataLock);
	return position;
}

/*********************
 * Client :: getName *
 * The player's name *
 *********************/
const String& Client::getName()
{
	lock_guard<mutex> lock(dataLock);
	return name;
}

/************************************************
 * Client :: getLocale                          *
 * The language the player is using (ie. en_US) *
 ************************************************/
const String& Client::getLocale()
{
	lock_guard<mutex> lock(dataLock);
	return locale;
}

/******************************************************
 * Client :: getBrand                                 *
 * The type of client (usually vanilla unless modded) *
 ******************************************************/
const String& Client::getBrand()
{
	lock_guard<mutex> lock(dataLock);
	return brand;
}

/******************************
 * Client :: setMainHand      *
 * Set the player's main hand *
 ******************************/
void Client::setMainHand(MainHand hand)
{
	lock_guard<mutex> lock(dataLock);
	mainHand = hand;
}

/**********************************************
 * Client :: setIsDead                        *
 * Change whether the player is alive or dead *
 **********************************************/
void Client::setIsDead(Boolean isDead)
{
	lock_guard<mutex> lock(dataLock);
	this->isDead = isDead;
}

/******************************************************
 * Client :: setChatColors                            *
 * Change if the player wants colors sent in the chat *
 ******************************************************/
void Client::setChatColors(Boolean chatColors)
{
	lock_guard<mutex> lock(dataLock);
	this->chatColors = chatColors;
}

/******************************************************
 * Client :: setOnGround                              *
 * Change if the player is on the floor or in the air *
 ******************************************************/
void Client::setOnGround(Boolean isOnGround)
{
	lock_guard<mutex> lock(dataLock);
	onGround = isOnGround;
}

/****************************************************************
 * Client :: setChatMode                                        *
 * Change how the client wants to receive chat / command output *
 ****************************************************************/
void Client::setChatMode(ChatMode chatMode)
{
	lock_guard<mutex> lock(dataLock);
	this->chatMode = chatMode;
}

/****************************************************************************
 * Client :: setState                                                       *
 * Change whether the player is connecting, playing, or changing its status *
 ****************************************************************************/
void Client::setState(ServerState state)
{
	lock_guard<mutex> lock(dataLock);
	this->state = state;
}

/********************************
 * Client :: setGamemode        *
 * Change the player's gamemode *
 ********************************/
void Client::setGamemode(Gamemode gamemode)
{
	lock_guard<mutex> lock(dataLock);
	this->gamemode = gamemode;
}

/*****************************************
 * Client :: setDimension                *
 * Change the dimension the player is in *
 *****************************************/
void Client::setDimension(Dimension dimension)
{
	lock_guard<mutex> lock(dataLock);
	this->dimension = dimension;
}

/************************************************
 * Client :: setViewDistance                    *
 * Set how far the client wants to view terrain *
 ************************************************/
void Client::setViewDistance(Byte viewDistance)
{
	lock_guard<mutex> lock(dataLock);
	this->viewDistance = viewDistance;
}

/******************************************
 * Client :: setSkinParts                 *
 * Change the player's visible skin parts *
 ******************************************/
void Client::setSkinParts(DisplayedSkinParts skinParts)
{
	lock_guard<mutex> lock(dataLock);
	this->skinParts = skinParts;
}

/*************************************************************************
 * Client :: setAbilities                                                *
 * Change the client's capabilities (creative, vulnerable, flying, etc.) *
 *************************************************************************/
void Client::setAbilities(PlayerAbilities abilities)
{
	lock_guard<mutex> lock(dataLock);
	this->abilities = abilities;
}

/******************************************************************************
 * Client :: resetTicksSinceUpdate                                            *
 * Reset how many ticks have passed since a ping was received from the client *
 ******************************************************************************/
void Client::resetTicksSinceUpdate()
{
	lock_guard<mutex> lock(dataLock);
	ticksSinceUpdate = 0;
}

/**********************************************************************************
 * Client :: incrementTicksSinceUpdate                                            *
 * Increment how many ticks have passed since a ping was received from the client *
 **********************************************************************************/
Int Client::incrementTicksSinceUpdate(Int ticks)
{
	lock_guard<mutex> lock(dataLock);
	return ticksSinceUpdate += ticks;
}

/*******************************************************
 * Client :: setProtocolVersion                        *
 * Change the version of the client's network protocol *
 *******************************************************/
void Client::setProtocolVersion(Int protocol)
{
	lock_guard<mutex> lock(dataLock);
	protocolVersion = protocol;
}

/****************************************
 * Client :: setEntityID                *
 * Change the player's unique entity id *
 ****************************************/
void Client::setEntityID(Int EID)
{
	lock_guard<mutex> lock(dataLock);
	entityID = EID;
}

/************************************************
 * Client :: setSocket                          *
 * Change the socket the player is connected on *
 ************************************************/
void Client::setSocket(SOCKET socket)
{
	lock_guard<mutex> lock(dataLock);
	this->socket = socket;
}

/****************************************
 * Client :: setYaw                     *
 * Change the player camera's yaw angle *
 ****************************************/
void Client::setYaw(Float yaw)
{
	lock_guard<mutex> lock(dataLock);
	this->yaw = yaw;
}

/******************************************
 * Client :: setPitch                     *
 * Change the player camera's pitch angle *
 ******************************************/
void Client::setPitch(Float pitch)
{
	lock_guard<mutex> lock(dataLock);
	this->pitch = pitch;
}

/***************************************************************
 * Client :: resetUptime                                       *
 * Reset how many ticks have passed since the player connected *
 ***************************************************************/
void Client::resetUptime()
{
	lock_guard<mutex> lock(dataLock);
	uptime = 0;
}

/*******************************************************************
 * Client :: incrementUptime                                       *
 * Increment how many ticks have passed since the player connected *
 *******************************************************************/
Long Client::incrementUptime(Int ticks)
{
	lock_guard<mutex> lock(dataLock);
	return uptime += ticks;
}

/***********************************************
 * Client :: setUUID                           *
 * Change the unique id assigned to the player *
 ***********************************************/
void Client::setUUID(UUID uuid)
{
	lock_guard<mutex> lock(dataLock);
	this->uuid = uuid;
}

/*****************************************
 * Client :: set                         *
 * Change the player's absolute position *
 *****************************************/
void Client::setPosition(PositionF position)
{
	lock_guard<mutex> lock(dataLock);
	this->position = position;
}

/****************************
 * Client :: setName        *
 * Change the player's name *
 ****************************/
void Client::setName(String name)
{
	lock_guard<mutex> lock(dataLock);
	this->name = name;
}

/********************************************
 * Client :: setLocale                      *
 * Change the player's language (ie. en_US) *
 ********************************************/
void Client::setLocale(String locale)
{
	lock_guard<mutex> lock(dataLock);
	this->locale = locale;
}

/*******************************************
 * Client :: setBrand                      *
 * Change the client's brand (ie. vanilla) *
 *******************************************/
void Client::setBrand(String brand)
{
	lock_guard<mutex> lock(dataLock);
	this->brand = brand;
}
