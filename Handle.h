#ifndef HANDLE_H
#define HANDLE_H
#include "Job.h"
#include <thread>
#include <queue>
#include <mysql/mysql.h>

class Handle
{
	public:
		Handle();
		~Handle();

		bool initialize();

		void addJob(Job* j);

		int getJobCount();

	private:
		std::thread t;
		std::queue<Job*> jobQueue;
		bool running;
		MYSQL* con;

		void run();
};

#endif // HANDLE_H
