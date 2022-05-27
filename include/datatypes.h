#pragma once

#include "blocks.h"
#include "biomes.h"
#include <set>
#include <list>
#include <memory>
#include <string>
#include <iostream>
#include <stdint.h>

// Size definitions
constexpr auto VARNUM_PARSE_ERROR = -1;
constexpr auto VARINT_MAX_SIZE = 5;
constexpr auto VARLONG_MAX_SIZE = 10;
constexpr auto VARNUM_MAX_SIZE = 100;
constexpr auto SERIALSTRING_MAX_LENGTH = 32767;
constexpr auto SERIALSTRING_MAX_SIZE = 131071;
constexpr auto SERIALCHAT_MAX_LENGTH = 262144;
constexpr auto SERIALCHAT_MAX_SIZE = 1048579;

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
template<typename T> using List = std::list<T>;
template<typename T> using Set = std::set<T>;
class Client;

/**********************************
 * VarInt                         *
 * Variable length 32-bit integer *
 **********************************/
class VarLong;
class VarInt
{
protected:
	Byte length;
	Byte data[VARINT_MAX_SIZE];
public:
	VarInt(const Byte* data = nullptr);
	VarInt(const VarInt& value);
	VarInt(const Int value);
	VarInt& operator=(const VarInt& rhs);
	Byte size() const { return length; }
	const Byte* makeData() const { return data; }
	Int toInt() const;
};

/**********************************
 * VarLong                        *
 * Variable length 64-bit integer *
 **********************************/
class VarLong
{
protected:
	Byte length;
	Byte data[VARLONG_MAX_SIZE];
public:
	VarLong(const Byte* data = nullptr);
	VarLong(const VarLong& value);
	VarLong(const Long value);
	VarLong& operator=(const VarLong& rhs);
	Byte size() const { return length; }
	const Byte* makeData() const { return data; }
	Long toLong() const;
};

/*******************************************
 * SerialString                            *
 * A serializable string used in Minecraft *
 *******************************************/
class SerialString
{
protected:
	// Byte*  data;   // A UTF-8 string with no null terminator
	VarInt lenInt; // Length of the array
	String data;
public:
	SerialString(const Byte* data = nullptr);
	SerialString(const String& str) : data(str), lenInt(str.length()) {}
	SerialString(const SerialString& str) : data(str.data), lenInt(str.data.length()) {}
	SerialString& operator=(const SerialString& rhs) { data = rhs.data; return *this; }
	const String str() const { return data; }
	const Int length() const { return data.length(); }
	const Int size() const { return lenInt.size() + data.length(); }
	std::unique_ptr<Byte[]> makeData() const;
};

/***********************************************
 * Chat                                        *
 * The serializable string with the max length *
 ***********************************************/
class Chat : public SerialString
{
public:
	Chat(const Byte* data = nullptr) : SerialString(data) {}
	Chat(const String& str) : SerialString(str) {}
	Chat(const Chat& str) : SerialString(str) {}
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
	Position(const Int x = 0, const Int y = 0, const Int z = 0) : x(x), y(y), z(z) {}
};

/*****************************
 * Position (Long)           *
 * The position of an object *
 *****************************/
class PositionL
{
public:
	Long x;
	Long y;
	Long z;
	PositionL(const Long x = 0, const Long y = 0, const Long z = 0) : x(x), y(y), z(z) {}
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
	PositionF(const Double x = 0.0, const Double y = 0.0, const Double z = 0.0) : x(x), y(y), z(z) {}
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
	SerialPosition(Long data = 0) : data(data) {}
	SerialPosition(Int x, Int y, Int z) : data(((Long)(x & 0x3FFFFFF) << 38) | ((Long)(y & 0xFFF) << 26) | (Long)(z & 0x3FFFFFF)) {}
	SerialPosition(Position pos) : SerialPosition(pos.x, pos.y, pos.z) {}
	const Int getX() const { return data >> 38; }
	const Int getY() const { return (data >> 26) & 0xFFF; }
	const Int getZ() const { return data << 38 >> 38; }
	const Long makeData() const { return data; }
	const Position toPosition() const { return Position(getX(), getY(), getZ()); }
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
