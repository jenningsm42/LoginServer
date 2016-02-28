#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "Handle.h"
#include <vector>

class Dispatcher
{
	public:
		Dispatcher();
		~Dispatcher();

		void initialize(); // Starts threads in thread pool
		void dispatch(std::queue<Job*>& jobQueue);

	private:
		std::vector<Handle*> threadPool;
};

#endif // DISPATCHER_H
