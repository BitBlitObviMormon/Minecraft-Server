#include "datatypes.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>

/******************************************
 * CountVarNumLength                      *
 * Reads a variable and counts its length *
 ******************************************/
Byte countVarNumLength(const Byte* data, const Byte maxlength)
{
	// Check for null data
	if (data == nullptr)
		return 0;

	// Parse the data byte by byte
	Int numRead = 0;
	Byte read;
	do
	{
		// Read a byte of data
		read = data[numRead];
		numRead++;

		// If we read too much data then throw an exception
		if (numRead > maxlength)
			throw new std::overflow_error("VarNum is too big");
	} while ((read & 0x80) != 0); // Stop when we hit a certain number

	return numRead;
}

/***************************************************
 * ParseVarNum                                     *
 * Returns the length of a VarNum's data.          *
 * Returns -1 if the length exceeds the max given. *
 ***************************************************/
Long parseVarNum(const Byte* data, const Byte maxlength)
{
	// Check for null data
	if (data == nullptr)
		return 0;

	// Parse the data byte by byte
	Int numRead = 0;
	Long result = 0;
	Byte read;
	do
	{
		// Read a byte of data
		read = data[numRead];
		result |= ((read & 0x7f) << (7 * numRead));
		numRead++;

		// If we read too much data then throw an exception
		if (numRead > maxlength)
			return -1;
	} while ((read & 0x80) != 0); // Stop when we hit a certain number

	return result;
}

/***********************
 * VarInt :: VarInt    *
 * Default Constructor *
 ***********************/
VarInt::VarInt(const Byte* data) // data can be a nullptr
{
	length = countVarNumLength(data, VARINT_MAX_SIZE);
	if (length == VARNUM_PARSE_ERROR)
		throw new std::overflow_error("VarInt is too big");
	else if (length > 0)
		std::copy_n(data, VARINT_MAX_SIZE, this->data);
	else { // Make value zero for nullptr
		this->data[0] = '\0';
		length = 1;
	}
}

/*********************
 * VarInt :: VarInt  *
 * Copy Constructor  *
 *********************/
VarInt::VarInt(const VarInt& value)
{
	std::copy_n(value.data, VARINT_MAX_SIZE, this->data);
	this->length = value.length;
}

/************************
 * VarInt :: VarInt     *
 * Constructor from Int *
 ************************/
VarInt::VarInt(const Int value)
{
	Byte numWrote = 0;
	Int copyVal = value;
	do
	{
		// Calculate a byte
		Byte temp = (Byte)(value & 0x7f);
		copyVal = (Int)(((UInt)value) >> 7); // Logical right-shift
		if (copyVal != 0)
			temp |= 0x80;

		// Write the calculated byte
		data[numWrote++] = temp;

		// Check if we exceeded the array
		if (numWrote >= VARINT_MAX_SIZE)
			throw new std::overflow_error("VarInt is too big");
		else // Write the null byte at the end
			data[numWrote] = 0;

	} while (value != 0);

	// Assemble and return the VarNum
	length = numWrote;
}

/****************************
 * VarInt :: operator=      *
 * Copy Assignment Operator *
 ****************************/
VarInt& VarInt::operator=(const VarInt& rhs)
{
	std::copy_n(rhs.data, VARINT_MAX_SIZE, this->data);
	this->length = rhs.length;

	return *this;
}

/**************************
 * VarInt :: toInt        *
 * Converts VarInt to Int *
 **************************/
Int VarInt::toInt() const { return (Int)parseVarNum(data, VARINT_MAX_SIZE); }

/***********************
 * VarLong :: VarLong  *
 * Default Constructor *
 ***********************/
VarLong::VarLong(const Byte* data) // data can be a nullptr
{
	length = countVarNumLength(data, VARLONG_MAX_SIZE);
	if (length == VARNUM_PARSE_ERROR)
		throw new std::overflow_error("VarLong is too big");
	else if (length > 0)
		std::copy_n(data, VARLONG_MAX_SIZE, this->data);
	else { // Make value zero for nullptr
		this->data[0] = '\0';
		length = 1;
	}
}

/**********************
 * VarLong :: VarLong *
 * Copy Constructor   *
 **********************/
VarLong::VarLong(const VarLong& value)
{
	std::copy_n(value.data, VARLONG_MAX_SIZE, this->data);
	this->length = value.length;
}

/*************************
 * VarLong :: VarLong    *
 * Constructor from Long *
 *************************/
VarLong::VarLong(const Long value)
{
	Byte numWrote = 0;
	Long copyVal = value;
	do
	{
		// Calculate a byte
		Byte temp = (Byte)(value & 0x7f);
		copyVal = (Long)(((ULong)value) >> 7); // Logical right-shift
		if (copyVal != 0)
			temp |= 0x80;

		// Write the calculated byte
		data[numWrote++] = temp;

		// Check if we exceeded the array
		if (numWrote >= VARLONG_MAX_SIZE)
			throw new std::overflow_error("VarLong is too big");
		else // Write the null byte at the end
			data[numWrote] = 0;

	} while (value != 0);

	// Assemble and return the VarNum
	length = numWrote;
}

/****************************
 * VarLong :: operator=      *
 * Copy Assignment Operator *
 ****************************/
VarLong& VarLong::operator=(const VarLong& rhs)
{
	std::copy_n(rhs.data, VARLONG_MAX_SIZE, this->data);
	this->length = rhs.length;

	return *this;
}

/****************************
 * VarLong :: toLong        *
 * Converts VarLong to Long *
 ****************************/
Long VarLong::toLong() const { return parseVarNum(data, VARLONG_MAX_SIZE); }

/*********************************
 * SerialString :: SerialString  *
 * Default Constructor           *
 *********************************/
SerialString::SerialString(const Byte* data)
{
	if (data != nullptr)
	{
		// Get the length of the string
		lenInt = VarInt(data);
		data += lenInt.size();

		// Copy the data to the string
		Int len = lenInt.toInt();
		this->data.reserve(len);
		this->data.append((const char*)data, len);
	}
}

/***************************************
 * SerialString :: makeData            *
 * Creates an array of serialized data *
 ***************************************/
std::unique_ptr<Byte[]> SerialString::makeData() const
{
	// Create a buffer to copy the contents onto
	Int totalLength = size();
	Byte* base = new Byte[totalLength];
	Byte* ptr = base;

	// Copy the VarInt and string (without the null terminator) to the buffer
	std::copy_n(lenInt.makeData(), lenInt.size(), ptr);
	std::copy(data.begin(), data.end(), ptr + lenInt.size());

	// Return ownership to the data
	return std::unique_ptr<Byte[]>(base);
}

/*****************************************************************
 * ChunkSection :: setBlock                                      *
 * Sets the block at the specified index to the specified id     *
 * Any block id that is < 0 or > 4095 causes undefined behavior! *
 *****************************************************************/
void ChunkSection::setBlock(int index, BlockID blockid)
{
	// Create a new array when necessary
	if (!blocks)
		fillBlocks();

	// Set the block to the new block and the current block state
	blocks[index] = (blocks[index] & 0xF) | ((Short)blockid << 4);
}

/*****************************************************************************
 * ChunkSection :: setBlock                                                  *
 * Sets the block at the specified index to the specified id and block state *
 * Any block id that is < 0 or > 4095 causes undefined behavior!             *
 * Any block state that is < 0 or > 15 causes undefined behavior!            *
 *****************************************************************************/
void ChunkSection::setBlock(int index, BlockID blockid, Byte blockstate)
{
	// Create a new array when necessary
	if (!blocks)
		fillBlocks();

	// Set the block to the new block and the current block state
	blocks[index] = ((Short)blockid << 4) | blockstate;
}

/*******************************************************************
 * ChunkSection :: setBlockState                                   *
 * Sets the block state at the specified index to the specified id *
 * Any block state that is < 0 or > 15 causes undefined behavior!  *
 *******************************************************************/
void ChunkSection::setBlockState(int index, Byte blockstate)
{
	// Create a new array when necessary
	if (!blocks)
		fillBlocks();

	// Set the block to the current block with the new block state
	blocks[index] = (blocks[index] & 0xFFF0) | blockstate;
}

/*********************************************************************
 * ChunkSection :: setBlockLighting                                  *
 * Sets the block lighting at the specified index to the given value *
 * Any value that is < 0 or > 15 causes undefined behavior!          *
 *********************************************************************/
void ChunkSection::setBlockLighting(int index, Byte value)
{
	// Create a new array when necessary
	if (!lights)
		fillLighting();

	// Set the lighting to the new block lighting and the current sky lighting
	lights[index] &= 0xF0;
	lights[index] |= value << 4;
}

/*******************************************************************
 * ChunkSection :: setSkyLighting                                  *
 * Sets the sky lighting at the specified index to the given value *
 * Any value that is < 0 or > 15 causes undefined behavior!        *
 *******************************************************************/
void ChunkSection::setSkyLighting(int index, Byte value)
{
	// Create a new array when necessary
	if (!lights)
		fillLighting();

	// Set the lighting to the current block lighting and the new sky lighting
	lights[index] &= 0xF;
	lights[index] |= value;
}

/****************************************************************
 * ChunkSection :: setLighting                                  *
 * Sets the lighting at the specified index to the given values *
 * Any value that is < 0 or > 15 causes undefined behavior!     *
 ****************************************************************/
void ChunkSection::setLighting(int index, Byte blockLightValue, Byte skyLightValue)
{
	// Create a new array when necessary
	if (!lights)
		fillLighting();

	// Set the lighting to the current block lighting and the new sky lighting
	lights[index] = (blockLightValue << 4) | skyLightValue;
}

/******************************************************************
 * ChunkSection :: fillBlocks                                     *
 * Fills the chunk with the given block and block state           *
 * Any block id that is < 0 or > 4095 causes undefined behavior!  *
 * Any block state that is < 0 or > 15 causes undefined behavior! *
 ******************************************************************/
void ChunkSection::fillBlocks(BlockID blockid, Byte blockstate)
{
	// Create a new array when necessary
	if (!blocks)
		blocks = new Short[4096];

	Short value = ((Short)blockid << 4) | blockstate;

	// Set every block to the given block state and id
	for (int i = 0; i < 4096; i++)
		blocks[i] = value;
}

/************************************************************
 * ChunkSection :: fillLighting                             *
 * Sets all of the lighting to the given values             *
 * Any value that is < 0 or > 15 causes undefined behavior! *
 ************************************************************/
void ChunkSection::fillLighting(Byte blockLightValue, Byte skyLightValue)
{
	// Create a new array when necessary
	if (!lights)
		lights = new Byte[4096];

	Byte value = (blockLightValue << 4) | skyLightValue;

	// Set all of the lighting to the given block and sky lighting values
	for (int i = 0; i < 4096; i++)
		lights[i] = value;
}
