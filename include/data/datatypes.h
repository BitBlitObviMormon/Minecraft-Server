#pragma once

#include "data/blocks.h"
#include "data/biomes.h"
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
class Client;

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
	VarNum(VarNum&& value);
	~VarNum();
	Long toNum() const;
public:
	const Byte getSize() const;
	const Byte* getData() const;
	friend void makeNumVar(Long value, const Byte maxlength, VarNum& num);
	friend void makeNumVar(Int value, const Byte maxlength, VarNum& num);
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
	VarInt& operator=(const VarInt& right);
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
	VarLong& operator=(const VarLong& right);
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
	Byte*  data;   // A UTF-8 string with no null terminator
public:
	SerialString(Byte* data = NULL);
	SerialString(const String& text);
	SerialString(const SerialString& str);
	~SerialString();
	SerialString& operator=(const SerialString& rhs);
	const Int getLength() const;
	const Int getSize() const;
	char* makeData() const;
	const String str() const;
};

/***********************************************
 * Chat                                        *
 * The serializable string with the max length *
 ***********************************************/
class Chat : public SerialString
{
public:
	Chat(Byte* data = NULL);
	Chat(const String& text);
	~Chat();
};

/*****************************
 * Position                  *
 * The position of an object *
 *****************************/
class Position
{
public:
	Int x;
	Int y;
	Int z;
	Position(Int x = 0, Int y = 0, Int z = 0);
};

/*****************************
 * Position (Float)          *
 * The position of an object *
 *****************************/
class PositionF
{
public:
	Double x;
	Double y;
	Double z;
	PositionF(Double x = 0, Double y = 0, Double z = 0);
};

/*****************************
 * SerialPosition            *
 * The position of an object *
 *****************************/
class SerialPosition
{
private:
	Long data;
public:
	SerialPosition(Long data = 0);
	SerialPosition(Int x, Int y, Int z);
	SerialPosition(Position pos);
	~SerialPosition();
	const Int getX() const;
	const Int getY() const;
	const Int getZ() const;
	const Long getData() const;
	const Position toPosition() const;
};

/**************************
 * Angle                  *
 * The angle of an object *
 **************************/
class Angle
{
public:
	Byte angle;
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
	UUID(Long value1 = 0, Long value2 = 0);
	UUID(Client* client);
	~UUID();
	void recalculate(Client* client);
	const String str() const;
	const Long getFirst() const;
	const Long getSecond() const;
};

/************************
 * ChunkSection         *
 * A section of a chunk *
 * optimized for memory *
 ************************/
class ChunkSection
{
private:
	// If the chunk is empty, the block array is null
	Byte*  lights; // First nibble is block lighting; second nibble is sky lighting
	Short* blocks; // First 3 nibbles is block id; last nibble is block metadata
public:
	ChunkSection() : lights(NULL), blocks(NULL) {}
	~ChunkSection() { if (lights) delete[] lights; if (blocks) delete[] blocks; }
	Boolean empty() { return !blocks; }
	void deleteBlocks() { if (blocks) delete[] blocks; blocks = NULL; }
	void deleteLights() { if (lights) delete[] lights; lights = NULL; }
	Short getBlockData(int index) { return blocks[index]; }
	Short getBlockData(int x, int y, int z) { return getBlockData(y * 256 + z * 16 + x); }
	BlockID getBlock(int index) { return (BlockID)(((UByte)blocks[index]) >> 4); }
	Byte getBlockState(int index) { return blocks[index] & 0xF; }
	Byte getBlockLighting(int index) { return ((UByte)lights[index]) >> 4; }
	Byte getSkyLighting(int index) { return (UByte)lights[index] & 0xF; }
	BlockID getBlock(int x, int y, int z) { return getBlock(y * 256 + z * 16 + x); }
	Byte getBlockState(int x, int y, int z) { return getBlockState(y * 256 + z * 16 + x); }
	Byte getBlockLighting(int x, int y, int z) { return getBlockLighting(y * 256 + z * 16 + x); }
	Byte getSkyLighting(int x, int y, int z) { return getSkyLighting(y * 256 + z * 16 + x); }
	void setBlock(int index, BlockID blockid);
	void setBlock(int index, BlockID blockid, Byte blockstate);
	void setBlockState(int index, Byte blockstate);
	void setBlockLighting(int index, Byte value);
	void setSkyLighting(int index, Byte value);
	void setLighting(int index, Byte blockLightValue, Byte skyLightValue);
	void setBlock(int x, int y, int z, BlockID blockid) { setBlock(y * 256 + z * 16 + x, blockid); }
	void setBlock(int x, int y, int z, BlockID blockid, Byte blockstate) { setBlock(y * 256 + z * 16 + x, blockid, blockstate); }
	void setBlockState(int x, int y, int z, Byte blockstate) { setBlockState(y * 256 + z * 16 + x, blockstate); }
	void setBlockLighting(int x, int y, int z, Byte value) { setBlockLighting(y * 256 + z * 16 + x, value); }
	void setSkyLighting(int x, int y, int z, Byte value) { setSkyLighting(y * 256 + z * 16 + x, value); }
	void setLighting(int x, int y, int z, Byte blockLightValue, Byte skyLightValue) { setLighting(y * 256 + z * 16 + x, blockLightValue, skyLightValue); }
	void fillBlocks(BlockID blockid = BlockID::Air, Byte blockstate = 0);
	void fillLighting(Byte blockLightValue = 15, Byte skyLightValue = 15);
};

/*************************************
 * ChunkColumn                       *
 * A 256-block high column of chunks *
 *************************************/
class ChunkColumn
{
protected:
	BiomeID* biomes;
public:
	ChunkSection chunks[16];
	ChunkColumn() : biomes(NULL) {}
	~ChunkColumn() { if (biomes) delete[] biomes; }
	Boolean noBiomes() { return !biomes; }
	BiomeID& getBiome(int index) { return biomes[index]; }
	BiomeID& getBiome(int x, int z) { return getBiome(z * 16 + x); }
	void fillBiomes(BiomeID biome = BiomeID::TheVoid) { if (!biomes) biomes = new BiomeID[256]; for (int i = 0; i < 256; ++i) biomes[i] = biome; }
	void setBiome(int index, BiomeID biome) { if (!biomes) biomes = new BiomeID[256]; biomes[index] = biome; }
	void setBiome(int x, int z, BiomeID biome) { setBiome(z * 16 + x, biome); }
};

/**************************
 * SerialChunkSection     *
 * A section of a chunk   *
 * optimized for networks *
 **************************/
class SerialChunkSection
{
private:
	UByte bitsPerBlock;
	Int paletteLength;
	VarInt* palette;
	Int dataArrayLength;
	Long* dataArray;
	Byte* blockLights;
	Byte* skyLights;
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
