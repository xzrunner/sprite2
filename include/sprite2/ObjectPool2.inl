#ifndef _SPRITE2_OBJECT_POOL2_INL_
#define _SPRITE2_OBJECT_POOL2_INL_

#include <stddef.h>

namespace s2
{

template <typename T>
ObjectPool2<T>* ObjectPool2<T>::m_instance = nullptr;

template <typename T>
ObjectPool2<T>* ObjectPool2<T>::Instance()
{
	if (!m_instance) {
		m_instance = new ObjectPool2<T>();
	}
	return m_instance;
}

template <typename T>
ObjectPool2<T>::ObjectPool2()
{
}

template <typename T>
T* ObjectPool2<T>::Pop()
{
	if (m_freelist.empty()) {
		return new T();
	} else {
		T* ret = m_freelist.top();
		m_freelist.pop();
		ret->Init();
		return ret;
	}
}

template <typename T>
void ObjectPool2<T>::Push(T* obj)
{
	obj->Term();
	m_freelist.push(obj);
}

}

#endif // _SPRITE2_OBJECT_POOL2_INL_