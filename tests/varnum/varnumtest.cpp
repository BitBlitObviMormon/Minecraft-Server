#include "varnumtest.h"
#include "datatypes.h"
#include <assert.h>

/*************************************************************
 * VAR NUM TEST                                              *
 *************************************************************/
using namespace std;
void VarNumTest() {
	// Create the integer limits
	cout << "VarInt / VarLong Tests\n\tInteger limits\n";
	Int maxint = 0x7FFFFFFF;
	Int minint = -(Int)0x80000000;
	Long maxlong = 0x7FFFFFFFFFFFFFFF;
	Long minlong = -(Long)0x8000000000000000;
	Int null = 0;
	VarInt maxvint(maxint);
	VarInt minvint(minint);
	VarInt nullvint(null);
	VarLong maxvong(maxlong);
	VarLong minvong(minlong);
	VarLong nullvong(null);

	// Test the sizes
	assert(maxvint.size() == VARINT_MAX_SIZE);
	assert(minvint.size() == VARINT_MAX_SIZE);
	assert(nullvint.size() == 1);
	assert(maxvong.size() == VARLONG_MAX_SIZE - 1);
	assert(minvong.size() == VARLONG_MAX_SIZE);
	assert(nullvong.size() == 1);

	// Test the integer limits
	assert(maxvint.toInt() == maxint);
	assert(minvint.toInt() == minint);
	assert(nullvint.toInt() == null);
	assert(maxvong.toLong() == maxlong);
	assert(minvong.toLong() == minlong);
	assert(nullvong.toLong() == null);

	cout << "\tData parsing\n";

	// Check that VarInts can parse their own data
	assert(VarInt(maxvint.makeData()).toInt() == maxvint.toInt());
	assert(VarInt(minvint.makeData()).toInt() == minvint.toInt());
	assert(VarInt(nullvint.makeData()).toInt() == nullvint.toInt());
	assert(VarLong(maxvong.makeData()).toLong() == maxvong.toLong());
	assert(VarLong(minvong.makeData()).toLong() == minvong.toLong());
	assert(VarLong(nullvong.makeData()).toLong() == nullvong.toLong());
	
	// Check that basic VarInts and VarLongs parse according to existing standards
	assert(VarInt(0).makeData()[0] == 0);
	assert(VarInt(1).makeData()[0] == 1);
	assert(VarInt(2).makeData()[0] == 2);
	assert(VarInt(127).makeData()[0] == 127);
	assert(VarLong((Long)0).makeData()[0] == 0);
	assert(VarLong(1).makeData()[0] == 1);
	assert(VarLong(2).makeData()[0] == 2);
	assert(VarLong(127).makeData()[0] == 127);
	
	// Check that more advanced VarInts parse according to existing standards
	VarInt temp(128);
	assert((UByte)temp.makeData()[0] == 128);
	assert((UByte)temp.makeData()[1] == 1);

	temp = VarInt(255);
	assert((UByte)temp.makeData()[0] == 255);
	assert((UByte)temp.makeData()[1] == 1);

	temp = VarInt(25565);
	assert((UByte)temp.makeData()[0] == 221);
	assert((UByte)temp.makeData()[1] == 199);
	assert((UByte)temp.makeData()[2] == 1);

	temp = VarInt(2097151);
	assert((UByte)temp.makeData()[0] == 255);
	assert((UByte)temp.makeData()[1] == 255);
	assert((UByte)temp.makeData()[2] == 127);

	temp = VarInt(2147483647);
	assert((UByte)temp.makeData()[0] == 255);
	assert((UByte)temp.makeData()[1] == 255);
	assert((UByte)temp.makeData()[2] == 255);
	assert((UByte)temp.makeData()[3] == 255);
	assert((UByte)temp.makeData()[4] == 7);

	temp = VarInt(-1);
	assert((UByte)temp.makeData()[0] == 255);
	assert((UByte)temp.makeData()[1] == 255);
	assert((UByte)temp.makeData()[2] == 255);
	assert((UByte)temp.makeData()[3] == 255);
	assert((UByte)temp.makeData()[4] == 15);

	temp = VarInt(-(Int)2147483648);
	assert((UByte)temp.makeData()[0] == 128);
	assert((UByte)temp.makeData()[1] == 128);
	assert((UByte)temp.makeData()[2] == 128);
	assert((UByte)temp.makeData()[3] == 128);
	assert((UByte)temp.makeData()[4] == 8);

	// Check that more advanced VarLongs parse according to existing standards
	VarLong temp2(128);
	assert((UByte)temp2.makeData()[0] == 128);
	assert((UByte)temp2.makeData()[1] == 1);

	temp2 = VarLong(255);
	assert((UByte)temp2.makeData()[0] == 255);
	assert((UByte)temp2.makeData()[1] == 1);

	temp2 = VarLong(2147483647);
	assert((UByte)temp2.makeData()[0] == 255);
	assert((UByte)temp2.makeData()[1] == 255);
	assert((UByte)temp2.makeData()[2] == 255);
	assert((UByte)temp2.makeData()[3] == 255);
	assert((UByte)temp2.makeData()[4] == 7);

	temp2 = VarLong(9223372036854775807);
	assert((UByte)temp2.makeData()[0] == 255);
	assert((UByte)temp2.makeData()[1] == 255);
	assert((UByte)temp2.makeData()[2] == 255);
	assert((UByte)temp2.makeData()[3] == 255);
	assert((UByte)temp2.makeData()[4] == 255);
	assert((UByte)temp2.makeData()[5] == 255);
	assert((UByte)temp2.makeData()[6] == 255);
	assert((UByte)temp2.makeData()[7] == 255);
	assert((UByte)temp2.makeData()[8] == 127);

	temp2 = VarLong(-1);
	assert((UByte)temp2.makeData()[0] == 255);
	assert((UByte)temp2.makeData()[1] == 255);
	assert((UByte)temp2.makeData()[2] == 255);
	assert((UByte)temp2.makeData()[3] == 255);
	assert((UByte)temp2.makeData()[4] == 255);
	assert((UByte)temp2.makeData()[5] == 255);
	assert((UByte)temp2.makeData()[6] == 255);
	assert((UByte)temp2.makeData()[7] == 255);
	assert((UByte)temp2.makeData()[8] == 255);
	assert((UByte)temp2.makeData()[9] == 1);

	temp2 = VarLong(-(Long)2147483648);
	assert((UByte)temp2.makeData()[0] == 128);
	assert((UByte)temp2.makeData()[1] == 128);
	assert((UByte)temp2.makeData()[2] == 128);
	assert((UByte)temp2.makeData()[3] == 128);
	assert((UByte)temp2.makeData()[4] == 248);
	assert((UByte)temp2.makeData()[5] == 255);
	assert((UByte)temp2.makeData()[6] == 255);
	assert((UByte)temp2.makeData()[7] == 255);
	assert((UByte)temp2.makeData()[8] == 255);
	assert((UByte)temp2.makeData()[9] == 1);

	temp2 = VarLong(-(Long)9223372036854775808);
	assert((UByte)temp2.makeData()[0] == 128);
	assert((UByte)temp2.makeData()[1] == 128);
	assert((UByte)temp2.makeData()[2] == 128);
	assert((UByte)temp2.makeData()[3] == 128);
	assert((UByte)temp2.makeData()[4] == 128);
	assert((UByte)temp2.makeData()[5] == 128);
	assert((UByte)temp2.makeData()[6] == 128);
	assert((UByte)temp2.makeData()[7] == 128);
	assert((UByte)temp2.makeData()[8] == 128);
	assert((UByte)temp2.makeData()[9] == 1);

	cout << "\tFinished\n\n";
}
