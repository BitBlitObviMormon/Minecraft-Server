#pragma once

enum class ClientHandshakePacket
{
	Handshake = 0x00,
	LegacyServerPing = 0xfe
};

enum class ClientStatusPacket
{
	Request = 0x00,
	Ping = 0x01
};

enum class ClientLoginPacket
{
	LoginStart = 0x00,
	EncryptionResponse = 0x01
};

enum class ClientPlayPacket
{
	TeleportConfirm = 0x00,
	TabComplete = 0x01,
	ChatMessage = 0x02,
	ClientStatus = 0x03,
	ClientSettings = 0x04,
	ConfirmTransaction = 0x05,
	EnchantItem = 0x06,
	ClickWindow = 0x07,
	CloseWindow = 0x08,
	PluginMessage = 0x09,
	UseEntity = 0x0a,
	KeepAlive = 0x0b,
	Player = 0x0c,
	PlayerPosition = 0x0d,
	PlayerPositionAndLook = 0x0e,
	PlayerLook = 0x0f,
	VehicleMove = 0x10,
	SteerBoat = 0x11,
	CraftRecipeRequest = 0x12,
	PlayerAbilities = 0x13,
	PlayerDigging = 0x14,
	EntityAction = 0x15,
	SteerVehicle = 0x16,
	CraftingBookData = 0x17,
	ResourcePackStatus = 0x18,
	AdvancementTab = 0x19,
	HeldItemChange = 0x1a,
	CreativeInventoryAction = 0x1b,
	UpdateSign = 0x1c,
	Animation = 0x1d,
	Spectate = 0x1e,
	PlayerBlockPlacement = 0x1f,
	UseItem = 0x20
};

enum class ServerHandshakePacket {};

enum class ServerStatusPacket
{
	Response = 0x00,
	Pong = 0x01
};

enum class ServerLoginPacket
{
	Disconnect = 0x00,
	EncryptionRequest = 0x01,
	LoginSuccess = 0x02,
	SetCompression = 0x03
};

enum class ServerPlayPacket
{
	SpawnObject = 0x00,
	SpawnExperienceOrb = 0x01,
	SpawnGlobalEntity = 0x02,
	SpawnMob = 0x03,
	SpawnPainting = 0x04,
	SpawnPlayer = 0x05,
	Animation = 0x06,
	Statistics = 0x07,
	BlockBreakAnimation = 0x08,
	UpdateBlockEntity = 0x09,
	BlockAction = 0x0a,
	BlockChange = 0x0b,
	BossBar = 0x0c,
	Difficulty = 0x0d,
	TabComplete = 0x0e,
	ChatMessage = 0x0f,
	MultiBlockChange = 0x10,
	ConfirmTransaction = 0x11,
	CloseWindow = 0x12,
	OpenWindow = 0x13,
	WindowItems = 0x14,
	WindowProperty = 0x15,
	SetSlot = 0x16,
	SetCooldown = 0x17,
	PluginMessage = 0x18,
	NamedSoundEffect = 0x19,
	Disconnect = 0x1a,
	EntityStatus = 0x1b,
	Explosion = 0x1c,
	UnloadChunk = 0x1d,
	ChangeGameState = 0x1e,
	KeepAlive = 0x1f,
	ChunkData = 0x20,
	Effect = 0x21,
	Particle = 0x22,
	JoinGame = 0x23,
	Map = 0x24,
	Entity = 0x25,
	EntityRelativeMove = 0x26,
	EntityLookAndRelativeMove = 0x27,
	EntityLook = 0x28,
	VehicleMove = 0x29,
	OpenSignEditor = 0x2a,
	CraftRecipeResponse = 0x2b,
	PlayerAbilities = 0x2c,
	CombatEvent = 0x2d,
	PlayerListItem = 0x2e,
	PlayerPositionAndLook = 0x2f,
	UseBed = 0x30,
	UnlockRecipes = 0x31,
	DestroyEntities = 0x32,
	RemoveEntityEffect = 0x33,
	ResourcePackSend = 0x34,
	Respawn = 0x35,
	EntityHeadLook = 0x36,
	SelectAdvancementTab = 0x37,
	WorldBorder = 0x38,
	Camera = 0x39,
	HeldItemChange = 0x3a,
	DisplayScoreboard = 0x3b,
	EntityMetadata = 0x3c,
	AttachEntity = 0x3d,
	EntityVelocity = 0x3e,
	EntityEquipment = 0x3f,
	SetExperience = 0x40,
	UpdateHealth = 0x41,
	ScoreboardObjective = 0x42,
	SetPassengers = 0x43,
	Teams = 0x44,
	UpdateScore = 0x45,
	SpawnPosition = 0x46,
	TimeUpdate = 0x47,
	Title = 0x48,
	SoundEffect = 0x49,
	PlayerListHeaderAndFooter = 0x4a,
	CollectItem = 0x4b,
	EntityTeleport = 0x4c,
	Advancements = 0x4d,
	EntityProperties = 0x4e,
	EntityEffect = 0x4f
};