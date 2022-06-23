#include "positiontest.h"
#include <bbms/datatypes.h>
#include <assert.h>
#pragma warning( disable : 4703 ) // Don't worry about potential uninitialized pointers. They get initialized or throw an assert

/************************************************************
 * POSITION TEST                                            *
 ************************************************************/
using namespace std;
void PositionTest() {
	cout << "Position Test\n\tAssignment\n";

	SerialPosition* pos0; // Null position
	SerialPosition* pos1; // Null position
	SerialPosition* pos2; // -1 for each value
	SerialPosition* pos3; // -1 for each value
	SerialPosition* pos4; // Max values
	SerialPosition* pos5; // Min values
	SerialPosition* pos6; // Max values
	SerialPosition* pos7; // Min values

	try {
		pos0 = new SerialPosition(0, 0, 0); // Null position
		pos1 = new SerialPosition(0);  // Also null position
		pos2 = new SerialPosition(-1, -1, -1); // -1 for each value
		pos3 = new SerialPosition(0xffffffffffffffff); // -1 for each value
		pos4 = new SerialPosition(SERIALPOSITION_X_MAX, SERIALPOSITION_Y_MAX, SERIALPOSITION_Z_MAX); // Max values
		pos5 = new SerialPosition(SERIALPOSITION_X_MIN, SERIALPOSITION_Y_MIN, SERIALPOSITION_Z_MIN); // Min values
		pos6 = new SerialPosition(0x7fffffdffffff7ff); // Max values
		pos7 = new SerialPosition(0x8000002000000800); // Min values
	}
	catch (...) {
		assert("Exception was raised at inappropriate time." && false);
	}

	// Check that the values remain the same
	assert(*pos0 == *pos1);
	assert(*pos2 == *pos3);
	assert(*pos4 == *pos6);
	assert(*pos5 == *pos7);
	assert(pos0->getX() == 0);
	assert(pos0->getY() == 0);
	assert(pos0->getZ() == 0);
	assert(pos1->getX() == 0);
	assert(pos1->getY() == 0);
	assert(pos1->getZ() == 0);
	assert(pos2->getX() == -1);
	assert(pos2->getY() == -1);
	assert(pos2->getZ() == -1);
	assert(pos3->getX() == -1);
	assert(pos3->getY() == -1);
	assert(pos3->getZ() == -1);
	assert(pos4->getX() == SERIALPOSITION_X_MAX);
	assert(pos4->getY() == SERIALPOSITION_Y_MAX);
	assert(pos4->getZ() == SERIALPOSITION_Z_MAX);
	assert(pos5->getX() == SERIALPOSITION_X_MIN);
	assert(pos5->getY() == SERIALPOSITION_Y_MIN);
	assert(pos5->getZ() == SERIALPOSITION_Z_MIN);
	assert(pos6->getX() == SERIALPOSITION_X_MAX);
	assert(pos6->getY() == SERIALPOSITION_Y_MAX);
	assert(pos6->getZ() == SERIALPOSITION_Z_MAX);
	assert(pos7->getX() == SERIALPOSITION_X_MIN);
	assert(pos7->getY() == SERIALPOSITION_Y_MIN);
	assert(pos7->getZ() == SERIALPOSITION_Z_MIN);

	// Clean up previous data
	cout << "\tException checking\n";
	delete pos0;
	delete pos1;
	delete pos2;
	delete pos3;
	delete pos4;
	delete pos5;
	delete pos6;
	delete pos7;

	// Check x overflow
	try {
		SerialPosition pos(SERIALPOSITION_X_MAX + 1, 0, 0);
		assert("SerialPosition failed to throw exception for X value overflow" && false);
	}
	catch (std::overflow_error) {} // Ignore this exception, it's what we want
	catch (std::underflow_error) { // We do not want to catch this exception
		assert("SerialPosition threw wrong exception for X value overflow" && false);
	}

	// Check x underflow
	try {
		SerialPosition pos(SERIALPOSITION_X_MIN - 1, 0, 0);
		assert("SerialPosition failed to throw exception for X value underflow" && false);
	}
	catch (std::underflow_error) {} // Ignore this exception, it's what we want
	catch (std::overflow_error) { // We do not want to catch this exception
		assert("SerialPosition threw wrong exception for X value underflow" && false);
	}

	// Check y overflow
	try {
		SerialPosition pos(0, SERIALPOSITION_Y_MAX + 1, 0);
		assert("SerialPosition failed to throw exception for Y value overflow" && false);
	}
	catch (std::overflow_error) {} // Ignore this exception, it's what we want
	catch (std::underflow_error) { // We do not want to catch this exception
		assert("SerialPosition threw wrong exception for Y value overflow" && false);
	}

	// Check y underflow
	try {
		SerialPosition pos(0, SERIALPOSITION_Y_MIN - 1, 0);
		assert("SerialPosition failed to throw exception for Y value underflow" && false);
	}
	catch (std::underflow_error) {} // Ignore this exception, it's what we want
	catch (std::overflow_error) { // We do not want to catch this exception
		assert("SerialPosition threw wrong exception for Y value underflow" && false);
	}

	// Check z overflow
	try {
		SerialPosition pos(0, 0, SERIALPOSITION_Z_MAX + 1);
		assert("SerialPosition failed to throw exception for Z value overflow" && false);
	}
	catch (std::overflow_error) {} // Ignore this exception, it's what we want
	catch (std::underflow_error) { // We do not want to catch this exception
		assert("SerialPosition threw wrong exception for Z value overflow" && false);
	}

	// Check z underflow
	try {
		SerialPosition pos(0, 0, SERIALPOSITION_Z_MIN - 1);
		assert("SerialPosition failed to throw exception for Z value underflow" && false);
	}
	catch (std::underflow_error) {} // Ignore this exception, it's what we want
	catch (std::overflow_error) { // We do not want to catch this exception
		assert("SerialPosition threw wrong exception for Z value underflow" && false);
	}

	cout << "\tFinished\n\n";
}
