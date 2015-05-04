#include <pthread.h>
#include <iostream>
#include <deque>
using namespace std;
class Job
{
	public:
		void *(*doJobFun)(void *arg);
		void *arg;
};

class ThreadPool
{
	private:
		int threadnum;
		int maxJobnum;
		deque<Job> jobQueue;
		pthread_t *pthreads;
		pthread_mutex_t mutex;
		pthread_cond_t condQueueEmpty;
		pthread_cond_t condQueueFull;
		bool isPoolClose;
	public:
		ThreadPool(int threadNum = 10, int maxJobNum = 100);
		int addJob(Job job);
		int destroy();
		friend void *run(void *arg);
		~ThreadPool();
};
