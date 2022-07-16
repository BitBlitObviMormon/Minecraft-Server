// Set to true to run all of the given tests
#define RUN_TESTS true

#if(RUN_TESTS)
	#include <iostream>
	#include <memory>
	#include "tests/angle/angletest.h"
	#include "tests/nbt/nbttest.h"
	#include "tests/position/positiontest.h"
	#include "tests/slot/slottest.h"
	#include "tests/string/stringtest.h"
	#include "tests/uuid/uuidtest.h"
	#include "tests/varnum/varnumtest.h"

	// Comment any of these definitions to run that test
	// Any tests that are commented out will not be run
//	#define AngleTest()
//	#define NBTTest()
//	#define PositionTest()
//	#define SlotTest()
//	#define StringTest()
//	#define UUIDTest()
//	#define VarNumTest()

	// Runs all of the tests to make sure that certain code works
	void RunTests()
	{
		// Execute all of the tests (if enabled)
		AngleTest();
		NBTTest();
		PositionTest();
		SlotTest();
		StringTest();
		UUIDTest();
		VarNumTest();

		// Wait for the user to press enter before exiting
		system("pause");
		exit(0);
	}
#else
	#define RunTests()
#endif

///////////////
// MAIN FILE //
///////////////
#include <bbms/server.h>
using namespace std;

class Player : public Client
{
public:
	Short damage;
	Player(SOCKET socket) : Client(socket), damage(0) {}
};

class Game : public EventHandler
{
public:
	Game() : EventHandler() {}
	Client* createClient(SOCKET socket) override {
		return new Player(socket);
	}
};


int main()
{
	// Run the tests, if they exist
	RunTests();

	// Create and start the server
	Game game;
	Server server(&game);
	server.start();

	return 0;
}