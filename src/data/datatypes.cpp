#include "datatypes.h"
#include <stdexcept>

/*******************************************
 * ParseNumVar                             *
 * Reads a variable of length x and throws *
 * an exception when that length is passed *
 *******************************************/
Long parseNumVar(const Byte* data, const Byte maxlength)
{
	// Check for null data
	if (data == NULL)
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
			throw new std::overflow_error("VarNum is too big");
	} while ((read & 0x80) != 0); // Stop when we hit a certain number

	return result;
}

/*******************************************************
 * MakeNumVar                                          *
 * Takes a long and creates a variable of length x     *
 * It throws an exception if the new value is too long *
 *******************************************************/
VarNum& makeNumVar(Long value, const Byte maxlength)
{
	// Allocate some memory for the value
	Byte* data = new Byte[maxlength];
	Byte numWrote = 0;

	do
	{
		// Calculate a byte
		Byte temp = (Byte)(value & 0x7f);
		value = (Long)(((ULong)value) >> 7); // Logical right-shift
		if (value != 0)
			temp |= 0x80;

		// Write the calculated byte
		data[numWrote++] = temp;

		// Check if we exceeded the array
		if (numWrote >= maxlength)
			throw new std::overflow_error("VarNum is too big");

		// Write the null byte at the end
//		data[numWrote] = 0;
	} while (value != 0);

	return *(new VarNum(data));
}

/*******************************************************
 * MakeNumVar                                          *
 * Takes an int and creates a variable of length x     *
 * It throws an exception if the new value is too long *
 *******************************************************/
VarNum& makeNumVar(Int value, const Byte maxlength)
{
	// Allocate some memory for the value
	Byte* data = new Byte[maxlength];
	Byte numWrote = 0;

	do
	{
		// Calculate a byte
		Byte temp = (Byte)(value & 0x7f);
		value = (Int)(((UInt)value) >> 7); // Logical right-shift
		if (value != 0)
			temp |= 0x80;

		// Write the calculated byte
		data[numWrote++] = temp;

		// Check if we exceeded the array
		if (numWrote >= maxlength)
			throw new std::overflow_error("VarInt is too big");

		// Write the null byte at the end
		data[numWrote] = 0;
	} while (value != 0);

	return *(new VarNum(data));
}

/******************************************
 * CountNumVarLength                      *
 * Reads a variable and counts its length *
 ******************************************/
Byte CountNumVarLength(const Byte* data, const Byte maxlength)
{
	// Check for null data
	if (data == NULL)
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

/***********************
 * VarNum :: VarNum    *
 * Default Constructor *
 ***********************/
VarNum::VarNum(Byte* data) : data(data) { length = CountNumVarLength(data, VARNUM_MAX_SIZE); }

/*********************
 * VarNum :: VarNum  *
 * Copy Constructor  *
 *********************/
VarNum::VarNum(const VarNum& value)
{
	// If the other value has no data then forget about copying
	if (value.data == 0)
	{
		length = 0;
		data = NULL;
		return;
	}

	// Copy the data from the value to the new long
	length = value.length;
	data = new Byte[length];
	for (int i = 0; i < length; i++)
		data[i] = value.data[i];
}

/**************************
* VarNum :: VarNum       *
* Constructor from a Num *
**************************/
VarNum::VarNum(const Long value) { *this = makeNumVar(value, VARNUM_MAX_SIZE); }

/**************************
 * VarNum :: VarNum       *
 * Constructor from a Num *
 **************************/
VarNum::VarNum(const Int value) { *this = makeNumVar(value, VARNUM_MAX_SIZE); }

/*********************
 * VarNum :: ~VarNum *
 * Destructor        *
 *********************/
VarNum::~VarNum() { if (data != NULL) { delete[] data; data = NULL; } }

/**************************
 * VarNum :: toNum        *
 * Converts VarNum to Num *
 **************************/
Long VarNum::toNum() const { return parseNumVar(data, VARNUM_MAX_SIZE); }

/*****************************
 * VarNum :: getSize         *
 * Returns the size in bytes *
 *****************************/
const Byte VarNum::getSize() const { return length; }

/**************************
 * VarNum :: getData      *
 * Returns the data array *
 **************************/
const Byte* VarNum::getData() const { return data; }

/***********************
 * VarInt :: VarInt    *
 * Default Constructor *
 ***********************/
VarInt::VarInt(Byte* data) : VarNum(data)
{
	if (length > VARINT_MAX_SIZE)
		throw new std::overflow_error("VarInt is too big");
}

/*********************
 * VarInt :: VarInt  *
 * Copy Constructor  *
 *********************/
VarInt::VarInt(const VarInt& value) : VarNum(value)
{
	if (length > VARINT_MAX_SIZE)
		throw new std::overflow_error("VarInt is too big");
}

/************************
 * VarInt :: VarInt     *
 * Constructor from Int *
 ************************/
VarInt::VarInt(const Int value) : VarNum((const Int)value)
{
	if (length > VARINT_MAX_SIZE)
		throw new std::overflow_error("VarInt is too big");
}

/****************************
 * VarInt :: VarInt         *
 * Constructor from VarLong *
 ****************************/
VarInt::VarInt(const VarLong& value) : VarNum(value)
{
	if (length > VARINT_MAX_SIZE)
		throw new std::overflow_error("VarInt is too big");
}

/*********************
 * VarInt :: ~VarInt *
 * Destructor        *
 *********************/
VarInt::~VarInt() {}

/**************************
 * VarInt :: toInt        *
 * Converts VarInt to Int *
 **************************/
Int VarInt::toInt() const { return (Int)toNum(); }

/***********************
 * VarLong :: VarLong  *
 * Default Constructor *
 ***********************/
VarLong::VarLong(Byte* data) : VarNum(data)
{
	if (length > VARLONG_MAX_SIZE)
		throw new std::overflow_error("VarLong is too big");
}

/***********************
 * VarLong :: VarLong  *
 * Copy Constructor    *
 ***********************/
VarLong::VarLong(const VarLong& value) : VarNum(value)
{
	if (length > VARLONG_MAX_SIZE)
		throw new std::overflow_error("VarLong is too big");
}

/*************************
 * VarLong :: VarLong    *
 * Constructor from Long *
 *************************/
VarLong::VarLong(const Long value) : VarNum((const Long)value)
{
	if (length > VARLONG_MAX_SIZE)
		throw new std::overflow_error("VarLong is too big");
}

/***************************
 * VarLong :: VarLong      *
 * Constructor from VarInt *
 ***************************/
VarLong::VarLong(const VarInt& value) : VarNum(value)
{
	if (length > VARLONG_MAX_SIZE)
		throw new std::overflow_error("VarLong is too big");
}

/***********************
 * VarLong :: ~VarLong *
 * Destructor          *
 ***********************/
VarLong::~VarLong() {}

/****************************
 * VarLong :: toLong        *
 * Converts VarLong to Long *
 ****************************/
Long VarLong::toLong() const { return toNum(); }

/*********************************
 * SerialString :: SerialString  *
 * Default Constructor           *
 *********************************/
SerialString::SerialString() : length(0), data(0) {}

/*********************************
 * SerialString :: SerialString  *
 * Constructor from UTF-8 String *
 *********************************/
SerialString::SerialString(String text)
{
	// TODO: Create the SerialString here
}

/*********************************
 * SerialString :: ~SerialString *
 * Destructor                    *
 *********************************/
SerialString::~SerialString() { if (data != NULL) { delete[] data; data = NULL; } }

/***********************
 * Chat :: Chat        *
 * Default Constructor *
 ***********************/
Chat::Chat() : SerialString() {}

/*********************************
 * Chat :: Chat                  *
 * Constructor from UTF-8 String *
 *********************************/
Chat::Chat(String text) : SerialString()
{
	// TODO: Create the Chat here
}

/*****************
 * Chat :: ~Chat *
 * Destructor    *
 *****************/
Chat::~Chat() { if (data != NULL) { delete[] data; data = NULL; } }

/************************
 * Position :: Position *
 * Default Constructor  *
 ************************/
Position::Position() : data(0) {}

/****************************
 * Position :: Position     *
 * Constructor from 3 Ints  *
 ****************************/
Position::Position(Int x, Int y, Int z) 
{
	data = ((Long)(x & 0x3FFFFFF) << 38) | ((Long)(y & 0xFFF) << 26) | (Long)(z & 0x3FFFFFF);
}

/*************************
 * Position :: ~Position *
 * Destructor            *
 *************************/
Position::~Position() {}

/************************
 * Position :: getX     *
 * Get the X coordinate *
 ************************/
const Int Position::getX() const { return data >> 38; }

/************************
 * Position :: getY     *
 * Get the Y coordinate *
 ************************/
const Int Position::getY() const { return (data >> 26) & 0xFFF; }

/************************
 * Position :: getZ     *
 * Get the Z coordinate *
************************/
const Int Position::getZ() const { return data << 38 >> 38; }

/***********************
 * Angle :: Angle      *
 * Default Constructor *
 ***********************/
Angle::Angle() {}

/*************************
 * Angle :: Angle        *
 * Constructor from Byte *
 *************************/
Angle::Angle(Byte angle) : angle(angle) {}

/*******************
 * Angle :: ~Angle *
 * Destructor      *
 *******************/
Angle::~Angle() {}

/***********************
 * UUID :: UUID        *
 * Default Constructor *
 ***********************/
UUID::UUID() {}

/******************************
 * UUID :: UUID               *
 * Constructor from two Longs *
 ******************************/
UUID::UUID(Long value1, Long value2) : v1(value1), v2(value2) {}

/*******************
 * UUID :: ~UUID   *
 * Destructor      *
 *******************/
UUID::~UUID() {}