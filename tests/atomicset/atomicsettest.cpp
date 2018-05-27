#include "atomicsettest.h"
#include "data/atomicset.h"
#include <thread>
#include <iostream>
using namespace std;

AtomicSet<int> testData;

// A thread function that inserts numbers into the set
void insertThread(int num)
{
	for (int i = num * 1000; i < num * 1000 + 1000; ++i)
	{
		testData.insert(i);
	}
}

// Test how well the AtomicSet functions in a multi-threaded environment
void AtomicSetTest() {
	// Start up a bunch of threads that insert data
	cout << "Inserting into set...\n";
	thread threads[10];
	for (int i = 0; i < 10; ++i)
		threads[i] = thread([&](){ insertThread(i); });

	// While they are inserting, go ahead and clear the data for trolls
	testData.clear();
	
	// Wait for each thread
	for (int i = 0; i < 10; ++i)
		threads[i].join();

	// Now that the data is cleared, attempt to iterate through the rest of the input
	for each (int num in testData)
		cout << num << " ";

	// Reset the set for another test
	testData.clear();

	// Start inserting into the set again
	for (int i = 0; i < 10; ++i)
		threads[i] = thread([&](){ insertThread(i); });

	// Attempt to iterate through the set as stuff is being shoved into it
	for each (int num in testData)
		cout << num << " ";

	// Wait for each thread
	for (int i = 0; i < 10; ++i)
		threads[i].join();


}
