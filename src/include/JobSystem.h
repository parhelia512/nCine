#ifndef CLASS_NCINE_JOBSYSTEM
#define CLASS_NCINE_JOBSYSTEM

#include "IJobSystem.h"
#include "JobQueue.h"
#include "Thread.h"
#include "ThreadSync.h"
#include <nctl/Array.h>

namespace ncine {

class JobQueue;

/// The multi-threaded job system
class JobSystem: public IJobSystem
{
  public:
	/// Creates a job system with as many threads as available processors for its pool
	JobSystem();
	/// Creates a job system with the specified number of threads for its pool
	explicit JobSystem(unsigned char numThreads);
	~JobSystem() override;

	JobId createJob(JobFunction function, const void *data, unsigned int dataSize) override;
	JobId createJobAsChild(JobId parentId, JobFunction function, const void *data, unsigned int dataSize) override;

	bool addContinuation(JobId ancestor, JobId continuation) override;

	void run(JobId jobId) override;
	void wait(JobId jobId) override;

	int32_t unfinishedJobs(JobId jobId) override;
	bool isValid(JobId jobId) override;

  private:
	static const unsigned int AlignmentBytes = sizeof(int);

	struct CommonThreadDataStruct
	{
		CommonThreadDataStruct(Mutex &mutex, CondVariable &CV)
		    : numThreads(0), jobQueues(nullptr), queueMutex(mutex), queueCV(CV) {}

		unsigned char numThreads;
		JobQueue *jobQueues;
		Mutex &queueMutex;
		CondVariable &queueCV; // TODO: Try also a semaphore that is incremented every time a job is added
	};

	struct ThreadStruct
	{
		explicit ThreadStruct(const CommonThreadDataStruct &commonThreadData)
		    : threadId(0), shouldQuit(false), commonData(commonThreadData) {}

		unsigned char threadId;
		unsigned char shouldQuit;
		const CommonThreadDataStruct &commonData;
	};

	uint8_t *jobQueuesBuffer_;
	JobQueue *jobQueues_;
	nctl::Array<Thread> threads_;
	nctl::Array<ThreadStruct> threadStructs_;

	Mutex queueMutex_;
	CondVariable queueCV_;
	CommonThreadDataStruct commonData_;

	void initPointers();
	static void workerFunction(void *arg);

	/// Deleted copy constructor
	JobSystem(const JobSystem &) = delete;
	/// Deleted assignment operator
	JobSystem &operator=(const JobSystem &) = delete;
};

}

#endif
