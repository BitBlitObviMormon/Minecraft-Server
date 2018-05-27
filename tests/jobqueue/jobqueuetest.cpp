#include "jobqueuetest.h"
#include "data/jobqueue.h"
#include <thread>
#include <iostream>

#define TIMES_TO_START 3

int data[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

JobQueue jobs;
void test()
{
	// Count to 10 using an index to static data
	// Sleep to increase the *chance* of pushing and popping onto the queue simultaneously
	srand(clock());
	for (int i = 0; i < 10; ++i)
	{
		std::cout << "Pushed: " << data[i] << "\n";
		jobs.push([i]() { assert(i < 10 && i >= 0); std::cout << "Popped: " << data[i] << "\n"; assert(data[i] == i); std::this_thread::sleep_for(std::chrono::nanoseconds(1)); });
		std::this_thread::sleep_for(std::chrono::nanoseconds(rand() % 16));
	}

	// Stop the job queue to check that the stop function works
	std::cout << "Stopping queue...\n";
	jobs.stop();

	// Count to 50 using a variable
	// Sleep a significantly longer amount of time to increase the *chance* that the other thread will finish popping before this one is done pushing
	for (int i = 10; i < 50; ++i)
	{
		std::cout << "Pushed: " << i << "\n";
		jobs.push([i]() { std::cout << "Popped: " << i << "\n"; assert(i < 100 && i >= 0); });
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 8));
	}
}

/***************************************************************
 * JOB QUEUE TEST                                              *
 ***************************************************************
 * Simply tests that the job queue can have jobs inserted and  *
 * run on different threads at the same time. INCOMPLETE TEST! *
 ***************************************************************/
void JobQueueTest() {
	std::thread t1(test);
	std::thread t2(test);

	// Start and restart the job execution until it's reached the intended number of startups.
	for (int i = 0; i < TIMES_TO_START; ++i)
	{
		std::cout << "Starting...\n";
		std::cout << (jobs.start(false) ? "Finished...\n" : "Stopped...\n");
	}

	// Wait for the other threads to finish
	std::cout << "Joining...\n";
	t1.join();
	t2.join();

	// Execute the rest of the jobs
	std::cout << "Starting...\n";
	std::cout << (jobs.start(false) ? "Finished...\n" : "Stopped...\n") << "Done!\n";
}
