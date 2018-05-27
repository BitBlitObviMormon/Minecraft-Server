#include "debug.h"
#include "server/server.h"
#include <iostream>

// Set to true to run all of the given tests
#define RUN_TESTS 0

#if(RUN_TESTS)
	#include "tests/varnum/varnumtest.h"
	#include "tests/jobqueue/jobqueuetest.h"
	#include "tests/bitstream/bitstreamtest.h"
	#include "tests/atomicset/atomicsettest.h"

	// Comment any of these definitions to run that test
//	#define VarNumTest()
	#define JobQueueTest()
	#define BitStreamTest()
	#define AtomicSetTest()

	// Runs all of the tests to make sure that certain code works
	void RunTests()
	{
		// Test the VarNums
		VarNumTest();

		// Test the JobQueue
		JobQueueTest();

		// Test the BitStream
		BitStreamTest();

		// Test the AtomicSet
		AtomicSetTest();

		// Wait for the user to press enter before exiting
		system("pause");
		exit(0);
	}
#else
	#define RunTests()
#endif

class Game : public EventHandler
{
private:
public:
	Game() : EventHandler() {}
};


int main()
{
	// Run the tests, if they exist
	RunTests();

	// Create the server and its handlers then start the server
//	Game game();
	EventHandler* game = new EventHandler();
	NetworkHandler* networkHandler = new NetworkHandler(game);
	Server server = Server(game, networkHandler);
	server.start();

	return 0;
}
