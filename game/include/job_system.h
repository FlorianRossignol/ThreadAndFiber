#pragma once
#include <functional>
#include <mutex>
#include <optional>

#include "entity.h"

//job receive a function argument
struct JobDispatchArgs
{
	uint32_t jobIndex;
	uint32_t groupIndex;
};

namespace JobSystem
{
	//initialyze job system
	void Initialize();

	//add a job to execute asynchronously, any ide thread execute
	void Execute(const std::function<void()>& job);

	//Divide job into multiple in parallel.
	//jobcount : how many jobs generate for this task
	//groupeSize : how many job to execute per thread. Job inside a groupe execute serially. It might be worth to increment
	//func : receives a JobdispatcherArgs as parameter
	void Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job);

	//check if threads are wokinng currently or not
	bool IsBusy();

	//Wait until all threads become idle
	void Wait();

	// This little helper function will not let the system to be deadlocked while the main thread is waiting for something
	void Pool();

	template <typename T,size_t capacity>
	class ThreadSafeRingBuffer
	{
	public:
		//	Push an item to the end if there is free space
		//  Returns true if succesful
		//  Returns false if there is not enough space
		inline bool push_back(const T& item)
		{
			bool result = false;
			lock.lock();
			size_t next = (head + 1) % capacity;
			if (next != tail)
			{
				data[head] = item;
				head = next;
				result = true;
			}
			lock.unlock();
			return result;
		}

		// Get an item if there are any
		//  Returns true if succesful
		//  Returns false if there are no items
		inline bool pop_front(T& item)
		{
			bool result = false;
			lock.lock();
			if (tail != head)
			{
				item = data[tail];
				tail = (tail + 1) % capacity;
				result = true;
 			}
			lock.unlock();
			return result;
		}

	private:
		std::mutex lock;
		size_t tail = 0;
		size_t head = 0;
		T data[capacity];
	};

	class Coroutine
	{
	public:
		using Yield = std::function<void()>;
		using Run = std::function<void(Yield)>;

		virtual ~Coroutine() = default;

		virtual void Setup(Run f) = 0;

		virtual bool Step() = 0; 
	};

	class FiberCoroutine : Coroutine
	{
	public:
		FiberCoroutine() : mCurrent(nullptr), mRunning(false) {}

		~FiberCoroutine() override
		{
			if (mCurrent)
				DeleteFiber(mCurrent);
		}

		void Setup(Run f) override
		{
			if (!mMain)
			{
				mMain = ConvertThreadToFiber(NULL);
			}
			mRunning = true;
			mFunction = std::move(f);

			if (!mCurrent)
			{
				mCurrent = CreateFiber(0,
					&FiberCoroutine::proc, this);
			}
		}

		bool Step() override
		{
			SwitchToFiber(mCurrent);
			//return mRunning;
			return mRunning;
		}

		void yield()
		{
			SwitchToFiber(mMain);
		}

	private:

		void run()
		{
			while (true)
			{
				mFunction([this]
					{ yield(); });
				mRunning = false;
				yield();
			}
		}

		static VOID WINAPI proc(LPVOID data)
		{
			reinterpret_cast<FiberCoroutine*>(data)->run();
		}

		inline static LPVOID mMain = nullptr;
		LPVOID mCurrent;
		bool mRunning;
		Run mFunction;
	};
	
	
}
