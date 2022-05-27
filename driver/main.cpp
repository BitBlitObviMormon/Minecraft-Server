#include <iostream>
#include <memory>

// Set to true to run all of the given tests
#define RUN_TESTS 0

#if(RUN_TESTS)
	#include "tests/varnum/varnumtest.h"

	// Comment any of these definitions to run that test
//	#define VarNumTest()

	// Runs all of the tests to make sure that certain code works
	void RunTests()
	{
		// Test the VarNums
		VarNumTest();

		// Wait for the user to press enter before exiting
		system("pause");
		exit(0);
	}
#else
	#define RunTests()
#endif

#include "server.h"
using namespace std;

class Player : public Client
{
private:
public:
	Player(SOCKET socket) : Client(socket) {}
};

class Game : public EventHandler
{
private:
public:
	Game() : EventHandler() {}
	shared_ptr<Client> createClient(SOCKET socket) override {
		return static_pointer_cast<Client>(make_shared<Player>(socket));
	}
};


int main()
{
	// Run the tests, if they exist
	RunTests();

	// Create and start the server
	Game game = Game();
	Server server(&game);
	server.start();

	return 0;
}