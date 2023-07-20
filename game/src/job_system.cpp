#include "job_system.h"
#include <algorithm>
#include <iostream>
#include <atomic>
#include <thread>
#include <condition_variable>

namespace JobSystem
{
	uint32_t numThreads = 0;
	ThreadSafeRingBuffer<std::function<void()>, 256> jobPool;
	std::condition_variable wakeCondition;
	std::mutex wakeMutex;
	uint64_t currentLabel = 0;
	std::atomic<uint64_t> finishedLabel;
	

	void Initialize()
	{
		// Initialize the worker execution state to 0:
		finishedLabel.store(0);

		// Retrieve the number of hardware threads in this system:
		auto numCores = std::thread::hardware_concurrency();

		//calculate the actual number of worker threads we want
		numThreads = std::max(1u, numCores);

		// Create all our worker threads while immediately starting them:
		for (uint32_t threadId = 0; threadId < numThreads; ++threadId)
		{
			std::thread worker ([]()
			{
				std::function<void()> job; // the current job for the thread, it's empty at start.
				// This is the infinite loop that a worker thread will do
				while (true)
				{
					if (jobPool.pop_front(job)) // try to grab a job from the jobPool queue
					{
						// It found a job, execute it:
						job();
						finishedLabel.fetch_add(1); // update worker label state
					}
					else
					{
						// no job, put thread to sleep
						std::unique_lock<std::mutex> lock(wakeMutex);
						wakeCondition.wait(lock);
					}
				}
			});

			worker.detach(); // forget about this thread, let it do it's job in the infinite loop that we created above
		}
	}

	void Execute(const std::function<void()>& job)
	{
		// The main thread label state is updated:
		currentLabel += 1;

		// Try to push a new job until it is pushed successfully:
		while (!jobPool.push_back(job))
		{
			Pool();
		}
		// wake one thread
		wakeCondition.notify_one();
	}

	void Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job)
	{
		if (jobCount == 0 || groupSize == 0)
		{
			return;
		}
		// Calculate the amount of job groups to dispatch (overestimate, or "ceil"):
		const uint32_t groupCount = (jobCount + groupSize - 1) / groupSize;

		// The main thread label state is updated:
		currentLabel += groupCount;

		for (uint32_t groupIndex = 0; groupIndex < groupCount; ++groupIndex)
		{
			// For each group, generate one real job:
			auto jobGroup = [jobCount, groupSize, job, groupIndex]()
			{
				// Calculate the current group's offset into the jobs:
				const uint32_t groupJobOffset = groupIndex * groupSize;
				const uint32_t groupJobEnd = std::min(groupJobOffset + groupSize, jobCount);

				JobDispatchArgs args;
				args.groupIndex = groupIndex;

				// Inside the group, loop through all job indices and execute job for each index:
				for (uint32_t i = groupJobOffset; i < groupJobEnd; ++i)
				{
					args.jobIndex = i;
					job(args);
				}
			};

			// Try to push a new job until it is pushed successfully:
			while (!jobPool.push_back(jobGroup)) 
			{
				Pool();
			}
			wakeCondition.notify_one(); // wake one thread
		}
	}

	bool IsBusy()
	{
		// Whenever the main thread label is not reached by the workers, it indicates that some worker is still alive
		return finishedLabel.load() < currentLabel;
	}

	void Wait()
	{
		while (IsBusy())
		{
			Pool();
		}
	}

	void Pool()
	{
		wakeCondition.notify_one(); // wake one worker thread
		std::this_thread::yield();	// allow this thread to be rescheduled
	}

}
