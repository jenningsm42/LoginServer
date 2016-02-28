#include "Dispatcher.h"

Dispatcher::Dispatcher()
{
}

Dispatcher::~Dispatcher()
{
	for(unsigned int i = 0; i < threadPool.size(); i++)
	{
		if(threadPool[i]) delete threadPool[i];
	}
	threadPool.clear();
}

void Dispatcher::initialize()
{
	unsigned int nthreads = std::thread::hardware_concurrency()-1; // not including main
	for(unsigned int i = 0; i < nthreads; i++)
	{
		threadPool.push_back(new Handle());
		threadPool[i]->initialize();
	}
}

void Dispatcher::dispatch(std::queue<Job*>& jobQueue)
{
	while(!jobQueue.empty())
	{
		int min_idx = 0;
		for(unsigned int i = 1; i < threadPool.size(); i++)
		{
			if(threadPool[i]->getJobCount() < threadPool[i]->getJobCount())
				min_idx = i;
		}

		printf("Added job to thread #%d\n", min_idx);
		threadPool[min_idx]->addJob(jobQueue.front());
		jobQueue.pop();
	}
}
