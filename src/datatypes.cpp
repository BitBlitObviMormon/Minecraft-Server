#include "datatypes.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>

auto constexpr SEGMENT_BITS = 0x7f;
auto constexpr CONTINUE_BIT = 0x80;
#define STR(x) #x

/****************************************
 * VarInt :: parseVarInt                *
 * Reads a VarInt and captures its data *
 ****************************************/
Int VarInt::parseVarInt(const Byte* data, const Boolean copyData) // data can be a nullptr
{
	if (data == nullptr) {
		if (copyData) {
			this->length = 1;
			this->data[0] = 0;
		}

		return 0;
	}
	else {
		Int value = 0;
		Int position = 0;
		Byte i = 0;
		Byte currentByte;

		while (true) {
			if (copyData) this->data[i] = data[i];
			currentByte = data[i++];
			value |= (currentByte & SEGMENT_BITS) << position;

			if ((currentByte & CONTINUE_BIT) == 0) break;

			position += 7;

			if (position >= sizeof(Int) * 8) throw new std::runtime_error("VarInt is too big");
		}

		if (copyData) this->length = i;

		return value;
	}
}

/***********************
 * VarInt :: VarInt    *
 * Default Constructor *
 ***********************/
VarInt::VarInt(const Byte* data) // data can be a nullptr
{
	parseVarInt(data, true);
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
	Int temp = value;
	length = 0;
	while (true) {
		if ((temp & ~SEGMENT_BITS) == 0) {
			data[length++] = (Byte)temp;
			return;
		}

		data[length++] = ((temp & SEGMENT_BITS) | CONTINUE_BIT);
		temp = (Int)(((UInt)temp) >> 7); // Logical right-shift
	}
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
Int VarInt::toInt() const { return ((VarInt*)this)->parseVarInt(data, false); }

/******************************************
 * VarLong :: parseVarLong                *
 * Reads a VarLong and captures its data  *
 ******************************************/
Long VarLong::parseVarLong(const Byte* data, const Boolean copyData) // data can be a nullptr
{
	if (data == nullptr) {
		if (copyData) {
			this->length = 1;
			this->data[0] = 0;
		}

		return 0;
	}
	else {
		Long value = 0;
		Long position = 0;
		Byte i = 0;
		Byte currentByte;

		while (true) {
			if (copyData) this->data[i] = data[i];
			currentByte = data[i++];
			value |= ((Long)(currentByte & SEGMENT_BITS)) << position;

			if ((currentByte & CONTINUE_BIT) == 0) break;

			position += 7;

			if (position >= sizeof(Long) * 8) throw new std::runtime_error("VarLong is too big");
		}

		if (copyData) this->length = i;

		return value;
	}
}

/***********************
 * VarLong :: VarLong  *
 * Default Constructor *
 ***********************/
VarLong::VarLong(const Byte* data) // data can be a nullptr
{
	parseVarLong(data, true);
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
	Long temp = value;
	length = 0;
	while (true) {
		if ((temp & ~((Long)SEGMENT_BITS)) == 0) {
			data[length++] = (Byte)temp;
			return;
		}

		data[length++] = ((temp & SEGMENT_BITS) | CONTINUE_BIT);
		temp = (Long)(((ULong)temp) >> 7); // Logical right-shift
	}
}

/****************************
 * VarLong :: operator=     *
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
Long VarLong::toLong() const { return ((VarLong*)this)->parseVarLong(data, false);
}

/*****************************
 * SerialChat :: SerialChat  *
 * Default Constructor       *
 *****************************/
SerialChat::SerialChat(const Byte* data)
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

	checkSize();
}

/***************************************
 * SerialChat :: makeData              *
 * Creates an array of serialized data *
 ***************************************/
std::unique_ptr<Byte[]> SerialChat::makeData() const
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

/**************************************************
 * SerialChat :: checkSize                        *
 * Throws an exception if the string is too large *
 **************************************************/
void SerialChat::checkSize() const {
	if (data.size() > SERIALCHAT_MAX_LENGTH)
		throw std::length_error("SerialChat exceeds " STR(SERIALCHAT_MAX_LENGTH) " bytes.");
}

/**************************************************
 * SerialString :: checkSize                      *
 * Throws an exception if the string is too large *
 **************************************************/
void SerialString::checkSize() const {
	if (data.size() > SERIALSTRING_MAX_LENGTH)
		throw std::length_error("SerialString exceeds " STR(SERIALSTRING_MAX_LENGTH) " bytes.");
}

Position::operator PositionF() const { return PositionF(x, y, z); }
Position::operator PositionL() const { return PositionL(x, y, z); }
Position::operator SerialPosition() const { return SerialPosition(x, y, z); }
PositionL::operator PositionF() const { return PositionF(x, y, z); }
PositionL::operator Position() const { return Position(x, y, z); }
PositionL::operator SerialPosition() const { return SerialPosition(x, y, z); }
PositionF::operator Position() const { return Position(x, y, z); }
PositionF::operator PositionL() const { return PositionL(x, y, z); }
PositionF::operator SerialPosition() const { return SerialPosition(x, y, z); }

/****************************************************
 * SerialPosition :: SerialPosition                 *
 * Throws an exception if any value is out of range *
 ****************************************************/
void SerialPosition::checkData(const Int x, const Int y, const Int z) const {
	if (x > SERIALPOSITION_X_MAX) throw std::overflow_error("X value in SerialPosition exceeds " STR(SERIALPOSITION_X_MAX) ".");
	else if (x < SERIALPOSITION_X_MIN) throw std::underflow_error("X value in SerialPosition exceeds " STR(SERIALPOSITION_X_MIN) ".");
	else if (y > SERIALPOSITION_Y_MAX) throw std::overflow_error("Y value in SerialPosition exceeds " STR(SERIALPOSITION_Y_MAX) ".");
	else if (y < SERIALPOSITION_Y_MIN) throw std::underflow_error("Y value in SerialPosition exceeds " STR(SERIALPOSITION_Y_MIN) ".");
	else if (z > SERIALPOSITION_Z_MAX) throw std::overflow_error("Z value in SerialPosition exceeds " STR(SERIALPOSITION_Z_MAX) ".");
	else if (z < SERIALPOSITION_Z_MIN) throw std::underflow_error("Z value in SerialPosition exceeds " STR(SERIALPOSITION_Z_MIN) ".");
}

Long SerialPosition::toData(const Int x, const Int y, const Int z) const {
	return ((ULong)(x & 0x3FFFFFF) << 38) | ((ULong)(z & 0x3FFFFFF) << 12) | ((ULong)y & 0xFFF);
}

const Int SerialPosition::getX() const {
	Int retVal = data >> 38;
	return retVal >= -SERIALPOSITION_X_MIN ? retVal - (-SERIALPOSITION_X_MIN << 1) : retVal;
}

const Int SerialPosition::getY() const {
	Int retVal = data & 0xFFF;
	return retVal >= -SERIALPOSITION_Y_MIN ? retVal - (-SERIALPOSITION_Y_MIN << 1) : retVal;
}

const Int SerialPosition::getZ() const {
	Int retVal = (data >> 12) & 0x3FFFFFF;
	return retVal >= -SERIALPOSITION_Z_MIN ? retVal - (-SERIALPOSITION_Z_MIN << 1) : retVal;
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
