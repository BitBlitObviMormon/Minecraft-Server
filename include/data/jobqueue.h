#pragma once

#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <functional>

class JobQueue
{
private:
	std::thread jobThread;
	std::mutex queueLock;
	std::queue< const std::function<void()> > jobQueue;
	std::atomic<bool> running;
	bool executeJobs(bool waitForJobs);
public:
	JobQueue() {}
	~JobQueue() { stop(); }
	bool start(bool runAsync = true);
	int size();
	void push(const std::function<void()> job);
	void stop() { running.store(false); }
	bool empty() { return !size(); }
	bool isRunning() { return running.load(); }
	bool isExecuting() { return size() && running.load(); }
};