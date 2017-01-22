#ifndef _SPRITE2_OBJECT_POOL_INL_
#define _SPRITE2_OBJECT_POOL_INL_

#include <stddef.h>

namespace s2
{

template <typename T>
ObjectPool<T>* ObjectPool<T>::m_instance = NULL;

template <typename T>
ObjectPool<T>* ObjectPool<T>::Instance()
{
	if (!m_instance) {
		m_instance = new ObjectPool<T>();
	}
	return m_instance;
}

template <typename T>
ObjectPool<T>::ObjectPool()
{
	m_freelist = NULL;
}

template <typename T>
T* ObjectPool<T>::Pop()
{
	if (m_freelist) {
		T* ret = m_freelist;
		m_freelist = m_freelist->GetNext();
		ret->Init();
		return ret;
	} else {
		return new T();
	}
}

template <typename T>
void ObjectPool<T>::Push(T* obj)
{
	obj->Term();
	if (m_freelist) {
		obj->SetNext(m_freelist);
	} else {
		obj->SetNext(NULL);
	}
	m_freelist = obj;
}

}

#endif // _SPRITE2_OBJECT_POOL_INL_