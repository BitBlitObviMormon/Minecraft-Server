#pragma once

enum class BlockID
{
	Air = 0,
	Stone = 1,
	Grass = 2,
	Dirt = 3,
	Cobblestone = 4,
	Planks = 5,
	Sapling = 6,
	Bedrock = 7,
	FlowingWater = 8,
	StillWater = 9,
	FlowingLava = 10,
	StillLava = 11,
	Sand = 12,
	Gravel = 13,
	GoldOre = 14,
	IronOre = 15,
	CoalOre = 16,
	Log = 17,
	Leaves = 18,
	Sponge = 19,
	Glass = 20,
	LapisLazuliOre = 21,
	LapisLazuliBlock = 22,
	Dispenser = 23,
	Sandstone = 24,
	NoteBlock = 25,
	Bed = 26,
	PoweredRail = 27,
	DetectorRail = 28,
	StickyPiston = 29,
	Cobweb = 30,
	Shrub = 31,
	DeadBush = 32,
	Piston = 33,
	PistonHead = 34,
	Wool = 35,
	PistonExtension = 36,
	Dandelion = 37,
	Flower = 38,
	BrownMushroom = 39,
	RedMushroom = 40,
	GoldBlock = 41,
	IronBlock = 42,
	DoubleSlab = 43,
	Slab = 44,
	RedBricks = 45,
	TNT = 46,
	Bookshelf = 47,
	MossStone = 48,
	Obsidian = 49,
	Torch = 50,
	Fire = 51,
	MonsterSpawner = 52,
	OakStairs = 53,
	Chest = 54,
	Redstone = 55,
	DiamondOre = 56,
	DiamondBlock = 57,
	CraftingTable = 58,
	WheatCrops = 59,
	Farmland = 60,
	Furnace = 61,
	LitFurnace = 62,
	StandingSign = 63,
	OakDoor = 64,
	Ladder = 65,
	Rail = 66,
	CobblestoneStairs = 67,
	WallMountedSign = 68,
	Lever = 69,
	StonePressurePlate = 70,
	IronDoor = 71,
	WoodenPressurePlate = 72,
	RedstoneOre = 73,
	GlowingRedstoneOre = 74,
	RedstoneTorch = 75,
	LitRedstoneTorch = 76,
	StoneButton = 77,
	Snow = 78,
	Ice = 79,
	SnowBlock = 80,
	Cactus = 81,
	Clay = 82,
	SugarCane = 83,
	Jukebox = 84,
	OakFence = 85,
	Pumpkin = 86,
	Netherrack = 87,
	SoulSand = 88,
	Glowstone = 89,
	NetherPortal = 90,
	JackOLantern = 91,
	Cake = 92,
	RedstoneRepeater = 93,
	LitRedstoneRepeater = 94,
	StainedGlass = 95,
	WoodenTrapdoor = 96,
	MonsterEgg = 97,
	StoneBrick = 98,
	BrownMushroomBlock = 99,
	RedMushroomBlock = 100,
	IronBars = 101,
	GlassPane = 102,
	MelonBlock = 103,
	PumpkinStem = 104,
	MelonStem = 105,
	Vines = 106,
	OakFenceGate = 107,
	BrickStairs = 108,
	StoneBrickStairs = 109,
	Mycelium = 110,
	LilyPad = 111,
	NetherBrick = 112,
	NetherBrickFence = 113,
	NetherBrickStairs = 114,
	NetherWart = 115,
	EnchantmentTable = 116,
	BrewingStand = 117,
	Cauldron = 118,
	EndPortal = 119,
	EndPortalFrame = 120,
	EndStone = 121,
	DragonEgg = 122,
	RedstoneLamp = 123,
	LitRedstoneLamp = 124,
	DoubleWoodSlab = 125,
	WoodSlab = 126,
	Cocoa = 127,
	SandstoneStairs = 128,
	EmeraldOre = 129,
	EnderChest = 130,
	TripwireHook = 131,
	Tripwire = 132,
	EmeraldBlock = 133,
	SpruceWoodStairs = 134,
	BirchWoodStairs = 135,
	JungleWoodStairs = 136,
	CommandBlock = 137,
	Beacon = 138,
	CobblestoneWall = 139,
	FlowerPot = 140,
	Carrots = 141,
	Potatoes = 142,
	WoodenButton = 143,
	MobHead = 144,
	Anvil = 145,
	TrappedChest = 146,
	LightWeightedPressurePlate = 147,
	HeavyWeightedPressurePlate = 148,
	RedstoneComparator = 149,
	LitRedstoneComparator = 150,
	DaylightSensor = 151,
	RedstoneBlock = 152,
	NetherQuartzOre = 153,
	Hopper = 154,
	QuartzBlock = 155,
	QuartzStairs = 156,
	ActivatorRail = 157,
	Dropper = 158,
	StainedClay = 159,
	StainedGlassPane = 160,
	Leaves2 = 161,
	Log2 = 162,
	AcaciaStairs = 163,
	DarkOakStairs = 164,
	SlimeBlock = 165,
	Barrier = 166,
	IronTrapdoor = 167,
	Prismarine = 168,
	SeaLantern = 169,
	HayBale = 170,
	Carpet = 171,
	HardenedClay = 172,
	CoalBlock = 173,
	PackedIce = 174,
	TallPlant = 175,
	StandingBanner = 176,
	WallMountedBanner = 177,
	InvertedDaylightSensor = 178,
	RedSandstone = 179,
	RedSandstoneStairs = 180,
	DoubleRedSandstoneSlab = 181,
	RedSandstoneSlab = 182,
	SpruceFenceGate = 183,
	BirchFenceGate = 184,
	JungleFenceGate = 185,
	DarkOakFenceGate = 186,
	AcaciaFenceGate = 187,
	SpruceFence = 188,
	BirchFence = 189,
	JungleFence = 190,
	DarkOakFence = 191,
	AcaciaFence = 192,
	SpruceDoor = 193,
	BirchDoor = 194,
	JungleDoor = 195,
	DarkOakDoor = 196,
	AcaciaDoor = 197,
	EndRod = 198,
	ChorusPlant = 199,
	ChorusFlower = 200,
	PurpurBlock = 201,
	PurpurPillar = 202,
	PurpurStairs = 203,
	PurpurDoubleSlab = 204,
	PurpurSlab = 205,
	EndStoneBricks = 206,
	Beetroot = 207,
	GrassPath = 208,
	EndGateway = 209,
	RepeatingCommandBlock = 210,
	ChainCommandBlock = 211,
	FrostedIce = 212,
	MagmaBlock = 213,
	NetherWartBlock = 214,
	RedNetherBrick = 215,
	BoneBrick = 216,
	StructureVoid = 217,
	Observer = 218,
	WhiteShulkerBox = 219,
	OrangeShulkerBox = 220,
	MagentaShulkerBox = 221,
	LightBlueShulkerBox = 222,
	YellowShulkerBox = 223,
	LimeShulkerBox = 224,
	PinkShulkerBox = 225,
	GrayShulkerBox = 226,
	LightGrayShulkerBox = 227,
	CyanShulkerBox = 228,
	PurpleShulkerBox = 229,
	BlueShulkerBox = 230,
	BrownShulkerBox = 231,
	GreenShulkerBox = 232,
	RedShulkerBox = 233,
	BlackShulkerBox = 234,
	WhiteTerracotta = 235,
	OrangeTerracotta = 236,
	MagentaTerracotta = 237,
	LightBlueTerracotta = 238,
	YellowTerracotta = 239,
	LimeTerracotta = 240,
	PinkTerracotta = 241,
	GrayTerracotta = 242,
	LightGrayTerracotta = 243,
	CyanTerracotta = 244,
	PurpleTerracotta = 245,
	BlueTerracotta = 246,
	BrownTerracotta = 247,
	GreenTerracotta = 248,
	RedTerracotta = 249,
	BlackTerracotta = 250,
	Concrete = 251,
	ConcretePowder = 252,
	Unused1 = 253,
	Unused2 = 254,
	StructureBlock = 255
};

enum class StoneID
{
	Stone = 0,
	Granite = 1,
	PolishedGranite = 2,
	Diorite = 3,
	PolishedDiorite = 4,
	Andesite = 5,
	PolishedAndesite = 6
};

enum class DirtID
{
	Dirt = 0,
	CoarseDirt = 1,
	Podzol = 2
};

enum class WoodID
{
	Oak = 0,
	Spruce = 1,
	Birch = 2,
	Jungle = 3,
	Acacia = 4,
	DarkOak = 5
};

enum class Wood2ID
{
	Acacia = 0,
	DarkOak = 1
};

enum class SandID
{
	Sand = 0,
	RedSand = 1
};

enum class SpongeID
{
	Sponge = 0,
	WetSponge = 1
};

enum class SandstoneID
{
	Sandstone = 0,
	ChiseledSandstone = 1,
	SmoothSandstone = 2
};

enum class ShrubID
{
	DeadShrub = 0,
	Grass = 1,
	Fern = 2
};

enum class ColorID
{
	White = 0,
	Orange = 1,
	Magenta = 2,
	LightBlue = 3,
	Yellow = 4,
	Lime = 5,
	Pink = 6,
	Gray = 7,
	LightGray = 8,
	Cyan = 9,
	Purple = 10,
	Blue = 11,
	Brown = 12,
	Green = 13,
	Red = 14,
	Black = 15
};

enum class FlowerID
{
	Poppy = 0,
	BlueOrchid = 1,
	Allium = 2,
	AzureBluet = 3,
	RedTulip = 4,
	OrangeTulip = 5,
	WhiteTulip = 6,
	PinkTulip = 7,
	OxeyeDaisy = 8
};

enum class SlabID
{
	Stone = 0,
	Sandstone = 1,
	Wooden = 2,
	Cobblestone = 3,
	Brick = 4,
	StoneBrick = 5,
	NetherBrick = 6,
	Quartz = 7
};

enum class MonsterEggID
{
	Stone = 0,
	Cobblestone = 1,
	StoneBrick = 2,
	MossyStoneBrick = 3,
	CrackedStoneBrick = 4,
	ChiseledStoneBrick = 5
};

enum class StoneBrickID
{
	StoneBrick = 0,
	MossyStoneBrick = 1,
	CrackedStoneBrick = 2,
	ChiseledStoneBrick = 3
};

enum class CobblestoneWallID
{
	CobblestoneWall = 0,
	MossyCobblestoneWall = 1
};

enum class QuartzID
{
	Quartz = 0,
	ChiseledQuartz = 1,
	PillarQuartz = 2
};

enum class PrismarineID
{
	Prismarine = 0,
	PrismarineBlocks = 1,
	DarkPrismarine = 2
};

enum class TallPlantID
{
	Sunflower = 0,
	Lilac = 1,
	TallGrass = 2,
	TallFern = 3,
	RoseBush = 4,
	Peony = 5
};