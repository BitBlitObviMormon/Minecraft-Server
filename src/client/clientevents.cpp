#include "debug.h"
#include "client/clientevents.h"

DisplayedSkinParts::DisplayedSkinParts(Byte byte)
{
	cape		= (byte & 0x01) != 0;
	jacket		= (byte & 0x02) != 0;
	leftSleeve	= (byte & 0x04) != 0;
	rightSleeve	= (byte & 0x08) != 0;
	leftPants	= (byte & 0x10) != 0;
	rightPants	= (byte & 0x20) != 0;
	hat			= (byte & 0x40) != 0;
	unusedBit	= (byte & 0x80) != 0;
}
