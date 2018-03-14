#include "debug.h"
#include "data/bitstream.h"

#define BYTE_HIGH_BIT 7
#define BITS_IN_BYTE 8
#define BYTES_IN_SHORT 2
#define BYTES_IN_INT 4
#define BYTES_IN_LONG 8

/* Default Constructor */
BitStream::BitStream() : data(NULL), nextBit(BITS_IN_BYTE)
{
	data = new String();
	data->append(1, '\0');
}

/* Move Constructor */
BitStream::BitStream(BitStream&& stream) : data(NULL), nextBit(BITS_IN_BYTE)
{
	data = stream.data;
	nextBit = stream.nextBit;
	stream.data = new String();
	stream.data->append(1, '\0');
	stream.nextBit = BITS_IN_BYTE;
}

/***************************
 * Bit Stream :: Push Back *
 * Inserts a boolean       *
 ***************************/
void BitStream::push_back(Boolean num)
{
	// If the byte has been filled up, reset the bit counter and create a new byte
	if (--nextBit < 0)
	{
		nextBit = BYTE_HIGH_BIT;
		data->append(1, '\0');
	}

	// Append a bit to the end of the stream
	data->back() |= ((char)num << nextBit);
}

/**********************************************
 * Bit Stream :: Push Back                    *
 * Inserts a long with the given bitness.     *
 * A bits value that is greater than 64 or    *
 * lower than 0 will cause undefined behavior *
 **********************************************/
void BitStream::push_back(Long num, Byte bits)
{
	// Insert every bit in the number into the bit stream
	for (int i = bits - 1; i >= 0; --i)
		push_back((Boolean)(num >> i & 1));
}

/*************************
 * Bit Stream :: Clear   *
 * Clears the bit stream *
 *************************/
void BitStream::clear()
{
	// Reset the data
	data->clear();
	data->append(1, '\0');
	nextBit = BITS_IN_BYTE;
}

/*************************************
 * Bit Stream :: Extraction Operator *
 * Extracts a byte from the stream   *
 *************************************/
BitStream& BitStream::operator>>(Byte& num)
{
	// Pop the first byte off of the string and store its value
	num = data->front();
	data->erase(0, 1);

	// If the data is empty then place a null character in
	if (data->empty())
	{
		data->append(1, '\0');
		nextBit = BITS_IN_BYTE;
	}

	return *this;
}

/*************************************
 * Bit Stream :: Extraction Operator *
 * Extracts a short from the stream  *
 *************************************/
BitStream& BitStream::operator>>(Short& num)
{
	// Grab the bytes individually
	Byte bytes[BYTES_IN_SHORT];
	for (int i = 0; i < BYTES_IN_SHORT; ++i)
		*this >> bytes[i];

	// Store the bytes in the number
	for (int i = 0, num = 0; i < BYTES_IN_SHORT; ++i)
		num |= bytes[i] << (i * BITS_IN_BYTE);

	return *this;
}

/*************************************
 * Bit Stream :: Extraction Operator *
 * Extracts an int from the stream   *
 *************************************/
BitStream& BitStream::operator>>(Int& num)
{
	// Grab the bytes individually
	Byte bytes[BYTES_IN_INT];
	for (int i = 0; i < BYTES_IN_INT; ++i)
		*this >> bytes[i];

	// Store the bytes in the number
	for (int i = 0, num = 0; i < BYTES_IN_INT; ++i)
		num |= bytes[i] << (i * BITS_IN_BYTE);

	return *this;
}

/*************************************
 * Bit Stream :: Extraction Operator *
 * Extracts a long from the stream   *
 *************************************/
BitStream& BitStream::operator>>(Long& num)
{
	// Grab the bytes individually
	Byte bytes[BYTES_IN_LONG];
	for (int i = 0; i < BYTES_IN_LONG; ++i)
		*this >> bytes[i];

	// Store the bytes in the number
	for (int i = 0, num = 0; i < BYTES_IN_LONG; ++i)
		num |= bytes[i] << (i * BITS_IN_BYTE);

	return *this;
}
