#include "data/datatypes.h"

/************************************************
 * Bit Stream                                   *
 * Inserts and extracts bits from a data stream *
 ************************************************/
class BitStream
{
protected:
	String* data;
	Byte nextBit;
public:
	explicit BitStream();
	BitStream(BitStream&& stream);
	~BitStream() { delete data; }
	void push_back(Boolean num);
	void push_back(Long num, Byte bits = 64);
	void push_back(Int num, Byte bits = 32)   { push_back((Long)num, bits); }
	void push_back(Short num, Byte bits = 16) { push_back((Long)num, bits); }
	void push_back(Byte num, Byte bits = 8)   { push_back((Long)num, bits); }
	void clear();
	String& str() { return *data; }
	BitStream& operator<<(Boolean num) { push_back(num); return *this; }
	BitStream& operator<<(Long num)    { push_back(num); return *this; }
	BitStream& operator<<(Int num)     { push_back(num); return *this; }
	BitStream& operator<<(Short num)   { push_back(num); return *this; }
	BitStream& operator<<(Byte num)    { push_back(num); return *this; }
	BitStream& operator>>(Byte& num);
	BitStream& operator>>(Short& num);
	BitStream& operator>>(Int& num);
	BitStream& operator>>(Long& num);
};
