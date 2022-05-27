#pragma once

#include "datatypes.h"
#include "serverevents.h"

/**************************************
 * Object Type                        *
 * Most projectiles and some entities *
 **************************************/
enum struct ObjectType
{
	Boat = 1,
	ItemStack = 2,
	AreaEffectCloud = 3,
	Minecart = 10,
	PrimedTNT = 50,
	EnderCrystal = 51,
	Arrow = 60,
	Snowball = 61,
	Egg = 62,
	Fireball = 63,
	FireCharge = 64,
	Enderpearl = 65,
	WitherSkull = 66,
	ShulkerBullet = 67,
	LlamaSpit = 68,
	FallingObject = 70,
	ItemFrame = 71,
	EyeOfEnder = 72,
	ThrownPotion = 73,
	ThrownExpBottle = 75,
	FireworkRocket = 76,
	LeashKnot = 77,
	ArmorStand = 78,
	EvocationFangs = 79,
	FishingHook = 90,
	SpectralArrow = 91,
	DragonFireball = 93
};

/**********************
 * Object             *
 * A Minecraft object *
 **********************/
class Object
{
public:
	Byte pitch;
	Byte yaw;
	ObjectType type;
	Int entityID;
	Int data;
	UUID uuid;
	PositionF pos;
	Position velocity;
};

/********************
 * Experience Orb   *
 * An XP-giving orb *
 ********************/
class ExperienceOrb
{
public:
	Short exp;
	Int entityID;
	PositionF pos;
};

/*****************************
 * Global Entity Type        *
 * Only thunderbolts for now *
 *****************************/
enum struct GlobalEntityType
{
	Thunderbolt = 1
};

/*****************************
 * Global Entity             *
 * Only thunderbolts for now *
 *****************************/
class GlobalEntity
{
public:
	GlobalEntityType type;
	Int entityID;
	PositionF pos;
};

/***************
 * Entity Type *
 ***************/
enum struct EntityType
{
	/* Use sendSpawnMob to create these entities on a client */
	ElderGuardian = 4,
	WitherSkeleton = 5,
	Stray = 6,
	Husk = 23,
	ZombieVillager = 27,
	SkeletonHorse = 28,
	ZombieHorse = 29,
	ArmorStand = 30,
	Donkey = 31,
	Mule = 32,
	EvocationIllager = 34,
	Vex = 35,
	VindicationIllager = 36,
	IllusionIllager = 37,
	Creeper = 50,
	Skeleton = 51,
	Spider = 52,
	Giant = 53,
	Zombie = 54,
	Slime = 55,
	Ghast = 56,
	ZombiePigman = 57,
	Enderman = 58,
	CaveSpider = 59,
	Silverfish = 60,
	Blaze = 61,
	LavaSlime = 62,
	EnderDragon = 63,
	WitherBoss = 64,
	Bat = 65,
	Witch = 66,
	Endermite = 67,
	Guardian = 68,
	Shulker = 69,
	Pig = 90,
	Sheep = 91,
	Cow = 92,
	Chicken = 93,
	Squid = 94,
	Wolf = 95,
	Mooshroom = 96,
	SnowGolem = 97,
	Ocelot = 98,
	IronGolem = 99,
	Horse = 100,
	Rabbit = 101,
	PolarBear = 102,
	Llama = 103,
	Parrot = 105,
	Villager = 120,

	/* Use sendSpawnExperienceOrb to create an XPOrb on a client */
	XPOrb = 2,

	/* Use sendSpawnPainting to create a painting on a client */
	Painting = 9,

	/* Use sendSpawnObject to create these entities on a client */
	Item = 1,
	AreaEffectCloud = 3,
	Egg = 7,
	LeashKnot = 8,
	Arrow = 10,
	Snowball = 11,
	Fireball = 12,
	FireCharge = 13,
	Enderpearl = 14,
	EyeOfEnderSignal = 15,
	ThrownPotion = 16,
	ThrownExpBottle = 17,
	ItemFrame = 18,
	WitherSkull = 19,
	PrimedTNT = 20,
	FallingSand = 21,
	FireworksRocket = 22,
	SpectralArrow = 24,
	ShulkerBullet = 25,
	DragonFireball = 26,
	EvocationFangs = 33,
	MinecartCommandBlock = 40,
	Boat = 41,
	MinecartRideable = 42,
	MinecartChest = 43,
	MinecartFurnace = 44,
	MinecartTNT = 45,
	MinecartHopper = 46,
	MinecartSpawner = 47,
	LlamaSpit = 104,
	EnderCrystal = 200
};

/**********************
 * Entity             *
 * A Minecraft Entity *
 **********************/
class Entity
{
public:
	Byte yaw;
	Byte pitch;
	Byte headPitch;
	EntityType type;
	UUID uuid;
	PositionF pos;
	Position velocity;
};

/************************
 * Painting             *
 * A Minecraft painting *
 ************************/
class Painting
{
public:
	Direction direction;
	Position pos;
	Int entityID;
	UUID uuid;
	String title;
};

/*******************
 * Player          *
 * A player entity *
 *******************/
class PlayerEntity : public Entity
{
public:
};
