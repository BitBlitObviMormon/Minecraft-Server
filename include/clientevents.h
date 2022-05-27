#pragma once

#include "uuid.h"
#include "datatypes.h"
#include "serverevents.h"
#include <memory>

namespace boost {
	class shared_mutex;
	typedef shared_mutex upgrade_mutex;
	template <typename T> class upgrade_lock;
	template <typename T> class upgrade_to_unique_lock;
}

class Client;
class ClientEventArgs
{
private:
	std::shared_ptr<Client> client;
	boost::upgrade_mutex* mutexPtr;

	/****************************************************
	 * DANGER ZONE                                      *
	 * This area allows the use of incomplete types,    *
	 * an action which might be illegal in c++17.       *
	 * These types are defined in the compiled library. *
	 ****************************************************/
	union ReadLock {
		std::unique_ptr< boost::upgrade_lock<boost::upgrade_mutex> > lock;
		ReadLock();
		ReadLock(ReadLock&& value);
		~ReadLock();
	} _readLock;
	union WriteLock {
		std::unique_ptr< boost::upgrade_to_unique_lock<boost::upgrade_mutex> > lock;
		WriteLock();
		WriteLock(WriteLock&& value);
		~WriteLock();
	} _writeLock;
	/* END OF DANGER ZONE */
public:
	// Read-only client data. DO NOT USE after unblockReadAccess!
	ClientEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr);
	ClientEventArgs(ClientEventArgs&& e);
	std::shared_ptr<Client> getWriteAccess();
	std::shared_ptr<const Client> getReadAccess();
	void unblockWriteAccess();
	void unblockReadAccess();
};

class InvalidPacketEventArgs : public ClientEventArgs
{
public:
	const Int packetID;
	const Int dataLength;
	const Byte* data;
	InvalidPacketEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Int packetID, const Int dataLength, const Byte* data)
		: ClientEventArgs(client, mutexPtr), packetID(packetID), dataLength(dataLength), data(data) {}
	InvalidPacketEventArgs(ClientEventArgs&& e, const Int packetID, const Int dataLength, const Byte* data)
		: ClientEventArgs(std::move(e)), packetID(packetID), dataLength(dataLength), data(data) {}
	InvalidPacketEventArgs(InvalidPacketEventArgs&& e) 
		: ClientEventArgs(std::move(e)), packetID(e.packetID), dataLength(e.dataLength), data(e.data) {}
};

class InvalidLengthEventArgs : public ClientEventArgs
{
public:
	const Int length;	// The given length (That was invalid)
	const Int actualLength; // The actual length of the packet
	const Int packetID;
	const Byte* data;
	InvalidLengthEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Int length, const Int actualLength, const Int packetID, const Byte* data)
		: ClientEventArgs(client, mutexPtr), length(length), actualLength(actualLength), packetID(packetID), data(data) {}
	InvalidLengthEventArgs(ClientEventArgs&& e, const Int length, const Int actualLength, const Int packetID, const Byte* data)
		: ClientEventArgs(std::move(e)), length(length), actualLength(actualLength), packetID(packetID), data(data) {}
	InvalidLengthEventArgs(InvalidLengthEventArgs&& e)
		: ClientEventArgs(std::move(e)), length(e.length), actualLength(e.actualLength), packetID(e.packetID), data(e.data) {}
};

class ClientDisconnectedEventArgs : public ClientEventArgs {
public:
	ClientDisconnectedEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr) : ClientEventArgs(client, mutexPtr) {}
	ClientDisconnectedEventArgs(ClientEventArgs&& e) : ClientEventArgs(std::move(e)) {}
	ClientDisconnectedEventArgs(ClientDisconnectedEventArgs&& e) : ClientEventArgs(std::move(e)) {}
};

class HandShakeEventArgs : public ClientEventArgs
{
public:
	const ServerState state;
	const UShort serverPort;
	const Int protocolVersion;
	const String serverAddress;
	HandShakeEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const ServerState state, const UShort serverPort, const Int protocolVersion, const String serverAddress)
		: ClientEventArgs(client, mutexPtr), state(state), serverPort(serverPort), protocolVersion(protocolVersion), serverAddress(serverAddress) {}
	HandShakeEventArgs(ClientEventArgs&& e, const ServerState state, const UShort serverPort, const Int protocolVersion, const String serverAddress)
		: ClientEventArgs(std::move(e)), state(state), serverPort(serverPort), protocolVersion(protocolVersion), serverAddress(serverAddress) {}
	HandShakeEventArgs(HandShakeEventArgs&& e)
		: ClientEventArgs(std::move(e)), state(e.state), serverPort(e.serverPort), protocolVersion(e.protocolVersion), serverAddress(std::move(serverAddress)) {}
};

class LegacyServerListPingEventArgs : public ClientEventArgs
{
public:
	const UByte payload;
	LegacyServerListPingEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const UByte payload)
		: ClientEventArgs(client, mutexPtr), payload(payload) {}
	LegacyServerListPingEventArgs(ClientEventArgs&& e, const UByte payload) : ClientEventArgs(std::move(e)), payload(payload) {}
	LegacyServerListPingEventArgs(LegacyServerListPingEventArgs&& e) : ClientEventArgs(std::move(e)), payload(e.payload) {}
};

class TeleportConfirmEventArgs : public ClientEventArgs
{
public:
	const Int teleportID;
	TeleportConfirmEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Int teleportID)
		: ClientEventArgs(client, mutexPtr), teleportID(teleportID) {}
	TeleportConfirmEventArgs(ClientEventArgs&& e, const Int teleportID) : ClientEventArgs(std::move(e)), teleportID(teleportID) {}
	TeleportConfirmEventArgs(TeleportConfirmEventArgs&& e) : ClientEventArgs(std::move(e)), teleportID(e.teleportID) {}
};

class TabCompleteEventArgs : public ClientEventArgs
{
public:
	const Boolean assumeCommand;
	const Boolean hasPosition;
	const Position lookedAtBlock;
	const String text;
	TabCompleteEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Boolean assumeCommand, const Boolean hasPosition, const String text, const Position lookedAtBlock = Position())
		: ClientEventArgs(client, mutexPtr), assumeCommand(assumeCommand), hasPosition(hasPosition), lookedAtBlock(lookedAtBlock), text(text) {}
	TabCompleteEventArgs(ClientEventArgs&& e, const Boolean assumeCommand, const Boolean hasPosition, const String text, const Position lookedAtBlock = Position())
		: ClientEventArgs(std::move(e)), assumeCommand(assumeCommand), hasPosition(hasPosition), lookedAtBlock(lookedAtBlock), text(text) {}
	TabCompleteEventArgs(TabCompleteEventArgs&& e)
		: ClientEventArgs(std::move(e)), assumeCommand(e.assumeCommand), hasPosition(e.hasPosition), lookedAtBlock(std::move(e.lookedAtBlock)), text(std::move(e.text)) {}
};

class ChatMessageEventArgs : public ClientEventArgs
{
public:
	const String message;
	ChatMessageEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const String message)
		: ClientEventArgs(client, mutexPtr), message(message) {}
	ChatMessageEventArgs(ClientEventArgs&& e, const String message) : ClientEventArgs(std::move(e)), message(message) {}
	ChatMessageEventArgs(ChatMessageEventArgs&& e) : ClientEventArgs(std::move(e)), message(std::move(message)) {}
};

enum struct ClientStatusAction : Byte
{
	Respawn = 0,
	Stats = 1,
	Inventory = 2
};

class ClientStatusEventArgs : public ClientEventArgs
{
public:
	const ClientStatusAction action;
	ClientStatusEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const ClientStatusAction action)
		: ClientEventArgs(client, mutexPtr), action(action) {}
	ClientStatusEventArgs(ClientEventArgs&& e, const ClientStatusAction action) : ClientEventArgs(std::move(e)), action(action) {}
	ClientStatusEventArgs(ClientStatusEventArgs&& e) : ClientEventArgs(std::move(e)), action(e.action) {}
};

class DisplayedSkinParts
{
public:
	Boolean cape;
	Boolean jacket;
	Boolean leftSleeve;
	Boolean rightSleeve;
	Boolean leftPants;
	Boolean rightPants;
	Boolean hat;
	Boolean unusedBit;	// May be used by mods

	/* Converts Byte to DisplayedSkinParts */
	DisplayedSkinParts(Byte byte = 0);
};

enum struct ChatMode : Byte
{
	Enabled = 0,
	CommandsOnly = 1,
	Hidden = 2
};

enum struct MainHand : Boolean
{
	LeftHand = 0,
	RightHand = 1
};

enum struct Hand : Boolean
{
	Main = 0,
	Off = 1
};

class ClientSettingsEventArgs : public ClientEventArgs
{
public:
	const Boolean chatColors;
	const MainHand mainHand;
	const Byte viewDistance;
	const ChatMode chatMode;
	const DisplayedSkinParts displayedSkinParts;
	const String locale;	// ie. en_US
	ClientSettingsEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Boolean chatColors, const MainHand mainHand, const Byte viewDistance, const ChatMode chatMode, const DisplayedSkinParts displayedSkinParts, const String locale)
		: ClientEventArgs(client, mutexPtr), chatColors(chatColors), mainHand(mainHand), viewDistance(viewDistance), chatMode(chatMode), displayedSkinParts(displayedSkinParts), locale(locale) {}
	ClientSettingsEventArgs(ClientEventArgs&& e, const Boolean chatColors, const MainHand mainHand, const Byte viewDistance, const ChatMode chatMode, const DisplayedSkinParts displayedSkinParts, const String locale)
		: ClientEventArgs(std::move(e)), chatColors(chatColors), mainHand(mainHand), viewDistance(viewDistance), chatMode(chatMode), displayedSkinParts(displayedSkinParts), locale(locale) {}
	ClientSettingsEventArgs(ClientSettingsEventArgs&& e)
		: ClientEventArgs(std::move(e)), chatColors(e.chatColors), mainHand(e.mainHand), viewDistance(e.viewDistance), chatMode(e.chatMode), displayedSkinParts(std::move(e.displayedSkinParts)), locale(std::move(e.locale)) {}
};

class ConfirmTransactionEventArgs : public ClientEventArgs
{
public:
	const Boolean accepted;
	const UByte windowID;
	const Short actionNum;
	ConfirmTransactionEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Boolean accepted, const UByte windowID, const Short actionNum)
		: ClientEventArgs(client, mutexPtr), accepted(accepted), windowID(windowID), actionNum(actionNum) {}
	ConfirmTransactionEventArgs(ClientEventArgs&& e, const Boolean accepted, const UByte windowID, const Short actionNum)
		: ClientEventArgs(std::move(e)), accepted(accepted), windowID(windowID), actionNum(actionNum) {}
	ConfirmTransactionEventArgs(ConfirmTransactionEventArgs&& e)
		: ClientEventArgs(std::move(e)), accepted(e.accepted), windowID(e.windowID), actionNum(e.actionNum) {}
};

enum struct EnchantmentPos : Byte
{
	First = 0,
	Second = 1,
	Third = 2
};

class EnchantItemEventArgs : public ClientEventArgs
{
public:
	const EnchantmentPos position;	// Position of enchantment (First, Second, Third)
	const UByte windowID;
	EnchantItemEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const EnchantmentPos position, const UByte windowID)
		: ClientEventArgs(client, mutexPtr), position(position), windowID(windowID) {}
	EnchantItemEventArgs(ClientEventArgs&& e, const EnchantmentPos position, const UByte windowID)
		: ClientEventArgs(std::move(e)), position(position), windowID(windowID) {}
	EnchantItemEventArgs(EnchantItemEventArgs&& e)
		: ClientEventArgs(std::move(e)), position(e.position), windowID(e.windowID) {}
};

/* The enum structs were created by combining two nibbles into a byte: */
/* The mode (Left Half), and the button (Right Half) */
/* See the network protocol under the server-bound Click Window event */
enum struct ClickMode : Byte
{
	LeftClick		= 0x00,
	RightClick		= 0x01,
	ShiftLeftClick	= 0x10,
	ShiftRightClick	= 0x11,
	NumKey1			= 0x20,
	NumKey2			= 0x21,
	NumKey3			= 0x22,
	NumKey4			= 0x23,
	NumKey5			= 0x24,
	NumKey6			= 0x25,
	NumKey7			= 0x26,
	NumKey8			= 0x27,
	NumKey9			= 0x28,
	MiddleClick		= 0x30,
	Drop			= 0x40,
	CtrlDrop		= 0x41,
	LeftDragBegin	= 0x50,
	LeftDrag		= 0x51,
	LeftDragEnd		= 0x52,
	RightDragBegin	= 0x54,
	RightDrag		= 0x55,
	RightDragEnd	= 0x56,
	MiddleDragBegin	= 0x58,
	MiddleDrag		= 0x59,
	MiddleDragEnd	= 0x5a,
	DoubleClick		= 0x60
};

class ClickWindowEventArgs : public ClientEventArgs
{
public:
	const UByte windowID;
	const ClickMode mode;
	const Short slot;
	const Short actionNumber;
	const Slot clickedItem;
	
	ClickWindowEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const UByte windowID, const ClickMode mode, const Short slot, const Short actionNumber, const Slot clickedItem)
		: ClientEventArgs(client, mutexPtr), windowID(windowID), mode(mode), slot(slot), actionNumber(actionNumber), clickedItem(clickedItem) {}
	ClickWindowEventArgs(ClientEventArgs&& e, const UByte windowID, const ClickMode mode, const Short slot, const Short actionNumber, const Slot clickedItem)
		: ClientEventArgs(std::move(e)), windowID(windowID), mode(mode), slot(slot), actionNumber(actionNumber), clickedItem(clickedItem) {}
	ClickWindowEventArgs(ClickWindowEventArgs&& e)
		: ClientEventArgs(std::move(e)), windowID(e.windowID), mode(e.mode), slot(e.slot), actionNumber(e.actionNumber), clickedItem(std::move(e.clickedItem)) {}
};

class CloseWindowEventArgs : public ClientEventArgs
{
public:
	const UByte windowID;
	CloseWindowEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const UByte windowID)
		: ClientEventArgs(client, mutexPtr), windowID(windowID) {}
	CloseWindowEventArgs(ClientEventArgs&& e, const UByte windowID) : ClientEventArgs(std::move(e)), windowID(windowID) {}
	CloseWindowEventArgs(CloseWindowEventArgs&& e)
		: ClientEventArgs(std::move(e)), windowID(e.windowID) {}
};

class PluginMessageEventArgs : public ClientEventArgs
{
public:
	const Int length;
	const String channel;
	const Byte* data;
	PluginMessageEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Int length, const String channel, const Byte* data)
		: ClientEventArgs(client, mutexPtr), length(length), channel(channel), data(data) {}
	PluginMessageEventArgs(ClientEventArgs&& e, const Int length, const String channel, const Byte* data)
		: ClientEventArgs(std::move(e)), length(length), channel(channel), data(data) {}
	PluginMessageEventArgs(PluginMessageEventArgs&& e)
		: ClientEventArgs(std::move(e)), length(e.length), channel(std::move(e.channel)), data(e.data) {}
};

enum struct EntityInteractType : Byte
{
	Interact = 0,
	Attack = 1,
	InteractAtPosition = 2
};

class UseEntityEventArgs : public ClientEventArgs
{
public:
	const Hand hand;
	const EntityInteractType interactType;
	const Int entityID;
	const PositionF position;
	UseEntityEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Hand hand, const EntityInteractType interactType, const Int entityID, const PositionF position)
		: ClientEventArgs(client, mutexPtr), hand(hand), interactType(interactType), entityID(entityID), position(position) {}
	UseEntityEventArgs(ClientEventArgs&& e, const Hand hand, const EntityInteractType interactType, const Int entityID, const PositionF position)
		: ClientEventArgs(std::move(e)), hand(hand), interactType(interactType), entityID(entityID), position(position) {}
	UseEntityEventArgs(UseEntityEventArgs&& e)
		: ClientEventArgs(std::move(e)), hand(e.hand), interactType(e.interactType), entityID(e.entityID), position(std::move(e.position)) {}
};

class KeepAliveEventArgs : public ClientEventArgs
{
public:
	const Long payload;
	KeepAliveEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Long payload)
		: ClientEventArgs(client, mutexPtr), payload(payload) {}
	KeepAliveEventArgs(ClientEventArgs&& e, const Long payload) : ClientEventArgs(std::move(e)), payload(payload) {}
	KeepAliveEventArgs(KeepAliveEventArgs&& e) : ClientEventArgs(std::move(e)), payload(e.payload) {}
};

class PlayerPositionAndLookEventArgs : public ClientEventArgs
{
public:
	const PositionF position;
	const Float yaw;
	const Float pitch;
	const Boolean onGround;
	PlayerPositionAndLookEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Boolean onGround, const PositionF position, const Float yaw, const Float pitch)
		: ClientEventArgs(client, mutexPtr), position(position), yaw(yaw), pitch(pitch), onGround(onGround) {}
	PlayerPositionAndLookEventArgs(ClientEventArgs&& e, const Boolean onGround, const PositionF position, const Float yaw, const Float pitch)
		: ClientEventArgs(std::move(e)), position(position), yaw(yaw), pitch(pitch), onGround(onGround) {}
	PlayerPositionAndLookEventArgs(PlayerPositionAndLookEventArgs&& e)
		: ClientEventArgs(std::move(e)), position(std::move(e.position)), yaw(e.yaw), pitch(e.pitch), onGround(e.onGround) {}
};

class PlayerMovedEventArgs : public PlayerPositionAndLookEventArgs {
public:
	const Boolean playerChangedPosition;
	const Boolean playerTurnedHead;
	PlayerMovedEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Boolean onGround)
		: PlayerPositionAndLookEventArgs(client, mutexPtr, onGround, PositionF(), 0.0, 0.0), playerChangedPosition(false), playerTurnedHead(false) {}
	PlayerMovedEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Boolean onGround, const Float yaw, const Float pitch)
		: PlayerPositionAndLookEventArgs(client, mutexPtr, onGround, PositionF(), yaw, pitch), playerChangedPosition(false), playerTurnedHead(true) {}
	PlayerMovedEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Boolean onGround, const PositionF position)
		: PlayerPositionAndLookEventArgs(client, mutexPtr, onGround, position, 0.0, 0.0), playerChangedPosition(true), playerTurnedHead(false) {}
	PlayerMovedEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Boolean onGround, const PositionF position, const Float yaw, const Float pitch)
		: PlayerPositionAndLookEventArgs(client, mutexPtr, onGround, position, yaw, pitch), playerChangedPosition(true), playerTurnedHead(true) {}
	PlayerMovedEventArgs(ClientEventArgs&& e, const Boolean onGround)
		: PlayerPositionAndLookEventArgs(std::move(e), onGround, PositionF(), 0.0, 0.0), playerChangedPosition(false), playerTurnedHead(false) {}
	PlayerMovedEventArgs(ClientEventArgs&& e, const Boolean onGround, const Float yaw, const Float pitch)
		: PlayerPositionAndLookEventArgs(std::move(e), onGround, PositionF(), yaw, pitch), playerChangedPosition(false), playerTurnedHead(true) {}
	PlayerMovedEventArgs(ClientEventArgs&& e, const Boolean onGround, const PositionF position)
		: PlayerPositionAndLookEventArgs(std::move(e), onGround, position, 0.0, 0.0), playerChangedPosition(true), playerTurnedHead(false) {}
	PlayerMovedEventArgs(ClientEventArgs&& e, const Boolean onGround, const PositionF position, const Float yaw, const Float pitch)
		: PlayerPositionAndLookEventArgs(std::move(e), onGround, position, yaw, pitch), playerChangedPosition(true), playerTurnedHead(true) {}
	PlayerMovedEventArgs(PlayerMovedEventArgs&& e)
		: PlayerPositionAndLookEventArgs(std::move(e)), playerChangedPosition(e.playerChangedPosition), playerTurnedHead(e.playerTurnedHead) {}
};

class VehicleMovedEventArgs : public ClientEventArgs
{
public:
	const PositionF position;
	const Float yaw;
	const Float pitch;
	VehicleMovedEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const PositionF position, const Float yaw, const Float pitch)
		: ClientEventArgs(client, mutexPtr), position(position), yaw(yaw), pitch(pitch) {}
	VehicleMovedEventArgs(ClientEventArgs&& e, const PositionF position, const Float yaw, const Float pitch)
		: ClientEventArgs(std::move(e)), position(position), yaw(yaw), pitch(pitch) {}
	VehicleMovedEventArgs(VehicleMovedEventArgs&& e)
		: ClientEventArgs(std::move(e)), position(std::move(e.position)), yaw(e.yaw), pitch(e.pitch) {}
};

class SteerBoatEventArgs : public ClientEventArgs
{
public:
	const Boolean turnLeftPaddle;
	const Boolean turnRightPaddle;
	SteerBoatEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Boolean turnLeftPaddle, const Boolean turnRightPaddle)
		: ClientEventArgs(client, mutexPtr), turnLeftPaddle(turnLeftPaddle), turnRightPaddle(turnRightPaddle) {}
	SteerBoatEventArgs(ClientEventArgs&& e, const Boolean turnLeftPaddle, const Boolean turnRightPaddle)
		: ClientEventArgs(std::move(e)), turnLeftPaddle(turnLeftPaddle), turnRightPaddle(turnRightPaddle) {}
	SteerBoatEventArgs(SteerBoatEventArgs&& e)
		: ClientEventArgs(std::move(e)), turnLeftPaddle(e.turnLeftPaddle), turnRightPaddle(e.turnRightPaddle) {}
};

class PlayerAbilities
{
public:
	Boolean invulnerable;
	Boolean canFly;
	Boolean isFlying;
	Boolean isCreative;
	PlayerAbilities(const Boolean invulnerable = false, const Boolean canFly = false, const Boolean isFlying = false, const Boolean isCreative = false)
		: invulnerable(invulnerable), canFly(canFly), isFlying(isFlying), isCreative(isCreative) {}
	PlayerAbilities(const Byte data)
		: invulnerable(data >> 3 & 1), canFly(data >> 2 & 1), isFlying(data >> 1 & 1), isCreative(data & 1) {}
	PlayerAbilities(const PlayerAbilities& value)
		: invulnerable(value.invulnerable), canFly(value.canFly), isFlying(value.isFlying), isCreative(value.isCreative) {}
	const Byte getFlags() const { return invulnerable << 3 || canFly << 2 || isFlying << 1 || isCreative; }
	PlayerAbilities& operator=(const PlayerAbilities& rhs) {
		this->invulnerable = rhs.invulnerable;
		this->canFly = rhs.canFly;
		this->isFlying = rhs.isFlying;
		this->isCreative = rhs.isCreative;
		return *this;
	}
};

class PlayerAbilitiesEventArgs : public ClientEventArgs
{
public:
	const PlayerAbilities abilities;
	const Float flyingSpeed;
	const Float walkingSpeed;
	PlayerAbilitiesEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const PlayerAbilities abilities, const Float flyingSpeed, const Float walkingSpeed)
		: ClientEventArgs(client, mutexPtr), abilities(abilities), flyingSpeed(flyingSpeed), walkingSpeed(walkingSpeed) {}
	PlayerAbilitiesEventArgs(ClientEventArgs&& e, const PlayerAbilities abilities, const Float flyingSpeed, const Float walkingSpeed)
		: ClientEventArgs(std::move(e)), abilities(abilities), flyingSpeed(flyingSpeed), walkingSpeed(walkingSpeed) {}
	PlayerAbilitiesEventArgs(PlayerAbilitiesEventArgs&& e)
		: ClientEventArgs(std::move(e)), abilities(std::move(e.abilities)), flyingSpeed(e.flyingSpeed), walkingSpeed(e.walkingSpeed) {}
};

enum struct PlayerDigStatus : Byte
{
	DigStart = 0,
	DigCancel = 1,
	DigFinish = 2,
	DropItemStack = 3,
	DropItem = 4,
	ShootOrEat = 5,
	SwapItem = 6
};

enum struct Face : Byte
{
	Special = -1,
	Bottom = 0,
	Top = 1,
	North = 2,
	South = 3,
	West = 4,
	East = 5
};

/* Used for dropping items, shooting arrows, and swapping items in hand for some reason?? */
/* I might want to break up this event into several... */
class PlayerDiggingEventArgs : public ClientEventArgs
{
public:
	const PlayerDigStatus status;
	const Position location;
	const Face face;
	PlayerDiggingEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const PlayerDigStatus status, const Position location = Position(), const Face face = Face::Bottom)
		: ClientEventArgs(client, mutexPtr), status(status), location(location), face(face) {}
	PlayerDiggingEventArgs(ClientEventArgs&& e, const PlayerDigStatus status, const Position location = Position(), const Face face = Face::Bottom)
		: ClientEventArgs(std::move(e)), status(status), location(location), face(face) {}
	PlayerDiggingEventArgs(PlayerDiggingEventArgs&& e)
		: ClientEventArgs(std::move(e)), status(e.status), location(std::move(e.location)), face(e.face) {}
};

enum struct EntityAction : Byte
{
	SneakStart = 0,
	SneakStop = 1,
	LeaveBed = 2,
	SprintStart = 3,
	SprintStop = 4,
	HorseJumpStart = 5,
	HorseJumpStop = 6,
	HorseInventory = 7,
	ElytraFlyStart = 8
};

/* Used for various actions, may want to split up into several events? */
class EntityActionEventArgs : public ClientEventArgs
{
public:
	const Byte jumpBoost;
	const Int actionID;
	const Int playerID;
	EntityActionEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Byte jumpBoost, const Int actionID, const Int playerID)
		: ClientEventArgs(client, mutexPtr), jumpBoost(jumpBoost), actionID(actionID), playerID(playerID) {}
	EntityActionEventArgs(ClientEventArgs&& e, const Byte jumpBoost, const Int actionID, const Int playerID)
		: ClientEventArgs(std::move(e)), jumpBoost(jumpBoost), actionID(actionID), playerID(playerID) {}
	EntityActionEventArgs(EntityActionEventArgs&& e)
		: ClientEventArgs(std::move(e)), jumpBoost(e.jumpBoost), actionID(e.actionID), playerID(e.playerID) {}
};

class SteerVehicleFlags
{
public:
	Boolean jump;
	Boolean unmount;
	SteerVehicleFlags(const Boolean jump = false, const Boolean unmount = false) : jump(jump), unmount(unmount) {}
	SteerVehicleFlags(const SteerVehicleFlags& value) : SteerVehicleFlags(value.jump, value.unmount) {}
};

class SteerVehicleEventArgs : public ClientEventArgs
{
public:
	const SteerVehicleFlags flags;
	const Float sideways;
	const Float forward;
	SteerVehicleEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const SteerVehicleFlags flags, const Float sideways, const Float forward)
		: ClientEventArgs(client, mutexPtr), flags(flags), sideways(sideways), forward(forward) {}
	SteerVehicleEventArgs(ClientEventArgs&& e, const SteerVehicleFlags flags, const Float sideways, const Float forward)
		: ClientEventArgs(std::move(e)), flags(flags), sideways(sideways), forward(forward) {}
	SteerVehicleEventArgs(SteerVehicleEventArgs&& e)
		: ClientEventArgs(std::move(e)), flags(std::move(e.flags)), sideways(e.sideways), forward(e.forward) {}
};

enum struct ResourcePackStatus : Byte
{
	loaded = 0,
	declined = 1,
	failed = 2,
	accepted = 3
};

class ResourcePackStatusEventArgs : public ClientEventArgs
{
public:
	const ResourcePackStatus status;
	ResourcePackStatusEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const ResourcePackStatus status)
		: ClientEventArgs(client, mutexPtr), status(status) {}
	ResourcePackStatusEventArgs(ClientEventArgs&& e, ResourcePackStatus status)
		: ClientEventArgs(std::move(e)), status(status) {}
	ResourcePackStatusEventArgs(ResourcePackStatusEventArgs&& e) : ClientEventArgs(std::move(e)), status(e.status) {}
};

class HeldItemChangeEventArgs : public ClientEventArgs
{
public:
	const Short slot;
	HeldItemChangeEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Short slot)
		: ClientEventArgs(client, mutexPtr), slot(slot) {}
	HeldItemChangeEventArgs(ClientEventArgs&& e, const Short slot) : ClientEventArgs(std::move(e)), slot(slot) {}
	HeldItemChangeEventArgs(HeldItemChangeEventArgs&& e) : ClientEventArgs(std::move(e)), slot(e.slot) {}
};

class CreativeInventoryActionEventArgs : public ClientEventArgs
{
public:
	const Short slot;
	const Slot clickedItem;
	CreativeInventoryActionEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Short slot, const Slot clickedItem)
		: ClientEventArgs(client, mutexPtr), slot(slot), clickedItem(clickedItem) {}
	CreativeInventoryActionEventArgs(ClientEventArgs&& e, const Short slot, const Slot clickedItem)
		: ClientEventArgs(std::move(e)), slot(slot), clickedItem(clickedItem) {}
	CreativeInventoryActionEventArgs(CreativeInventoryActionEventArgs&& e)
		: ClientEventArgs(std::move(e)), slot(e.slot), clickedItem(std::move(e.clickedItem)) {}
};

class UpdateSignEventArgs : public ClientEventArgs
{
public:
	const Position location;
	const String text[4];	// 4 lines, 1 for each line in the sign
	UpdateSignEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Position location, const String line1, const String line2, const String line3, const String line4)
		: ClientEventArgs(client, mutexPtr), location(location), text{ line1, line2, line3, line4 } {}
	UpdateSignEventArgs(ClientEventArgs&& e, const Position location, const String line1, const String line2, const String line3, const String line4)
		: ClientEventArgs(std::move(e)), location(location), text{ line1, line2, line3, line4 } {}
	UpdateSignEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Position location, const String* text)
		: ClientEventArgs(client, mutexPtr), location(location), text{ text[0], text[1], text[2], text[3] } {}
	UpdateSignEventArgs(ClientEventArgs&& e, const Position location, const String* text)
		: ClientEventArgs(std::move(e)), location(location), text{ text[0], text[1], text[2], text[3] } {}
	UpdateSignEventArgs(UpdateSignEventArgs&& e)
		: ClientEventArgs(std::move(e)), location(std::move(e.location)), text{ std::move(e.text[0]), std::move(e.text[1]), std::move(e.text[2]), std::move(e.text[3]) } {}
};

class HandAnimationEventArgs : public ClientEventArgs
{
public:
	const Hand hand;
	HandAnimationEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Hand hand)
		: ClientEventArgs(client, mutexPtr), hand(hand) {}
	HandAnimationEventArgs(ClientEventArgs&& e, const Hand hand) : ClientEventArgs(std::move(e)), hand(hand) {}
	HandAnimationEventArgs(HandAnimationEventArgs&& e)
		: ClientEventArgs(std::move(e)), hand(e.hand) {}
};

class SpectateEventArgs : public ClientEventArgs
{
public:
	const UUID id;
	SpectateEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const UUID id)
		: ClientEventArgs(client, mutexPtr), id(id) {}
	SpectateEventArgs(ClientEventArgs&& e, const UUID id) : ClientEventArgs(std::move(e)), id(id) {}
	SpectateEventArgs(SpectateEventArgs&& e)
		: ClientEventArgs(std::move(e)) {}
};

class PlayerBlockPlacementEventArgs : public ClientEventArgs
{
public:
	const Hand hand;
	const Face face;
	const Position location;
	const PositionF cursorPos;
	PlayerBlockPlacementEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Hand hand, const Face face, const Position location, const PositionF cursor_position)
		: ClientEventArgs(client, mutexPtr), hand(hand), face(face), location(location), cursorPos(cursor_position) {}
	PlayerBlockPlacementEventArgs(ClientEventArgs&& e, const Hand hand, const Face face, const Position location, const PositionF cursor_position)
		: ClientEventArgs(std::move(e)), hand(hand), face(face), location(location), cursorPos(cursor_position) {}
	PlayerBlockPlacementEventArgs(PlayerBlockPlacementEventArgs&& e)
		: ClientEventArgs(std::move(e)), hand(e.hand), face(e.face), location(std::move(e.location)), cursorPos(std::move(e.cursorPos)) {}
};

class UseItemEventArgs : public ClientEventArgs
{
public:
	const Hand hand;
	UseItemEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Hand hand)
		: ClientEventArgs(client, mutexPtr), hand(hand) {}
	UseItemEventArgs(ClientEventArgs&& e, const Hand hand) : ClientEventArgs(std::move(e)), hand(hand) {}
	UseItemEventArgs(UseItemEventArgs&& e) : ClientEventArgs(std::move(e)), hand(e.hand) {}
};

class RequestEventArgs : public ClientEventArgs 
{
public:
	RequestEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr) : ClientEventArgs(client, mutexPtr) {}
	RequestEventArgs(ClientEventArgs&& e) : ClientEventArgs(std::move(e)) {}
	RequestEventArgs(RequestEventArgs&& e) : ClientEventArgs(std::move(e)) {}
};

class PingEventArgs : public ClientEventArgs
{
public:
	const Long payload;
	PingEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Long payload)
		: ClientEventArgs(client, mutexPtr), payload(payload) {}
	PingEventArgs(ClientEventArgs&& e, const Long payload) : ClientEventArgs(std::move(e)), payload(payload) {}
	PingEventArgs(PingEventArgs&& e) : ClientEventArgs(std::move(e)), payload(e.payload) {}
};

class LoginStartEventArgs : public ClientEventArgs
{
public:
	const String name;
	LoginStartEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const String name)
		: ClientEventArgs(client, mutexPtr), name(name) {}
	LoginStartEventArgs(ClientEventArgs&& e, const String name) : ClientEventArgs(std::move(e)), name(name) {}
	LoginStartEventArgs(LoginStartEventArgs&& e) : ClientEventArgs(std::move(e)), name(std::move(e.name)) {}
};

class EncryptionResponseEventArgs : public ClientEventArgs
{
public:
	const Int sharedSecretLen;
	const Int verifyTokenLen;
	const Byte* sharedSecret;
	const Byte* verifyToken;
	EncryptionResponseEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const Byte* sharedSecret, const Int sharedSecretLen, const Byte* verifyToken, const Int verifyTokenLen)
		: ClientEventArgs(client, mutexPtr), sharedSecret(sharedSecret), verifyToken(verifyToken), sharedSecretLen(sharedSecretLen), verifyTokenLen(verifyTokenLen) {}
	EncryptionResponseEventArgs(ClientEventArgs&& e, const Byte* sharedSecret, const Int sharedSecretLen, const Byte* verifyToken, const Int verifyTokenLen)
		: ClientEventArgs(std::move(e)), sharedSecret(sharedSecret), verifyToken(verifyToken), sharedSecretLen(sharedSecretLen), verifyTokenLen(verifyTokenLen) {}
	EncryptionResponseEventArgs(EncryptionResponseEventArgs&& e)
		: EncryptionResponseEventArgs(std::move(e), e.sharedSecret, e.sharedSecretLen, e.verifyToken, e.verifyTokenLen) {}
};

class GetChunkEventArgs : public ClientEventArgs
{
public:
	const PositionL pos;
	GetChunkEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const PositionL pos)
		: ClientEventArgs(client, mutexPtr), pos(pos) {}
	GetChunkEventArgs(ClientEventArgs&& e, const PositionL pos) : ClientEventArgs(std::move(e)), pos(pos) {}
	GetChunkEventArgs(GetChunkEventArgs&& e) : ClientEventArgs(std::move(e)), pos(std::move(e.pos)) {}
};

class GetBiomeEventArgs : public ClientEventArgs
{
public:
	const PositionL pos;
	GetBiomeEventArgs(std::shared_ptr<Client> client, boost::upgrade_mutex* mutexPtr, const PositionL pos)
		: ClientEventArgs(client, mutexPtr), pos(pos) {}
	GetBiomeEventArgs(ClientEventArgs&& e, const PositionL pos) : ClientEventArgs(std::move(e)), pos(pos) {}
	GetBiomeEventArgs(GetBiomeEventArgs&& e) : ClientEventArgs(std::move(e)), pos(std::move(e.pos)) {}
};
