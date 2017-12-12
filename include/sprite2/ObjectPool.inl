#ifndef _SPRITE2_OBJECT_POOL_INL_
#define _SPRITE2_OBJECT_POOL_INL_

#include <stddef.h>

namespace s2
{

template <typename T>
thread_local ObjectPool<T>* ObjectPool<T>::m_instance = NULL;

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
ObjectPool<T>::~ObjectPool()
{
	// todo
}

template <typename T>
T* ObjectPool<T>::Pop()
{
	if (m_freelist) {
		T* ret = reinterpret_cast<T*>(m_freelist);
		m_freelist = m_freelist->next;
		--m_free_count;
		return ret;
	} else {
		++m_tot_count;
		return new T();
	}
}

template <typename T>
void ObjectPool<T>::Push(T* obj)
{
	FreeNode* freenode = reinterpret_cast<FreeNode*>(obj);
	if (m_freelist) {
		freenode->next = m_freelist;
	} else {
		freenode->next = nullptr;
	}
	m_freelist = freenode;
	++m_free_count;
}

}

#endif // _SPRITE2_OBJECT_POOL_INL_