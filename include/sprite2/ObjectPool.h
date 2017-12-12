#ifndef _SPRITE2_OBJECT_POOL_H_
#define _SPRITE2_OBJECT_POOL_H_

#include <assert.h>

namespace s2
{

template <typename T>
class ObjectPool
{
public:
	T*   Pop();
	void Push(T* obj);

	static ObjectPool<T>* Instance();

private:
	ObjectPool();
	~ObjectPool();

private:
	struct FreeNode
	{
		FreeNode* next;
	};

private:
	FreeNode* m_freelist;

	int m_free_count = 0;
	int m_tot_count = 0;

	thread_local static ObjectPool<T>* m_instance;

}; // ObjectPool

template <typename T>
class ObjProxy
{
public:
	ObjProxy()
		: obj(ObjectPool<T>::Instance()->Pop())
	{}
	~ObjProxy() {
		assert(obj);
		ObjectPool<T>::Instance()->Push(obj);
	}

	T* obj;

}; // ObjProxy

}

#include "sprite2/ObjectPool.inl"

#endif // _SPRITE2_OBJECT_POOL_H_