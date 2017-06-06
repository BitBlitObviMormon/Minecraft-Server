#include "datatypes.h"
#include "../client/client.h"
#include <stdexcept>
#include <iostream>

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
VarNum::VarNum(Byte* data) : data(NULL)
{
	length = CountNumVarLength(data, VARNUM_MAX_SIZE);

	// Copy the array given
	Byte* newData = new Byte[length];
	for (int i = 0; i < length; i++)
		newData[i] = data[i];
	this->data = newData;
}

/*********************
 * VarNum :: VarNum  *
 * Copy Constructor  *
 *********************/
VarNum::VarNum(const VarNum& value)
{
	// If the other value has no data then forget about copying
	if (value.data == NULL)
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
Long VarNum::toNum() const { return parseNumVar(data, length); }

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

/****************************
 * VarInt :: operator=      *
 * Copy Assignment Operator *
 ****************************/
VarInt& VarInt::operator=(const VarInt& right)
{
	// If the other value has no data then forget about copying
	if (right.data == NULL)
	{
		length = 0;
		data = NULL;
		return *this;
	}

	// Copy the data from the value to the new long
	length = right.length;
	data = new Byte[length];
	for (int i = 0; i < length; i++)
		data[i] = right.data[i];

	return *this;
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
 * VarLong :: operator=     *
 * Copy Assignment Operator *
 ****************************/
VarLong& VarLong::operator=(const VarLong& right)
{
	// If the other value has no data then forget about copying
	if (right.data == NULL)
	{
		length = 0;
		data = NULL;
		return *this;
	}

	// Copy the data from the value to the new long
	length = right.length;
	data = new Byte[length];
	for (int i = 0; i < length; i++)
		data[i] = right.data[i];

	return *this;
}

/****************************
 * VarLong :: toLong        *
 * Converts VarLong to Long *
 ****************************/
Long VarLong::toLong() const { return toNum(); }

/*********************************
 * SerialString :: SerialString  *
 * Default Constructor           *
 *********************************/
SerialString::SerialString(Byte* data) : length(0), data(NULL)
{
	if (data != NULL)
	{
		// Get the length of the string
		VarInt data1 = VarInt(data);
		length = data1;
		data += length.getSize();

		// Copy the string to a new buffer
		Int len = length.toInt();
		this->data = new Byte[len];
		for (int i = 0; i < len; i++)
			this->data[i] = data[i];
	}
}

/*********************************
 * SerialString :: SerialString  *
 * Constructor from UTF-8 String *
 *********************************/
SerialString::SerialString(const String& text)
{
	length = VarInt(text.length());
	data = new Byte[text.length()];

	// Copy the data from the string to this one
	for (UInt i = 0; i < text.length(); i++)
		data[i] = text[i];
}

/*********************************
 * SerialString :: ~SerialString *
 * Destructor                    *
 *********************************/
SerialString::~SerialString() { if (data != NULL) { delete[] data; data = NULL; } }

/*********************************
 * SerialString :: getLength     *
 * Returns the size of the array *
 *********************************/
const Int SerialString::getLength() const { return length.toInt(); }

/********************************
 * SerialString :: getSize      *
 * Returns the size of the data *
 ********************************/
const Int SerialString::getSize() const { return length.toInt() + length.getSize(); }

/*******************************
 * SerialString :: getData     *
 * Creates an array of data    *
 * Has to be deleted after use *
 *******************************/
char* SerialString::makeData() const
{
	// Get the dimensions of the data and create an array to store it
	Int end1 = length.getSize();
	Int end2 = getSize();
	const Byte* lengthData = length.getData();
	char* moreData = new char[end2];

	// Copy the contents of the length to the new array
	int i;
	for (i = 0; i < end1; i++)
		moreData[i] = lengthData[i];

	// Copy the contents of the data to the new array
	for (int y = 0; i < end2; y++, i++)
		moreData[i] = data[y];

	return moreData;
}

/***********************
 * SerialString :: str *
 * Returns a string    *
 ***********************/
const String SerialString::str() const
{
	return String((const char*)data, length.toInt());
}

/***********************
 * Chat :: Chat        *
 * Default Constructor *
 ***********************/
Chat::Chat(Byte* data) : SerialString(data) {}

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

/************************
 * Position :: getdata  *
 * Returns the raw data *
 ************************/
const Long Position::getData() const { return data; }

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
UUID::UUID(Long value1, Long value2) : v1(value1), v2(value2) {}

/**************************
 * UUID :: UUID           *
 * Generate from a client *
 **************************/
UUID::UUID(Client* client) { recalculate(client); }

/*******************
 * UUID :: ~UUID   *
 * Destructor      *
 *******************/
UUID::~UUID() {}

/**************************
 * UUID :: recalculate    *
 * Generate from a client *
 **************************/
void UUID::recalculate(Client* client)
{
	// TODO: Generate a UUID
	v1 = 0;
	v2 = 0;
}

/************************
* UUID :: ~UUID         *
* Returns a UUID String *
*************************/
const String UUID::str()
{
	// TODO: Generate UUID Strings
	return "00000000-0000-3000-0000-000000000000";
}
