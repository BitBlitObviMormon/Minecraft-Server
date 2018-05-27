#pragma once

#include "data/datatypes.h"
#include "data/atomicset.h"
#include "client/clientevents.h"
#include "server/serverevents.h"
#include <utility>
#include <set>

#ifdef _WIN32 // WINDOWS
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <WinSock2.h>
	#include <WS2tcpip.h>
	#pragma comment(lib, "Ws2_32.lib")
#else		  // LINUX, POSIX, OSX
	#include <sys/socket.h>
	#include <arpa/net.h>
	#include <netdb.h>
	#include <unistd.h>
	typedef int SOCKET;
#endif

class Client
{
private:
	std::mutex dataLock;						   // This is locked whenever the client is accessed
	MainHand mainHand;							   // Which hand is the player's main hand
	Boolean isDead;								   // Whether or not the player is dead
	Boolean chatColors;							   // Whether to send chat colors or not
	Boolean onGround;							   // Whether the player is on the floor or in air
	ChatMode chatMode;							   // What kind of chat / command output the client receives
	ServerState state;							   // Whether the player is connecting, playing or changing its status
	Gamemode gamemode;							   // The gamemode the player is playing with
	Dimension dimension;						   // The dimension the player is in
	Byte viewDistance;							   // How far the player's client is set to view
	DisplayedSkinParts skinParts;				   // Which skin parts are visible on the player model
	PlayerAbilities abilities;					   // Whether the client is creative, vulnerable, flying, etc.
	Int ticksSinceUpdate;						   // How many ticks it's been since a ping
	Int protocolVersion;						   // The version of the network protocol the client is using
	Int entityID;								   // The player's unique entity id
	SOCKET socket;								   // The socket the player is currently connected on
	Float yaw;									   // The player camera's yaw angle
	Float pitch;								   // The player camera's pitch angle
	Long uptime;								   // How many ticks the player was connected
	UUID uuid;									   // The unique id assigned to the player
	PositionF position;							   // The player's absolute position
	String name;								   // The name of the player
	String locale;					 			   // The language the player is using (ie. en_US)
	String brand;					 			   // The type of client (usually vanilla unless modded)
public:
	AtomicSet< std::pair<Int, Int> > loadedChunks; // All of the chunks that the client currently has loaded (x, z)

	// Default constructor
	Client(SOCKET newClient = NULL);

	// Destructor
	~Client();

	// Getters
	const MainHand& getMainHand();			  // Which hand is the player's main hand
	const Boolean& getIsDead();				  // Whether or not the player is dead
	const Boolean& getChatColors();			  // Whether to send chat colors or not
	const Boolean& getOnGround();			  // Whether the player is on the floor or in air
	const ChatMode& getChatMode();			  // What kind of chat / command output the client receives
	const ServerState& getState();			  // Whether the player is connecting, playing or changing its status
	const Gamemode& getGamemode();			  // The gamemode the player is playing with
	const Dimension& getDimension();		  // The dimension the player is in
	const Byte& getViewDistance();			  // How far the player's client is set to view
	const DisplayedSkinParts& getSkinParts(); // Which skin parts are visible on the player model
	const PlayerAbilities& getAbilities();	  // Whether the client is creative, vulnerable, flying, etc.
	const Int& getTicksSinceUpdate();		  // How many ticks it's been since a ping
	const Int& getProtocolVersion();		  // The version of the network protocol the client is using
	const Int& getEntityID();				  // The player's unique entity id
	const SOCKET& getSocket();				  // The socket the player is currently connected on
	const Float& getYaw();					  // The player camera's yaw angle
	const Float& getPitch();				  // The player camera's pitch angle
	const Long& getUptime();				  // How many ticks the player was connected
	const UUID& getUUID();					  // The unique id assigned to the player
	const PositionF& getPosition();			  // The player's absolute position
	const String& getName();				  // The name of the player
	const String& getLocale();				  // The language the player is using (ie. en_US)
	const String& getBrand();				  // The type of client (usually vanilla unless modded)

	// Setters
	void setMainHand(MainHand hand);				 // Which hand is the player's main hand
	void setIsDead(Boolean isDead);					 // Whether or not the player is dead
	void setChatColors(Boolean chatColors);			 // Whether to send chat colors or not
	void setOnGround(Boolean isOnGround);			 // Whether the player is on the floor or in air
	void setChatMode(ChatMode chatMode);			 // What kind of chat / command output the client receives
	void setState(ServerState state);				 // Whether the player is connecting, playing or changing its status
	void setGamemode(Gamemode gamemode);			 // The gamemode the player is playing with
	void setDimension(Dimension dimension);			 // The dimension the player is in
	void setViewDistance(Byte viewDistance);		 // How far the player's client is set to view
	void setSkinParts(DisplayedSkinParts skinParts); // Which skin parts are visible on the player model
	void setAbilities(PlayerAbilities abilities);	 // Whether the client is creative, vulnerable, flying, etc.
	void resetTicksSinceUpdate();					 // How many ticks it's been since a ping
	Int incrementTicksSinceUpdate(Int ticks);		 // How many ticks it's been since a ping
	void setProtocolVersion(Int protocol);			 // The version of the network protocol the client is using
	void setEntityID(Int EID);						 // The player's unique entity id
	void setSocket(SOCKET socket);					 // The socket the player is currently connected on
	void setYaw(Float yaw);							 // The player camera's yaw angle
	void setPitch(Float pitch);						 // The player camera's pitch angle
	void resetUptime();								 // How many ticks the player was connected
	Long incrementUptime(Int ticks);				 // How many ticks the player was connected
	void setUUID(UUID uuid);						 // The unique id assigned to the player
	void setPosition(PositionF position);			 // The player's absolute position
	void setName(String name);						 // The name of the player
	void setLocale(String locale);					 // The language the player is using (ie. en_US)
	void setBrand(String brand);					 // The type of client (usually vanilla unless modded)

	// Boolean operators
	bool operator>(const Client& rhs)  const { return socket > rhs.socket; }
	bool operator<(const Client& rhs)  const { return socket < rhs.socket; }
	bool operator>=(const Client& rhs) const { return socket >= rhs.socket; }
	bool operator<=(const Client& rhs) const { return socket <= rhs.socket; }
	bool operator==(const Client& rhs) const { return socket == rhs.socket; }
	bool operator!=(const Client& rhs) const { return socket != rhs.socket; }
};

// Used to compare Client* by the socket
class ClientComparator
{
public:
	bool operator()(Client* lhs, Client* rhs) { return lhs->getSocket() > rhs->getSocket(); }
};