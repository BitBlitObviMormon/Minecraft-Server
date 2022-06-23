#include "stringtest.h"
#include <bbms/datatypes.h>
#include <assert.h>
#include <string.h>
#pragma warning( disable : 4703 ) // Don't worry about potential uninitialized pointers. They get initialized or throw an assert

/************************************************************
 * STRING TEST                                              *
 * May take up to a gigabyte of memory!                     *
 ************************************************************/
using namespace std;
void StringTest() {
	cout << "String / Chat Tests\n\tInitializing...\n";

	// Generate some test data from a sequence of numbers
	string data0; // Null string
	string data1; // Data that overflows nothing
	string data2; // Overflows SerialString but not SerialChat
	string data3; // Overflows SerialString but not SerialChat, longer than data2
	string data4; // Overflows SerialString and SerialChat
	data1.reserve(SERIALSTRING_MAX_LENGTH);
	data2.reserve(SERIALSTRING_MAX_LENGTH + 1);
	data3.reserve(SERIALCHAT_MAX_LENGTH);
	data4.reserve(SERIALCHAT_MAX_LENGTH + 1);
	for (int i = 0; i < (SERIALCHAT_MAX_LENGTH + 2); ++i) {
		if (i < SERIALSTRING_MAX_LENGTH)
			data1.append(1, i % 94 + 32);
		if (i < SERIALSTRING_MAX_LENGTH + 1)
			data2.append(1, i % 94 + 32);
		if (i < SERIALCHAT_MAX_LENGTH)
			data3.append(1, i % 94 + 32);
		data4.append(1, i % 94 + 32);
	}

	// Create some variables
	cout << "\tTesting exception throwing\n";
	SerialString* str0; // Null string
	SerialString* str1; // Null string also
	SerialString* str2; // "Hello World!"
	SerialString* str3; // Data of allowable size
	SerialString* str4; // Too large, throws exception
	SerialChat* chat0; // Null string
	SerialChat* chat1; // Null string also
	SerialChat* chat2; // "Hello World!"
	SerialChat* chat3; // Data of allowable size
	SerialChat* chat4; // Too large, throws exception
	try {
		str0 = new SerialString;
		str1 = new SerialString(data0);
		str2 = new SerialString("Hello World!");
		str3 = new SerialString(data1);
		chat0 = new SerialChat;
		chat1 = new SerialChat(data0);
		chat2 = new SerialChat("Hello World!");
		chat3 = new SerialChat(data3);
	}
	catch (std::length_error) {
		assert("Exception was raised at inappropriate time." && false);
	}
	catch (std::bad_alloc e) {
		cout << "\t" << e.what() << "\tFailed to allocate data for testing, ending test now.\n\n";
		return;
	}

	// Check that str4 throws exception
	try {
		str4 = new SerialString(data2);
		assert("str4 failed to throw exception upon construction" && false);
	}
	catch (std::length_error) {} // Ignore the exception, we wanted it to happen
	catch (std::bad_alloc e) {
		cout << "\t" << e.what() << "\tFailed to allocate data for testing, ending test now.\n\n";
		return;
	}

	// Check that chat4 throws exception
	try {
		chat4 = new SerialChat(data4);
		assert("chat4 failed to throw exception upon construction" && false);
	}
	catch (std::length_error) {} // Ignore the exception, we wanted it to happen
	catch (std::bad_alloc e) {
		cout << "\t" << e.what() << "\tFailed to allocate data for testing, ending test now.\n\n";
		return;
	}

	// Validate the data
	cout << "\tTesting constructors\n";

	// Check the length of each variable
	const std::string hiStr("Hello World!");
	assert(str0->length() == 0);
	assert(str1->length() == 0);
	assert(str2->length() == hiStr.length());
	assert(str3->length() == SERIALSTRING_MAX_LENGTH);
	assert(chat0->length() == 0);
	assert(chat1->length() == 0);
	assert(chat2->length() == hiStr.length());
	assert(chat3->length() == SERIALCHAT_MAX_LENGTH);

	// Check the size of each variable
	VarInt vint0(0);
	VarInt vint1(hiStr.length());
	VarInt vint2(SERIALSTRING_MAX_LENGTH);
	VarInt vint3(SERIALCHAT_MAX_LENGTH);
	assert(str0->size() == vint0.size());
	assert(str1->size() == vint0.size());
	assert(str2->size() == vint1.size() + hiStr.length());
	assert(str3->size() == vint2.size() + data1.length());
	assert(chat0->size() == vint0.size());
	assert(chat1->size() == vint0.size());
	assert(chat2->size() == vint1.size() + hiStr.length());
	assert(chat3->size() == vint3.size() + data3.length());

	// Verify the integrity of the data
	{
		// Get some manageable pointers
		auto buf1 = str0->makeData();  // Null string
		auto buf2 = str1->makeData();  // Null string
		auto buf3 = chat0->makeData(); // Null string
		auto buf4 = chat1->makeData(); // Null string
		auto buf5 = str2->makeData();
		auto buf6 = str3->makeData();
		auto buf7 = chat2->makeData();
		auto buf8 = chat3->makeData();
		const char* ptr1 = (const char*)buf1.get();
		const char* ptr2 = (const char*)buf2.get();
		const char* ptr3 = (const char*)buf3.get();
		const char* ptr4 = (const char*)buf4.get();
		const char* ptr5 = (const char*)buf5.get();
		const char* ptr6 = (const char*)buf6.get();
		const char* ptr7 = (const char*)buf7.get();
		const char* ptr8 = (const char*)buf8.get();

		// Check that each length variable is what it should be
		assert(memcmp(ptr1, (const char*)vint0.makeData(), vint0.size()) == 0);
		assert(memcmp(ptr2, (const char*)vint0.makeData(), vint0.size()) == 0);
		assert(memcmp(ptr3, (const char*)vint0.makeData(), vint0.size()) == 0);
		assert(memcmp(ptr4, (const char*)vint0.makeData(), vint0.size()) == 0);
		assert(memcmp(ptr5, (const char*)vint1.makeData(), vint1.size()) == 0);

		// Move the pointers over to the string portion of the data
		ptr5 += vint1.size();
		ptr6 += vint2.size();
		ptr7 += vint1.size();
		ptr8 += vint3.size();

		// Check that each string is equal
		assert(memcmp(ptr5, "Hello World!", hiStr.length()) == 0);
		assert(memcmp(ptr6, data1.c_str(), SERIALSTRING_MAX_LENGTH) == 0);
		assert(memcmp(ptr7, "Hello World!", hiStr.length()) == 0);
		assert(memcmp(ptr8, data3.c_str(), SERIALCHAT_MAX_LENGTH) == 0);
	} // all the buffers get deleted here

	cout << "\tTesting assignment\n";

	// Verify that the null strings are equal to each other
	assert(str0->operator==(*str1));
	assert(chat0->operator==(*chat1));

	// Assign one of the null strings to "Hello World!" and verify it
	// is equal to another string that is already set to "Hello World!"
	str0->operator=("Hello World!");
	chat0->operator=("Hello World!");
	assert(str0->operator==(*str2));
	assert(chat0->operator==(*chat2));

	// Do this again, but with the massive strings
	str0->operator=(*str3);
	chat0->operator=(*chat3);
	assert(str0->operator==(*str3));
	assert(chat0->operator==(*chat3));

	// Delete the pointers and free the data
	// str4 and chat4 are already cleaned up by throwing an exception in their constructor
	cout << "\tCleaning up\n";
	delete str0;
	delete str1;
	delete str2;
	delete str3;
	delete chat0;
	delete chat1;
	delete chat2;
	delete chat3;

	cout << "\tFinished\n\n";
}
