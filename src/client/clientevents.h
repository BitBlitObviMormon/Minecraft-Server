#pragma once
#include "../server/serverstate.h"
#include "../data/datatypes.h"
#include "vld.h"

class Client;
class ClientEventArgs
{
public:
	Client* client;
};

class InvalidPacketEventArgs : public ClientEventArgs
{
public:
	Int packetID;
	Int dataLength;
	Byte* data;
};

class InvalidStateEventArgs : public ClientEventArgs
{
public:
	ServerState state;
};

class InvalidLengthEventArgs : public ClientEventArgs
{
public:
	Int length;	// The given length (That was invalid)
	String eventCause; // The event that raised this one ie. "pluginMessage"
	ClientEventArgs* e; // The causing event's data
};

class ClientDisconnectEventArgs : public ClientEventArgs {};

class HandShakeEventArgs : public ClientEventArgs
{
public:
	ServerState state;
	UShort serverPort;
	Int protocolVersion;
	String serverAddress;
};

class LegacyServerListPingEventArgs : public ClientEventArgs
{
public:
	UByte payload;
};

class TeleportConfirmEventArgs : public ClientEventArgs
{
public:
	Int teleportID;
};

class TabCompleteEventArgs : public ClientEventArgs
{
public:
	Boolean assumeCommand;
	Boolean hasPosition;
	Position lookedAtBlock;
	String text;
};

class ChatMessageEventArgs : public ClientEventArgs
{
public:
	String message;
};

enum class ClientStatusAction
{
	Respawn = 0,
	Stats = 1,
	Inventory = 2
};

class ClientStatusEventArgs : public ClientEventArgs
{
public:
	ClientStatusAction action;
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

enum class ChatMode
{
	Enabled = 0,
	CommandsOnly = 1,
	Hidden = 2
};

enum class MainHand : Boolean
{
	Left = 0,
	Right = 1
};

enum class Hand : Boolean
{
	Main = 0,
	Off = 1
};

class ClientSettingsEventArgs : public ClientEventArgs
{
public:
	Boolean chatColors;
	MainHand mainHand;
	Byte viewDistance;
	ChatMode chatMode;
	DisplayedSkinParts displayedSkinParts;
	String locale;	// ie. en_US
};

class ConfirmTransactionEventArgs : public ClientEventArgs
{
public:
	Boolean accepted;
	UByte windowID;
	Short actionNum;
};

enum class EnchantmentPos
{
	First = 0,
	Second = 1,
	Third = 2
};

class EnchantItemEventArgs : public ClientEventArgs
{
public:
	EnchantmentPos enchantment;	// Position of enchantment (First, Second, Third)
	UByte windowID;
};

/* The enums were created by combining two nibbles into a byte: */
/* The mode (Left Half), and the button (Right Half) */
/* See the network protocol under the server-bound Click Window event */
enum class ClickMode
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
	UByte windowID;
	ClickMode mode;
	Short slot;
	Short actionNumber;
	Slot clickedItem;
};

class CloseWindowEventArgs : public ClientEventArgs
{
public:
	UByte windowID;
};

class PluginMessageEventArgs : public ClientEventArgs
{
public:
	Byte* data;
	Int length;
	String channel;
};

enum class EntityInteractType
{
	Interact = 0,
	Attack = 1,
	InteractAtPosition = 2
};

class UseEntityEventArgs : public ClientEventArgs
{
public:
	Hand hand;
	EntityInteractType interactType;
	Int entityID;
	PositionF position;
};

class KeepAliveEventArgs : public ClientEventArgs
{
public:
	Int id;
};

class PlayerOnGroundEventArgs : public ClientEventArgs
{
public:
	Boolean onGround;
};

class PlayerPositionEventArgs : public PlayerOnGroundEventArgs
{
public:
	PositionF position;
};

class PlayerLookEventArgs : public PlayerOnGroundEventArgs
{
public:
	Float yaw;
	Float pitch;
};

class PlayerPositionAndLookEventArgs : public PlayerOnGroundEventArgs
{
public:
	PositionF position;
	Float yaw;
	Float pitch;
};

class VehicleMoveEventArgs : public ClientEventArgs
{
public:
	PositionF position;
	Float yaw;
	Float pitch;
};

class SteerBoatEventArgs : public ClientEventArgs
{
public:
	Boolean turnLeftPaddle;
	Boolean turnRightPaddle;
};

class PlayerAbilities
{
public:
	Boolean invulnerable;
	Boolean canFly;
	Boolean isFlying;
	Boolean isCreative;
	PlayerAbilities(Boolean invulnerable = false, Boolean canFly = false, Boolean isFlying = false, Boolean isCreative = false)
		: invulnerable(invulnerable), canFly(canFly), isFlying(isFlying), isCreative(isCreative) {}
	Byte getFlags() { return invulnerable << 3 || canFly << 2 || isFlying << 1 || isCreative; }
};

class PlayerAbilitiesEventArgs : public ClientEventArgs
{
public:
	PlayerAbilities abilities;
	Float flyingSpeed;
	Float walkingSpeed;
};

enum class PlayerDigStatus
{
	DigStart = 0,
	DigCancel = 1,
	DigFinish = 2,
	DropItemStack = 3,
	DropItem = 4,
	ShootOrEat = 5,
	SwapItem = 6
};

enum class Face
{
	Bottom = 0,
	Top = 1,
	North = 2,
	South = 3,
	West = 4,
	East = 5,
	Special = 255
};

/* Used for dropping items, shooting arrows, and swapping items in hand for some reason?? */
/* I might want to break up this event into several... */
class PlayerDiggingEventArgs : public ClientEventArgs
{
public:
	PlayerDigStatus status;
	Position location;
	Face face;
};

enum class EntityAction
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
	Byte jumpBoost;
	Int actionID;
	Int playerID;
};

class SteerVehicleFlags
{
	Boolean jump;
	Boolean unmount;
};

class SteerVehicleEventArgs : public ClientEventArgs
{
public:
	SteerVehicleFlags flags;
	Float sideways;
	Float forward;
};

enum class ResourcePackStatus
{
	loaded = 0,
	declined = 1,
	failed = 2,
	accepted = 3
};

class ResourcePackStatusEventArgs : public ClientEventArgs
{
public:
	ResourcePackStatus status;
};

class HeldItemChangeEventArgs : public ClientEventArgs
{
public:
	Short slot;
};

class CreativeInventoryActionEventArgs : public ClientEventArgs
{
public:
	Short slot;
	Slot clickedItem;
};

class UpdateSignEventArgs : public ClientEventArgs
{
public:
	Position location;
	String text[4];	// 4 lines, 1 for each line in the sign
};

class AnimationEventArgs : public ClientEventArgs
{
public:
	Hand hand;
};

class SpectateEventArgs : public ClientEventArgs
{
public:
	UUID id;
};

class PlayerBlockPlacementEventArgs : public ClientEventArgs
{
public:
	Hand hand;
	Face face;
	Position location;
	PositionF cursorPos;
};

class UseItemEventArgs : public ClientEventArgs
{
public:
	Hand hand;
};

class RequestEventArgs : public ClientEventArgs {};

class PingEventArgs : public ClientEventArgs
{
public:
	Long payload;
};

class LoginStartEventArgs : public ClientEventArgs
{
public:
	String name;
};

class EncryptionResponseEventArgs : public ClientEventArgs
{
public:
	Int sharedSecretLen;
	Int verifyTokenLen;
	Byte* sharedSecret;
	Byte* verifyToken;
};
