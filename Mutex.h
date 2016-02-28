#ifndef MUTEX_H
#define MUTEX_H

template <class T>
class Mutex
{
	public:
		Mutex();
		~Mutex();

		bool lock();
		void unlock();

		T resource();

	private:
		bool locked;
		T res;
};

#endif // MUTEX_H
