#include "uuidtest.h"
#include "uuid.h"
#include <assert.h>

/************************************************************
 * STRING TEST                                              *
 ************************************************************/
using namespace std;
void UUIDTest() {
	cout << "UUID Test\n\tConstruction\n";

	// Some data prepping
	Byte bytes0[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	Byte bytes1[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
	ULong longs0[2] = { 0x0000000000000000, 0x0000000000000000 };
	ULong longs1[2] = { 0x0011223344556677, 0x8899aabbccddeeff };
	String strings0 = "00000000-0000-0000-0000-000000000000";
	String strings1 = "00112233-4455-6677-8899-aabbccddeeff";

	// Do some construction checking
	UUID b0(bytes0);
	UUID b1(bytes1);
	UUID l0(longs0[0], longs0[1]);
	UUID l1(longs1[0], longs1[1]);
	UUID s0(strings0);
	UUID s1(strings1);
	UUID u0(b0);
	UUID u1(b1);

	// Do some comparison checking
	assert(b0 == l0);
	assert(b0 == s0);
	assert(b0 == u0);
	assert(l0 == s0);
	assert(l0 == u0);
	assert(s0 == u0);
	assert(b1 == l1);
	assert(b1 == s1);
	assert(b1 == u1);
	assert(l1 == s1);
	assert(l1 == u1);
	assert(s1 == u1);

	// Check the actual byte data
	const Byte* b0Data = b0.makeData();
	const Byte* b1Data = b1.makeData();
	const Byte* l0Data = l0.makeData();
	const Byte* l1Data = l1.makeData();
	const Byte* s0Data = s0.makeData();
	const Byte* s1Data = s1.makeData();
	const Byte* u0Data = u0.makeData();
	const Byte* u1Data = u1.makeData();
	for (int i = 0; i < 16; ++i) {
		assert(b0Data[i] == 0x00);
		assert(l0Data[i] == 0x00);
		assert(s0Data[i] == 0x00);
		assert(u0Data[i] == 0x00);
		assert(b1Data[i] == bytes1[i]);
		assert(l1Data[i] == bytes1[i]);
		assert(s1Data[i] == bytes1[i]);
		assert(u1Data[i] == bytes1[i]);
	}

	cout << "\tAssignment\n";
	assert((u1 = u0) == b0);
	assert(u1 == b0);

	cout << "\tGenerator Samples:\n";
	try {
		cout << "\t  Null:   " << UUID::generate::null() << "\n"
			<< "\t  String: " << UUID::generate::fromString("00112233-4455-6677-8899-aabbccddeeff") << "\n"
			<< "\t  Random: " << UUID::generate::random() << "\n"
			<< "\t  MD5:    " << UUID::generate::fromNameMD5("Minecraft") << "\n"
			<< "\t  SHA-1:  " << UUID::generate::fromNameSHA1("Minecraft") << "\n";
	}
	catch (...) {
		assert("One of the generators threw an exception!" && false);
	}

	cout << "\tFinished\n\n";
}
