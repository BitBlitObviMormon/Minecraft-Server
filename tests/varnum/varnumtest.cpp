#include "varnumtest.h"
#include "data/datatypes.h"
#include <atomic>
#include <thread>
#include <chrono>
#include <climits>
#include <iostream>

// Global Variables
std::atomic<Byte> stage = 0; // 0 = hasn't started, 1 = integer tests, 2 = long tests, 3 = finished
volatile Int i = 0;
volatile Long l = 0;

void GuiThread()
{
	Byte currentStage = 0;

	while (currentStage < 3)
	{
		// If we started the int tests then say so
		if (stage == 1 && currentStage != 1)
		{
			std::cout << "Starting integer tests...\n";
			currentStage = 1;
		}
		// If we started the long tests then say so
		else if (stage == 2 && currentStage != 2)
		{
			std::cout << "\nStarting long tests...\n";
			currentStage = 2;
		}
		// If we are running the int tests then show the current progress
		else if (currentStage == 1)
		{
			// Calculate the current progress
			Double prog = ((Double)(UInt)(i - INT_MIN)) / UINT_MAX * 100.0;
			std::cout << "\rCurrent Progress: %" << prog;
		}
		// If we are running the long tests then show the current progress
		else if (currentStage == 2)
		{
			// Calculate the current progress
			Double prog = ((Double)(ULong)(l - LLONG_MIN)) / ULLONG_MAX * 100.0;
			std::cout << "\rCurrent Progress: %" << prog;
		}

		// Sleep for a bit, the gui thread doesn't need to hog the CPU
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	// We finished! Yay!
	std::cout << "\nFinished!\n";
}

/*************************************************************
 * VAR NUM TEST                                              *
 *************************************************************
 * Tests that all VarNums are correct and do not leak memory *
 * Note: this is a very long test, will take WEEKS to finish!*
 *************************************************************/
void VarNumTest() {
	// Start the GUI Thread
	std::thread gui(GuiThread);

	// Run the integer tests
	stage = 1;
	for (i = INT_MAX - 10; i < INT_MAX; ++i)
	{
		VarInt v = VarInt((const Int)i);
		Int temp = v.toInt();
		if (temp != (Int)i)
			std::cout << temp << " != " << i << "! (assert failed: v.toInt() == (Int)i)\n";
	}

	// Run the long tests
	stage = 2;
	for (l = /*_I64_MIN*/-10; l < _I64_MAX; ++l)
	{
		VarLong v = VarLong(l);
		Long temp = v.toLong();
		if (temp != (Long)l)
			std::cout << temp << " != " << l << "! (assert failed: v.toLong() == (Long)l)\n";
	}

	// Wait for the GUI to exit
	stage = 3;
	gui.join();
}
