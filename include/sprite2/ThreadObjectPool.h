#ifndef _SPRITE2_THREAD_OBJECT_POOL_H_
#define v_SPRITE2_THREAD_OBJECT_POOL_H_

#include "sprite2/Callback.h"

#include <vector>

#include <assert.h>

#define S2_THREAD_OBJ_POOL_STAT

namespace s2
{

template <typename T>
class ThreadObjectPool
{
public:
	void Init(int thread_num);

	T*   Pop(int thread_idx);
	void Push(int thread_idx, T* obj);

	bool IsMainThreadID(std::thread::id id) const { return id == m_main_thread_id; }
	int  GetFreelistCount() const { return m_freelists.size(); }

	static ThreadObjectPool<T>* Instance();

private:
	ThreadObjectPool();
	~ThreadObjectPool();

private:
	struct FreeNode
	{
		FreeNode* next;
	};

	struct FreeList
	{
		FreeNode* list = nullptr;

#ifdef S2_THREAD_OBJ_POOL_STAT
		int free_count = 0;
		int tot_count  = 0;
#endif // S2_THREAD_OBJ_POOL_STAT
	};

private:
	std::vector<FreeList> m_freelists;

	std::thread::id m_main_thread_id;

	static ThreadObjectPool<T>* m_instance;

}; // ThreadObjectPool

template <typename T>
class ThreadObjProxy
{
public:
	ThreadObjProxy()
	{
		int thread_idx = -1;
		//auto thread_id = std::this_thread::get_id();
		auto pool = ThreadObjectPool<T>::Instance();
		//if (pool->IsMainThreadID(thread_id)) {
			thread_idx = pool->GetFreelistCount() - 1;
		//} else {
		//	thread_idx = Callback::QueryThreadIdx(thread_id);
		//}
		obj = pool->Pop(thread_idx);
		m_thread_idx = thread_idx;
	}
	ThreadObjProxy(int thread_idx)
		: obj(ThreadObjectPool<T>::Instance()->Pop(thread_idx))
		, m_thread_idx(thread_idx)
	{
	}
	~ThreadObjProxy() {
		assert(obj);
		ThreadObjectPool<T>::Instance()->Push(m_thread_idx, obj);
	}

public:
	T* obj;

private:
	int m_thread_idx;

}; // ThreadObjProxy

}

#include "sprite2/ThreadObjectPool.inl"

#endif // _SPRITE2_THREAD_OBJECT_POOL_H_