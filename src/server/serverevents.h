#pragma once
#include <vector>
#include "../server/serverstate.h"
#include "../data/datatypes.h"
#include "vld.h"

enum class Direction
{
	South = 0,
	West = 1,
	North = 2,
	East = 3
};

enum class Animation
{
	SwingMainArm = 0,
	TakeDamage = 1,
	LeaveBed = 2,
	SwingOffHand = 3,
	CriticalEffect = 4,
	MagicCriticalEffect = 5
};

class Statistic
{
	String statName;
	Int value;
};

enum BossBarAction
{
	Add = 0,
	Remove = 1,
	UpdateHealth = 2,
	UpdateTitle = 3,
	UpdateStyle = 4,
	UpdateFlags = 5
};

enum class BossBarColor
{
	Pink = 0,
	Blue = 1,
	Red = 2,
	Green = 3,
	Yellow = 4,
	Purple = 5,
	White = 6
};

enum class BossBarNumDividers
{
	None = 0,
	Six = 1,
	Ten = 2,
	Twelve = 3,
	Twenty = 4
};

class BossBar
{
public:
	Byte flags;
	BossBarColor color;
	BossBarNumDividers numDividers;
	Float health;
	UUID uuid;
	String title;
};

enum class Difficulty
{
	Peaceful = 0,
	Easy = 1,
	Normal = 2,
	Hard = 3
};

enum class ChatMessageType
{
	Chat = 0,
	System = 1,
	GameInfo = 2
};

class Block
{
	Position pos;
	Int id;
	Int metadata;
};

class WindowType
{
private:
	String string;
public:
	static const WindowType Container;
	static const WindowType Chest;
	static const WindowType CraftingTable;
	static const WindowType Furnace;
	static const WindowType Dispenser;
	static const WindowType EnchantmentTable;
	static const WindowType BrewingStand;
	static const WindowType Villager;
	static const WindowType Beacon;
	static const WindowType Anvil;
	static const WindowType Hopper;
	static const WindowType Dropper;
	static const WindowType ShulkerBox;
	static const WindowType Animal;
	WindowType(const char* string);
	WindowType(const String string);
	WindowType(const WindowType& type);
	const Boolean operator==(WindowType second) const;
	const Boolean operator!=(WindowType second) const;
	const String str() const;
};

class Window
{
	Byte windowID;
	Byte numSlots;

	String title;
};

// TODO: Verify that these are correct
enum class SoundCategory
{
	Master = 0,
	Music = 1,
	Records = 2,
	Weather = 3,
	Blocks = 4,
	Hostile = 5,
	Neutral = 6,
	Players = 7,
	Ambient = 8,
	Voice = 9
};

enum class GameStateReason
{
	InvalidBed = 0,
	RainEnd = 1,
	RainBegin = 2,
	ChangeGameMode = 3,
	ExitEnd = 4,
	DemoMessage = 5,
	ArrowHitPlayer = 6,
	FadeValue = 7,
	FadeTime = 8,
	ElderGuardianAppearance = 10
};

enum class EffectID
{
	/* SOUNDS */
	DispenserDispense = 1000,
	DispenserFail = 1001,
	DispenserShoot = 1002,
	EnderEyeLaunch = 1003,
	FireworkShot = 1004,
	IronDoorOpen = 1005,
	WoodDoorOpen = 1006,
	WoodTrapdoorOpen = 1007,
	FenceGateOpen = 1008,
	FireExtinguish = 1009,
	PlayRecord = 1010,
	IronDoorClose = 1011,
	WoodDoorClose = 1012,
	WoodTrapdoorClose = 1013,
	FenceGateClose = 1014,
	GhastWarn = 1015,
	GhastShoot = 1016,
	EnderDragonShoot = 1017,
	BlazeShoot = 1018,
	ZombieAttackWoodDoor = 1019,
	ZombieAttackIronDoor = 1020,
	ZombieBreakWoodDoor = 1021,
	WitherBreakBlock = 1022,
	WitherSpawn = 1023,
	WitherShoot = 1024,
	BatTakeoff = 1025,
	ZombieInfect = 1026,
	ZombieVillagerRevert = 1027,
	EnderDragonDie = 1028,
	AnvilDestroy = 1029,
	AnvilUse = 1030,
	AnvilLand = 1031,
	PortalTravel = 1032,
	ChorusFlowerGrow = 1033,
	ChorusFlowerDie = 1034,
	BrewingStandBrew = 1035,
	IronTrapdoorOpen = 1036,
	IronTrapdoorClose = 1037,

	/* PARTICLES */
	FireSmoke = 2000,
	BlockBreak = 2001,
	SplashPotion1 = 2002,
	EyeOfEnderBreak = 2003,
	MobSpawn = 2004,
	BonemealParticles = 2005,
	DragonBreath = 2006,
	SplashPotion2 = 2007,
	EndGatewaySpawn = 3000,
	EnderDragonGrowl = 3001
};

enum class ParticleID
{
	Explode = 0,
	LargeExplosion = 1,
	HugeExplosion = 2,
	FireworksSpark = 3,
	Bubble = 4,
	Splash = 5,
	Wake = 6,
	Suspend = 7,
	DepthSuspend = 8,
	Critical = 9,
	MagicCritical = 10,
	Smoke = 11,
	LargeSmoke = 12,
	Spell = 13,
	InstantSpell = 14,
	MobSpell = 15,
	MobSpellAmbient = 16,
	WitchMagic = 17,
	DripWater = 18,
	DripLava = 19,
	AngryVillager = 20,
	HappyVillager = 21,
	TownAura = 22,
	Note = 23,
	Portal = 24,
	EnchantmentTable = 25,
	Flame = 26,
	Lava = 27,
	Footstep = 28,
	Cloud = 29,
	RedDust = 30,
	SnowballPoof = 31,
	SnowShovel = 32,
	Slime = 33,
	Heart = 34,
	Barrier = 35,
	IconCrack = 36,
	BlockCrack = 37,
	BlockDust = 38,
	Droplet = 39,
	Take = 40,
	MobAppearance = 41,
	DragonBreath = 42,
	EndRod = 43,
	DamageIndicator = 44,
	SweepAttack = 45,
	FallingDust = 46
};

class Particle
{
	Boolean longDistance;
	ParticleID id;
	Float particleData;
	PositionF pos;
	PositionF offset;
};

enum class Gamemode
{
	Survival = 0,
	Creative = 1,
	Adventure = 2,
	Spectator = 3,

	/* Hardcore gamemodes (Literally the gamemodes above +8) */
	HardcoreSurvival = 8,
	HardcoreCreative = 9,
	HardcoreAdventure = 10,
	HardcoreSpectator = 11
};

enum class Dimension
{
	Nether = -1,
	Overworld = 0,
	End = 1
};

class LevelType
{
private:
	String string;
public:
	static const LevelType Default;
	static const LevelType SuperFlat;
	static const LevelType LargeBiomes;
	static const LevelType Amplified;
	static const LevelType Default_1_1;
	LevelType(const char* string);
	LevelType(const String string);
	LevelType(const LevelType& type);
	const Boolean operator==(LevelType second) const;
	const Boolean operator!=(LevelType second) const;
	const String str() const;
};

enum class IconType
{
	WhiteArrow = 0,
	GreenArrow = 1,
	RedArrow = 2,
	BlueArrow = 3,
	WhiteCross = 4,
	RedPointer = 5,
	WhiteCircle = 6,
	SmallWhiteCircle = 7,
	Mansion = 8,
	Temple = 9,
	Unused1 = 10,
	Unused2 = 11,
	Unused3 = 12,
	Unused4 = 13,
	Unused5 = 14,
	Unused6 = 15
};

// TODO: Use more robust color system
typedef Byte MapColor;

class MapIcon
{
	Position pos;
	IconType type;
	Byte angle;
};

class MapData
{
	Byte columns;
	Byte rows;
	Byte xOffset;
	Byte yOffset;
	std::vector<MapColor> colorData;
};

class Map
{
	Boolean showIcons;
	Byte scale;
	Int id;
	std::vector<MapIcon> icons;
	MapData data;
};

class PlayerPositionAndLookFlags
{
public:
	Boolean xIsRelative;
	Boolean yIsRelative;
	Boolean zIsRelative;
	Boolean yawIsRelative;
	Boolean pitchIsRelative;
	PlayerPositionAndLookFlags(Boolean xIsRelative, Boolean yIsRelative, Boolean zIsRelative, Boolean yawIsRelative, Boolean pitchIsRelative)
		: xIsRelative(xIsRelative), yIsRelative(yIsRelative), zIsRelative(zIsRelative), yawIsRelative(yawIsRelative), pitchIsRelative(pitchIsRelative) {}
	Byte getFlags() { return xIsRelative << 4 || yIsRelative << 3 || zIsRelative << 2 || yawIsRelative << 1 || pitchIsRelative; }
};

enum class StatusEffect
{
	Speed = 1,
	Slowness = 2,
	Haste = 3,
	MiningFatigue = 4,
	Strength = 5,
	InstantHealth = 6,
	InstantDamage = 7,
	JumpBoost = 8,
	Nausea = 9,
	Regeneration = 10,
	Resistance = 11,
	FireResistance = 12,
	WaterBreathing = 13,
	Invisibility = 14,
	Blindness = 15,
	NightVision = 16,
	Hunger = 17,
	Weakness = 18,
	Poison = 19,
	Wither = 20,
	HealthBoost = 21,
	Absorption = 22,
	Saturation = 23,
	Glowing = 24,
	Levitation = 25,
	Luck = 26,
	BadLuck = 27
};

class StatusEffectFlags
{
public:
	Boolean isAmbient;
	Boolean showParticles;
};

enum class ScoreboardPosition
{
	List = 0,
	Sidebar = 1,
	BelowName = 2
};

enum class EntityEquipmentSlot
{
	MainHand = 0,
	OffHand = 1,
	Boots = 2,
	Leggings = 3,
	Chestplate = 4,
	Helmet = 5
};

enum class ScoreboardMode
{
	Create = 0,
	Remove = 1,
	Update = 2
};

class ObjectiveType
{
private:
	String string;
public:
	static const ObjectiveType Integer;
	static const ObjectiveType Hearts;
	ObjectiveType(const char* string);
	ObjectiveType(const String string);
	ObjectiveType(const ObjectiveType& type);
	const Boolean operator==(ObjectiveType second) const;
	const Boolean operator!=(ObjectiveType second) const;
	const String str() const;
};

enum class TeamMode
{
	Create = 0,
	Remove = 1,
	UpdateInfo = 2,
	AddPlayers = 3,
	RemovePlayers = 4
};

class TeamNameTagVisibility
{
private:
	String string;
public:
	static const TeamNameTagVisibility AlwaysShow;
	static const TeamNameTagVisibility HideForOtherTeams;
	static const TeamNameTagVisibility HideForOwnTeam;
	static const TeamNameTagVisibility AlwaysHide;
	TeamNameTagVisibility(const char* string);
	TeamNameTagVisibility(const String string);
	TeamNameTagVisibility(const TeamNameTagVisibility& type);
	const Boolean operator==(TeamNameTagVisibility second) const;
	const Boolean operator!=(TeamNameTagVisibility second) const;
	const String str() const;
};

class TeamCollisionRule
{
private:
	String string;
public:
	static const TeamCollisionRule AlwaysPush;
	static const TeamCollisionRule PushOtherTeamsOnly;
	static const TeamCollisionRule PushOwnTeamOnly;
	static const TeamCollisionRule NeverPush;
	TeamCollisionRule(const char* string);
	TeamCollisionRule(const String string);
	TeamCollisionRule(const TeamCollisionRule& type);
	const Boolean operator==(TeamCollisionRule second) const;
	const Boolean operator!=(TeamCollisionRule second) const;
	const String str() const;
};

enum class ChatColor : Byte
{
	Black = 0,
	DarkBlue = 1,
	DarkGreen = 2,
	DarkCyan = 3,
	DarkRed = 4,
	DarkPurple = 5,
	Gold = 6,
	Gray = 7,
	DarkGray = 8,
	Blue = 9,
	BrightGreen = 10,
	Cyan = 11,
	Red = 12,
	Pink = 13,
	Yellow = 14,
	White = 15,
	Reset = -1
};

template <class T> // Any IEnumerable like vector<string>
class Team
{
public:
	Boolean friendlyFire;
	Boolean seeInvisibleMembers;
	ChatColor teamColor;
	TeamNameTagVisibility nameTagVisibility;
	TeamCollisionRule collisionRule;
	String name;
	String displayName;
	String teamPrefix;
	String teamSuffix;
	T members;
};

enum class Sound
{
	/* Blocks */
	AmbientCave = 0,
	AnvilBreak = 1,
	AnvilDestroy = 2,
	AnvilFall = 3,
	AnvilHit = 4,
	AnvilLand = 5,
	AnvilPlace = 6,
	AnvilStep = 7,
	AnvilUse = 8,
	BrewingStandBrew = 9,
	ChestClose = 10,
	ChestLocked = 11,
	ChestOpen = 12,
	ChorusFlowerDeath = 13,
	ChorusFlowerGrow = 14,
	ClothBreak = 15,
	ClothFall = 16,
	ClothHit = 17,
	ClothPlace = 18,
	ClothStep = 19,
	ComparatorClick = 20,
	DispenserDispense = 21,
	DispenserFail = 22,
	DispenserLaunch = 23,
	EnchantmentTableUse = 24,
	EndGatewaySpawn = 25,
	EnderChestClose = 26,
	EnderChestOpen = 27,
	FenceGateClose = 28,
	FenceGateOpen = 29,
	FireAmbient = 30,
	FireExtinguish = 31,
	FurnaceCrackle = 32,
	GlassBreak = 33,
	GlassFall = 34,
	GlassHit = 35,
	GlassPlace = 36,
	GlassStep = 37,
	GrassBreak = 38,
	GrassFall = 39,
	GrassHit = 40,
	GrassPlace = 41,
	GrassStep = 42,
	GravelBreak = 43,
	GravelFall = 44,
	GravelHit = 45,
	GravelPlace = 46,
	GravelStep = 47,
	IronDoorClose = 48,
	IronDoorOpen = 49,
	IronTrapdoorClose = 50,
	IronTrapdoorOpen = 51,
	LadderBreak = 52,
	LadderFall = 53,
	LadderHit = 54,
	LadderPlace = 55,
	LadderStep = 56,
	LavaAmbient = 57,
	LavaExtinguish = 58,
	LavaPop = 59,
	LeverClick = 60,
	MetalBreak = 61,
	MetalFall = 62,
	MetalHit = 63,
	MetalPlace = 64,
	MetalStep = 65,
	MetalPressurePlateClickOff = 66,
	MetalPressurePlateClickOn = 67,
	NoteBaseDrum = 68,
	NoteBass = 69,
	NoteHarp = 70,
	NoteHat = 71,
	NotePling = 72,
	NoteSnare = 73,
	PistonContract = 74,
	PistonExtend = 75,
	PortalAmbient = 76,
	PortalTravel = 77,
	PortalTrigger = 78,
	RedstoneTorchBurnout = 79,
	SandBreak = 80,
	SandFall = 81,
	SandHit = 82,
	SandPlace = 83,
	SandStep = 84,
	ShulkerBoxClose = 85,
	ShulkerBoxOpen = 86,
	SlimeBreak = 87,
	SlimeFall = 88,
	SlimeHit = 89,
	SlimePlace = 90,
	SlimeStep = 91,
	SnowBreak = 92,
	SnowFall = 93,
	SnowHit = 94,
	SnowPlace = 95,
	SnowStep = 96,
	StoneBreak = 97,
	StoneFall = 98,
	StoneHit = 99,
	StonePlace = 100,
	StoneStep = 101,
	StoneButtonClickOff = 102,
	StoneButtonClickOn = 103,
	StonePressurePlateClickOff = 104,
	StonePressurePlateClickOn = 105,
	TripwireAttach = 106,
	TripwireClickOff = 107,
	TripwireClickOn = 108,
	TripwireDetach = 109,
	WaterAmbient = 110,
	WaterlilyPlace = 111,
	WoodBreak = 112,
	WoodFall = 113,
	WoodHit = 114,
	WoodPlace = 115,
	WoodStep = 116,
	WoodButtonClickOff = 117,
	WoodButtonClickOn = 118,
	WoodPressurePlateClickOff = 119,
	WoodPressurePlateClickOn = 120,
	WoodenDoorClose = 121,
	WoodenDoorOpen = 122,
	WoodenTrapdoorClose = 123,
	WoodenTrapdoorOpen = 124,

	/* Enchantments */
	ThornsHit = 125,

	/* Entities */
	ArmorStandBreak = 126,
	ArmorStandFall = 127,
	ArmorStandHit = 128,
	ArmorStandPlace = 129,
	ArrowHit = 130,
	ArrowHitPlayer = 131,
	ArrowShoot = 132,
	BatAmbient = 133,
	BatDeath = 134,
	BatHurt = 135,
	BatLoop = 136,
	BatTakeoff = 137,
	BlazeAmbient = 138,
	BlazeBurn = 139,
	BlazeDeath = 140,
	BlazeHurt = 141,
	BlazeShoot = 142,
	BobberSplash = 143,
	BobberThrow = 144,
	CatAmbient = 145,
	CatDeath = 146,
	CatHiss = 147,
	CatHurt = 148,
	CatPurr = 149,
	CatPurreow = 150,
	ChickenAmbient = 151,
	ChickenDeath = 152,
	ChickenEgg = 153,
	ChickenHurt = 154,
	ChickenStep = 155,
	CowAmbient = 156,
	CowDeath = 157,
	CowHurt = 158,
	CowMilk = 159,
	CowStep = 160,
	CreeperDeath = 161,
	CreeperHurt = 162,
	CreeperPrimed = 163,
	DonkeyAmbient = 164,
	DonkeyAngry = 165,
	DonkeyChest = 166,
	DonkeyDeath = 167,
	DonkeyHurt = 168,
	EggThrow = 169,
	ElderGuardianAmbient = 170,
	ElderGuardianAmbientLand = 171,
	ElderGuardianCurse = 172,
	ElderGuardianDeath = 173,
	ElderGuardianDeathLand = 174,
	ElderGuardianFlop = 175,
	ElderGuardianHurt = 176,
	ElderGuardianHurtLand = 177,
	EnderDragonAmbient = 178,
	EnderDragonDeath = 179,
	EnderDragonFlap = 180,
	EnderDragonGrowl = 181,
	EnderDragonHurt = 182,
	EnderDragonShoot = 183,
	EnderDragonFireballExplode = 184,
	EnderEyeLaunch = 185,
	EndermanAmbient = 186,
	EndermanDeath = 187,
	EndermanHurt = 188,
	EndermanScream = 189,
	EndermanStare = 190,
	EndermanTeleport = 191,
	EndermiteAmbient = 192,
	EndermiteDeath = 193,
	EndermiteHurt = 194,
	EndermiteStep = 195,
	EnderPearlThrow = 196,
	EvocationFangsAttack = 197,
	EvocationIllagerAmbient = 198,
	EvocationIllagerCastSpell = 199,
	EvocationIllagerDeath = 200,
	EvocationIllagerHurt = 201,
	EvocationIllagerPrepareAttack = 202,
	EvocationIllagerPrepareSummon = 203,
	EvocationIllagerPrepareWololo = 204,
	ExperienceBottleThrow = 205,
	ExperienceOrbPickup = 206,
	FireworkBlast = 207,
	FireworkBlastFar = 208,
	FireworkLargeBlast = 209,
	FireworkLargeBlastFar = 210,
	FireworkLaunch = 211,
	FireworkShoot = 212,
	FireworkTwinkle = 213,
	FireworkTwinkleFar = 214,
	GenericBigFall = 215,
	GenericBurn = 216,
	GenericDeath = 217,
	GenericDrink = 218,
	GenericEat = 219,
	GenericExplode = 220,
	GenericExtinguishFire = 221,
	GenericHurt = 222,
	GenericSmallFall = 223,
	GenericSplash = 224,
	GenericSwim = 225,
	GhastAmbient = 226,
	GhastDeath = 227,
	GhastHurt = 228,
	GhastScream = 229,
	GhastShoot = 230,
	GhastWarn = 231,
	GuardianAmbient = 232,
	GuardianAmbientLand = 233,
	GuardianAttack = 234,
	GuardianDeath = 235,
	GuardianDeathLand = 236,
	GuardianFlop = 237,
	GuardianHurt = 238,
	GuardianHurtLand = 239,
	HorseAmbient = 240,
	HorseAngry = 241,
	HorseArmor = 242,
	HorseBreathe = 243,
	HorseDeath = 244,
	HorseEat = 245,
	HorseGallop = 246,
	HorseHurt = 247,
	HorseJump = 248,
	HorseLand = 249,
	HorseSaddle = 250,
	HorseStep = 251,
	HorseStepWood = 252,
	HostileBigFall = 253,
	HostileDeath = 254,
	HostileHurt = 255,
	HostileSmallFall = 256,
	HostileSplash = 257,
	HostileSwim = 258,
	HuskAmbient = 259,
	HuskDeath = 260,
	HuskHurt = 261,
	HuskStep = 262,
	IronGolemAttack = 263,
	IronGolemDeath = 264,
	IronGolemHurt = 265,
	IronGolemStep = 266,
	ItemBreak = 267,
	ItemPickup = 268,
	ItemFrameAddItem = 269,
	ItemFrameBreak = 270,
	ItemFramePlace = 271,
	ItemFrameRemoveItem = 272,
	ItemFrameRotateItem = 273,
	LeashKnotBreak = 274,
	LeashKnotPlace = 275,
	LightningImpact = 276,
	LightningThunder = 277,
	LingeringPotionThrow = 278,
	LlamaAmbient = 279,
	LlamaAngry = 280,
	LlamaChest = 281,
	LlamaDeath = 282,
	LlamaEat = 283,
	LlamaHurt = 284,
	LlamaSpit = 285,
	LlamaStep = 286,
	LlamaSwag = 287,
	MagmaCubeDeath = 288,
	MagmaCubeHurt = 289,
	MagmaCubeJump = 290,
	MagmaCubeSquish = 291,
	MinecartInside = 292,
	MinecartRiding = 293,
	MooshroomShear = 294,
	MuleAmbient = 295,
	MuleChest = 296,
	MuleDeath = 297,
	MuleHurt = 298,
	PaintingBreak = 299,
	PaintingPlace = 300,
	PigAmbient = 301,
	PigDeath = 302,
	PigHurt = 303,
	PigSaddle = 304,
	PigStep = 305,
	PlayerAttackCrit = 306,
	PlayerAttackKnockback = 307,
	PlayerAttackNoDamage = 308,
	PlayerAttackStrong = 309,
	PlayerAttackSweep = 310,
	PlayerAttackWeak = 311,
	PlayerBigFall = 312,
	PlayerBreath = 313,
	PlayerBurp = 314,
	PlayerDeath = 315,
	PlayerHurt = 316,
	PlayerLevelUp = 317,
	PlayerSmallFall = 318,
	PlayerSplash = 319,
	PlayerSwim = 320,
	PolarBearAmbient = 321,
	PolarBearBabyAmbient = 322,
	PolarBearDeath = 323,
	PolarBearHurt = 324,
	PolarBearStep = 325,
	PolarBearWarning = 326,
	RabbitAmbient = 327,
	RabbitAttack = 328,
	RabbitDeath = 329,
	RabbitHurt = 330,
	RabbitJump = 331,
	SheepAmbient = 332,
	SheepDeath = 333,
	SheepHurt = 334,
	SheepShear = 335,
	SheepStep = 336,
	ShulkerAmbient = 337,
	ShulkerClose = 338,
	ShulkerDeath = 339,
	ShulkerHurt = 340,
	ShulkerHurtClosed = 341,
	ShulkerOpen = 342,
	ShulkerShoot = 343,
	ShulkerTeleport = 344,
	ShulkerBulletHit = 345,
	ShulkerBulletHurt = 346,
	SilverfishAmbient = 347,
	SilverfishDeath = 348,
	SilverfishHurt = 349,
	SilverfishStep = 350,
	SkeletonAmbient = 351,
	SkeletonDeath = 352,
	SkeletonHurt = 353,
	SkeletonShoot = 354,
	SkeletonStep = 355,
	SkeletonHorseAmbient = 356,
	SkeletonHorseDeath = 357,
	SkeletonHorseHurt = 358,
	SlimeAttack = 359,
	SlimeDeath = 360,
	SlimeHurt = 361,
	SlimeJump = 362,
	SlimeSquish = 363,
	SmallMagmaCubeDeath = 364,
	SmallMagmaCubeHurt = 365,
	SmallMagmaCubeSquish = 366,
	SmallSlimeDeath = 367,
	SmallSlimeHurt = 368,
	SmallSlimeJump = 369,
	SmallSlimeSquish = 370,
	SnowballThrow = 371,
	SnowGolemAmbient = 372,
	SnowGolemDeath = 373,
	SnowGolemHurt = 374,
	SnowGolemShoot = 375,
	SpiderAmbient = 376,
	SpiderDeath = 377,
	SpiderHurt = 378,
	SpiderStep = 379,
	SplashPotionBreak = 380,
	SplashPotionThrow = 381,
	SquidAmbient = 382,
	SquidDeath = 383,
	SquidHurt = 384,
	StrayAmbient = 385,
	StrayDeath = 386,
	StrayHurt = 387,
	StrayStep = 388,
	TNTPrimed = 389,
	VexAmbient = 390,
	VexCharge = 391,
	VexDeath = 392,
	VexHurt = 393,
	VillagerAmbient = 394,
	VillagerDeath = 395,
	VillagerHurt = 396,
	VillagerNo = 397,
	VillagerTrading = 398,
	VillagerYes = 399,
	VindicationIllagerAmbient = 400,
	VindicationIllagerDeath = 401,
	VindicationIllagerHurt = 402,
	WitchAmbient = 403,
	WitchDeath = 404,
	WitchDrink = 405,
	WitchHurt = 406,
	WitchThrow = 407,
	WitherAmbient = 408,
	WitherBreakBlock = 409,
	WitherDeath = 410,
	WitherHurt = 411,
	WitherShoot = 412,
	WitherSpawn = 413,
	WitherSkeletonAmbient = 414,
	WitherSkeletonDeath = 415,
	WitherSkeletonHurt = 416,
	WitherSkeletonStep = 417,
	WolfAmbient = 418,
	WolfDeath = 419,
	WolfGrowl = 420,
	WolfHowl = 421,
	WolfHurt = 422,
	WolfPant = 423,
	WolfShake = 424,
	WolfStep = 425,
	WolfWhine = 426,
	ZombieAmbient = 427,
	ZombieAttackWoodDoor = 428,
	ZombieAttackIronDoor = 429,
	ZombieBreakWoodDoor = 430,
	ZombieDeath = 431,
	ZombieHurt = 432,
	ZombieInfect = 433,
	ZombieStep = 434,
	ZombieHorseAmbient = 435,
	ZombieHorseDeath = 436,
	ZombieHorseHurt = 437,
	ZombiePigmanAmbient = 438,
	ZombiePigmanAngry = 439,
	ZombiePigmanDeath = 440,
	ZombiePigmanHurt = 441,
	ZombieVillagerAmbient = 442,
	ZombieVillagerConverted = 443,
	ZombieVillagerCure = 444,
	ZombieVillagerDeath = 445,
	ZombieVillagerHurt = 446,
	ZombieVillagerStep = 447,

	/* Items */
	ArmorEquipChain = 448,
	ArmorEquipDiamond = 449,
	ArmorEquipElytra = 450,
	ArmorEquipGeneric = 451,
	ArmorEquipGold = 452,
	ArmorEquipIron = 453,
	ArmorEquipLeather = 454,
	BottleEmpty = 455,
	BottleFill = 456,
	BottleFillDragonBreath = 457,
	BucketEmpty = 458,
	BucketEmptyLava = 459,
	BucketFill = 460,
	BucketFillLava = 461,
	ChorusFruitTeleport = 462,
	ElytraFlying = 463,
	FireChargeUse = 464,
	FlintAndSteelUse = 465,
	HoeTill = 466,
	ShieldBlock = 467,
	ShieldBreak = 468,
	ShovelFlatten = 469,
	TotemUse = 470,

	/* Music */
	MusicCreative = 471,
	MusicCredits = 472,
	MusicDragon = 473,
	MusicEnd = 474,
	MusicGame = 475,
	MusicMenu = 476,
	MusicNether = 477,

	/* Records */
	Record11 = 478,
	Record13 = 479,
	RecordBlocks = 480,
	RecordCat = 481,
	RecordChirp = 482,
	RecordFar = 483,
	RecordMall = 484,
	RecordMellohi = 485,
	RecordStal = 486,
	RecordStrad = 487,
	RecordWait = 488,
	RecordWard = 489,

	/* User Interface */
	ButtonClick = 490,

	/* Weather */
	Rain = 491,
	RainAbove = 492
};
