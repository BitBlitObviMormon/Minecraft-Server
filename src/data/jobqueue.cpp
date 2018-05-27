#include "data/jobqueue.h"

/*********************************
 * Job Queue :: executeJobs      *
 * Executes jobs from the queue  *
 * Returns true if all jobs were *
 * executed successfully         *
 *********************************/
bool JobQueue::executeJobs(bool waitForJobs)
{
	// Run all of the jobs in the queue
	std::unique_lock<std::mutex> lock(queueLock);
	while (jobQueue.size() && running.load())
	{
		// Get the job and pop it off of the queue
		const std::function<void()> func = jobQueue.front();
		jobQueue.pop();

		// Unlock the mutex and run the job
		lock.unlock();
		func();
		
		// Lock the mutex again to check for more jobs
		lock.lock();
	}

	// Unlock the mutex after viewing the queue's size
	lock.unlock();

	// If we're waiting for another job then sleep
	if (waitForJobs && running.load())
	{
		// TODO: Use a condition to wake up if a job is added to the queue
	}

	// Make sure we stop and return whether we stopped prematurely or finished all jobs
	return running.exchange(false);
}

/**********************************************************************************
 * Job Queue :: start                                                             *
 * Starts executing jobs from the queue                                           *
 * If running async, returns true if a thread started successfully                *
 * If not async, returns true if all jobs in the queue were executed successfully *
 **********************************************************************************/
bool JobQueue::start(bool runAsync)
{
	// If the job queue is not already running then start
	if (!running.exchange(true))
	{
		// If the user wants to start on a different thread then do so
		if (runAsync)
			jobThread = std::thread([&]() { executeJobs(true); });
		// If the user doesn't want to run async then do them all now
		else
			return executeJobs(false);
	}

	return true;
}

/*********************************
 * Job Queue :: size             *
 * Returns the size of the queue *
 *********************************/
int JobQueue::size()
{
	std::lock_guard<std::mutex> lock(queueLock);
	return jobQueue.size();
}

/****************************************
 * Job Queue :: start                   *
 * Starts executing jobs from the queue *
 ****************************************/
void JobQueue::push(const std::function<void()> job)
{
	std::lock_guard<std::mutex> lock(queueLock);
	jobQueue.push(job);
}
