#pragma once

#include "data/datatypes.h"
#include "server/serverevents.h"

/*********************
 * Block Entity Type *
 *********************/
enum class BlockEntityType
{
	InvalidBlockEntity = 0,
	MobSpawner = 1,
	CommandBlock = 2,
	Beacon = 3,
	MobHead = 4,
	FlowerPot = 5,
	Banner = 6,
	StructureTile = 7,
	EndGateway = 8,
	Sign = 9,
	ShulkerBox = 10,

	// These ids are only useful for block entity actions and are ignored otherwise
	NoteBlock = 11,
	Piston = 12,
	StickyPiston = 13,
	Chest = 14,
	TrappedChest = 15,
	EnderChest = 16
};

enum class BlockEntityActionID
{
	NoteBlock = 25,
	Piston = 33,
	StickyPiston = 29,
	Chest = 54,
	TrappedChest = 146,
	EnderChest = 130,
	Beacon = 138,
	MobSpawner = 52,
	EndGateway = 209,
	
	// Each shulker box corresponds to a different color: Find out which!
	ShulkerBox0 = 219,
	ShulkerBox1 = 220,
	ShulkerBox2 = 221,
	ShulkerBox3 = 222,
	ShulkerBox4 = 223,
	ShulkerBox5 = 224,
	ShulkerBox6 = 225,
	ShulkerBox7 = 226,
	ShulkerBox8 = 227,
	ShulkerBox9 = 228,
	ShulkerBoxA = 229,
	ShulkerBoxB = 230,
	ShulkerBoxC = 231,
	ShulkerBoxD = 232,
	ShulkerBoxE = 233,
	ShulkerBoxF = 234
};

/****************************
 * Block Entity             *
 * A minecraft block entity *
 ****************************/
class BlockEntity
{
public:
	Position pos;
	BlockEntityType type;
	BlockEntity(BlockEntityType type = BlockEntityType::InvalidBlockEntity) : type(type) {}
};

/****************************
 * Block Entity             *
 * A minecraft block entity *
 ****************************/
class MobSpawner : public BlockEntity
{
public:
};

/****************************
 * Block Entity             *
 * A minecraft block entity *
 ****************************/
class CommandBlock : public BlockEntity
{
public:
};

/****************************
 * Block Entity             *
 * A minecraft block entity *
 ****************************/
class Beacon : public BlockEntity
{
public:
};

/****************************
 * Block Entity             *
 * A minecraft block entity *
 ****************************/
class MobHead : public BlockEntity
{
public:
};

/****************************
 * Block Entity             *
 * A minecraft block entity *
 ****************************/
class FlowerPot : public BlockEntity
{
public:
};

/****************************
 * Block Entity             *
 * A minecraft block entity *
 ****************************/
class Banner : public BlockEntity
{
public:
};

/****************************
 * Block Entity             *
 * A minecraft block entity *
 ****************************/
class StructureTile : public BlockEntity
{
public:
};

/****************************
 * Block Entity             *
 * A minecraft block entity *
 ****************************/
class EndGateway : public BlockEntity
{
public:
};

/****************************
 * Block Entity             *
 * A minecraft block entity *
 ****************************/
class Sign : public BlockEntity
{
public:
	String line1;
	String line2;
	String line3;
	String line4;
};

/********************
 * Note Block Sound *
 ********************/
enum class NoteBlockSound
{
	Harp = 0,
	BassDrum = 1,
	SnareDrum = 2,
	Click = 3,
	BassGuitar = 4,
	Flute = 5,
	Bell = 6,
	Guitar = 7,
	Chime = 8,
	Xylophone = 9
};

/****************************
 * Block Entity             *
 * A minecraft block entity *
 ****************************/
class NoteBlock : public BlockEntity
{
public:
	NoteBlockSound sound;
};

/********************
 * Piston Direction *
 ********************/
enum class PistonDirection
{
	Down = 0,
	Up = 1,
	South = 2,
	West = 3,
	North = 4,
	East = 5
};

/****************************
 * Block Entity             *
 * A minecraft block entity *
 ****************************/
class Piston : public BlockEntity
{
public:
	Boolean isRetracted;
	PistonDirection direction;
};

/*****************
 * Sticky Piston *
 *****************/
class StickyPiston : public Piston {};

/****************************
 * Block Entity             *
 * A minecraft block entity *
 ****************************/
class Chest : public BlockEntity
{
public:
	Byte playersUsing;
	Chest(BlockEntityType type) : BlockEntity(type) {}
};

/****************
 * Other Chests *
 ****************/
class TrappedChest : public Chest { public: TrappedChest() : Chest(BlockEntityType::TrappedChest) {} };
class EnderChest : public Chest { public: EnderChest() : Chest(BlockEntityType::EnderChest) {} };
class ShulkerBox : public Chest { public: Byte color; ShulkerBox(Byte color = 0) : color(color), Chest(BlockEntityType::ShulkerBox) {} };
