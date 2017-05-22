#ifndef _SPRITE2_OBJECT_POOL2_H_
#define _SPRITE2_OBJECT_POOL2_H_

#include <stack>

namespace s2
{

template <typename T>
class ObjectPool2
{
public:
	T* Pop();
	void Push(T* obj);

	static ObjectPool2<T>* Instance();

private:
	ObjectPool2();
	~ObjectPool2();

private:
	std::stack<T*> m_freelist;

	static ObjectPool2<T>* m_instance;

}; // ObjectPool2

}

#include "ObjectPool2.inl"

#endif // _SPRITE2_OBJECT_POOL2_H_