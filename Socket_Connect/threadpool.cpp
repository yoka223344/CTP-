#include <pthread.h>
#include <iostream>
#include <deque>
#include <stdlib.h>
#include <assert.h>
#include "threadpool.h"
using namespace std;
void *run(void *arg)
{
	ThreadPool *pool = (ThreadPool *)arg;
	class Job job;
	while(1)
	{
		pthread_mutex_lock(&(pool -> mutex));
		if (pool -> isPoolClose)
		{
			pthread_mutex_unlock(&(pool -> mutex));
			pthread_exit(NULL);
		}
		while((pool -> jobQueue.empty()))
		{
			pthread_cond_wait(&(pool -> condQueueEmpty), &(pool -> mutex));
			if (pool -> isPoolClose)
			{
				pthread_mutex_unlock(&(pool -> mutex));
				pthread_exit(NULL);
			}
		}
		job = pool -> jobQueue.front();
		pool -> jobQueue.pop_front();
		if (pool -> jobQueue.size() == (pool -> maxJobnum - 1))
		{
			pthread_cond_broadcast(&(pool -> condQueueFull));
		}
		pthread_mutex_unlock(&(pool -> mutex));
		(*(job.doJobFun))(job.arg);

	}
}

ThreadPool::ThreadPool(int threadNum, int maxJobNum)
{
	threadnum = threadNum;
	maxJobnum = maxJobNum;
	if (pthread_mutex_init(&(mutex), NULL))
	{
		cerr << "pthread_mutex_init error" << endl;
		exit(1);
		
	}
	if (pthread_cond_init(&(condQueueEmpty), NULL))
	{
		cerr << "pthread_cond_init error" << endl;
		exit(1);
	}
	if (pthread_cond_init(&(condQueueFull), NULL))
	{
		cerr << "pthread_cond_init error" << endl;
		exit(1);
	}
	pthreads = (pthread_t *) malloc(sizeof(pthread_t) * threadnum);
	if (NULL == pthreads)
	{
		cerr << "pthreads malloc error" << endl;
		exit(1);
	}
	isPoolClose = false;  
	unsigned int i;  
	for (i = 0; i < threadnum; ++i)  
	{						  
		pthread_create(&(pthreads[i]), NULL, run, (void *)this);  
	}
}

int ThreadPool::addJob(Job job)  
{
	assert(job.doJobFun != NULL);  
	assert(job.arg != NULL);  
	if (isPoolClose)   //线程池关闭，线程就退出  
	{
		return -1;  
	}  
	pthread_mutex_lock(&(mutex)); 
	while (jobQueue.size() == maxJobnum)  
	{
		cerr << "job queue is full" << endl;  
		pthread_cond_wait(&(condQueueFull), &(mutex));   //队列满的时候就等待  
	}  
	if (isPoolClose)    //队列关闭或者线程池关闭就退出  
	{
		pthread_mutex_unlock(&(mutex));  
		return -1;
	}
	if (jobQueue.empty())     
	{
		jobQueue.push_back(job);
		pthread_cond_broadcast(&(condQueueEmpty));  //队列空的时候，有任务来时就通知线程池中的线程：队列非空  
	}  
	else  
	{
		jobQueue.push_back(job);
	}                                                                                                     
	pthread_mutex_unlock(&(mutex));
	return 0;
}
int ThreadPool::destroy()  
{
	pthread_mutex_lock(&(mutex));  
	isPoolClose = true;        //置队列关闭标志    
	jobQueue.clear();  
	pthread_mutex_unlock(&(mutex));  	
	pthread_cond_broadcast(&(condQueueEmpty));  //唤醒线程池中正在阻塞的线程  
	pthread_cond_broadcast(&(condQueueFull));   //唤醒添加任务的threadpool_add_job函数  
	unsigned int i;  
	for (i = 0; i < threadnum; ++i)  
	{
		pthread_join(pthreads[i], NULL);    //等待线程池的所有线程执行完毕  
	}  
	pthread_mutex_destroy(&(mutex));          //清理资源  
	pthread_cond_destroy(&(condQueueEmpty));  
	pthread_cond_destroy(&(condQueueFull));        
	free(pthreads);  
	return 0;  
}  
ThreadPool::~ThreadPool()  
{
	destroy();  
}  
