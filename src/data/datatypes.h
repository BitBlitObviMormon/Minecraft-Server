#pragma once

#include <stdint.h>
#include <string>

// Size definitions
#define VARINT_MAX_SIZE			5
#define VARLONG_MAX_SIZE		10
#define VARNUM_MAX_SIZE			100
#define SERIALSTRING_MAX_LENGTH	32767
#define SERIALSTRING_MAX_SIZE	131071

// Simple types
typedef bool Boolean;
typedef int8_t Byte;
typedef uint8_t UByte;
typedef int16_t Short;
typedef uint16_t UShort;
typedef int32_t Int;
typedef uint32_t UInt;
typedef int64_t Long;
typedef uint64_t ULong;
typedef float Float;
typedef double Double;
typedef std::string String;

/**************************
 * VarNum                 *
 * Variable length BigNum *
 **************************/
class VarNum
{
protected:
	Byte  length; // Length of the array
	Byte* data;   // The data
	VarNum(Byte* data = 0);
	VarNum(const Long value);
	VarNum(const Int value);
	VarNum(const VarNum& value);
	~VarNum();
	Long toNum() const;
public:
	const Byte getSize() const;
	const Byte* getData() const;
	friend VarNum& makeNumVar(Long value, const Byte maxlength);
	friend VarNum& makeNumVar(Int value, const Byte maxlength);
};

/**********************************
 * VarInt                         *
 * Variable length 32-bit integer *
 **********************************/
class VarLong;
class VarInt : public VarNum
{
public:
	VarInt(Byte* data = 0);
	VarInt(const VarInt& value);
	VarInt(const Int value);
	VarInt(const VarLong& value);
	~VarInt();
	Int toInt() const;
};

/**********************************
 * VarLong                        *
 * Variable length 64-bit integer *
 **********************************/
class VarLong : public VarNum
{
public:
	VarLong(Byte* data = 0);
	VarLong(const VarLong& value);
	VarLong(const Long value);
	VarLong(const VarInt& value);
	~VarLong();
	Long toLong() const;
};

/*******************************************
 * SerialString                            *
 * A serializable string used in Minecraft *
 *******************************************/
class SerialString
{
protected:
	VarInt length; // Length of the array
	char*  data;   // A UTF-8 string with no null terminator
public:
	SerialString();
	SerialString(String text);
	~SerialString();
};

/***********************************************
 * Chat                                        *
 * The serializable string with the max length *
 ***********************************************/
class Chat : public SerialString
{
public:
	Chat();
	Chat(String text);
	~Chat();
};

/*****************************
 * Position                  *
 * The position of an object *
 *****************************/
class Position
{
private:
	Long data;
public:
	Position();
	Position(Int x, Int y, Int z);
	~Position();
	const Int getX() const;
	const Int getY() const;
	const Int getZ() const;
};

/**************************
 * Angle                  *
 * The angle of an object *
 **************************/
class Angle
{
public:
	Byte angle;
	Angle();
	Angle(Byte angle);
	~Angle();
};

/*******************************
 * UUID                        *
 * An unsigned 128-bit integer *
 *******************************/
class UUID
{
private:
	Long v1;
	Long v2;
public:
	UUID();
	UUID(Long value1, Long value2);
	~UUID();
};

/************************
 * ChunkSection         *
 * A section of a chunk *
 ************************/
class ChunkSection
{
private:
public:
	// TODO: Create ChunkSection
};

/*********************************
 * EntityData                *
 * The metadata each entity uses *
 *********************************/
class EntityData
{
private:
public:
	// Todo: Create EntityData
};

/**********************************************
 * Slot                                       *
 * An item stack in an inventory or container *
 **********************************************/
class Slot
{
private:
public:
	// TODO: Create Slot
};

/**************
 * NBT        *
 * An NBT Tag *
 **************/
class NBT
{
private:
public:
	// TODO: Create NBT
};
