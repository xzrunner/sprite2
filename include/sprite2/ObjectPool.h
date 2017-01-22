#ifndef _SPRITE2_OBJECT_POOL_H_
#define _SPRITE2_OBJECT_POOL_H_

namespace s2
{

template <typename T>
class ObjectPool
{
public:
	T* Pop();
	void Push(T* obj);

	static ObjectPool<T>* Instance();

private:
	ObjectPool();
	~ObjectPool();

private:
	T* m_freelist;

	static ObjectPool<T>* m_instance;

}; // ObjectPool

}

#include "ObjectPool.inl"

#endif // _SPRITE2_OBJECT_POOL_H_