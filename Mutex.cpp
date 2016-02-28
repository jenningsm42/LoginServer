#include "Mutex.h"

template <class T>
Mutex<T>::Mutex() : locked(false)
{
}

template <class T>
Mutex<T>::~Mutex()
{
}

template <class T>
bool Mutex<T>::lock()
{
	if(locked) return false;
	locked = true;
	return true;
}

template <class T>
void Mutex<T>::unlock()
{
	if(locked) locked = false;
}

template <class T>
T Mutex<T>::resource()
{
	return res;
}
