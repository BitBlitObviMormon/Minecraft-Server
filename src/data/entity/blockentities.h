#pragma once

#include "../../server/serverevents.h"
#include "../datatypes.h"

/*********************
 * Block Entity Type *
 *********************/
enum class BlockEntityType
{
	MobSpawner = 1,
	CommandBlock = 2,
	Beacon = 3,
	MobHead = 4,
	FlowerPot = 5,
	Banner = 6,
	StructureTile = 7,
	EndGateway = 8,
	Sign = 9,
	ShulkerBox = 10
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
};

/****************************
 * Block Entity             *
 * A minecraft block entity *
 ****************************/
class ShulkerBox : public BlockEntity
{
public:
};

