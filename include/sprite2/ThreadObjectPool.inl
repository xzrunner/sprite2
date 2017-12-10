#ifndef _SPRITE2_THREAD_OBJECT_POOL_INL_
#define _SPRITE2_THREAD_OBJECT_POOL_INL_

namespace s2
{

template <typename T>
ThreadObjectPool<T>* ThreadObjectPool<T>::m_instance = nullptr;

template <typename T>
ThreadObjectPool<T>* ThreadObjectPool<T>::Instance()
{
	if (!m_instance) {
		m_instance = new ThreadObjectPool<T>();
	}
	return m_instance;
}

template <typename T>
ThreadObjectPool<T>::ThreadObjectPool()
{
}

template <typename T>
ThreadObjectPool<T>::~ThreadObjectPool()
{
	// todo
}

template <typename T>
void ThreadObjectPool<T>::Init(int thread_num)
{
	m_main_thread_id = std::this_thread::get_id();
	if (m_freelists.empty()) {
		m_freelists.resize(thread_num + 1);
	}
}

template <typename T>
T* ThreadObjectPool<T>::Pop(int thread_idx)
{
	assert(thread_idx >= 0 && thread_idx < static_cast<int>(m_freelists.size()));

	auto& freelist = m_freelists[thread_idx];
	if (freelist.list) {
		T* ret = reinterpret_cast<T*>(freelist.list);
		freelist.list = freelist.list->next;
#ifdef S2_THREAD_OBJ_POOL_STAT
		--freelist.free_count;
#endif // S2_THREAD_OBJ_POOL_STAT
		return ret;
	} else {
#ifdef S2_THREAD_OBJ_POOL_STAT
		++freelist.tot_count;
#endif // S2_THREAD_OBJ_POOL_STAT
		return new T();
	}
}

template <typename T>
void ThreadObjectPool<T>::Push(int thread_idx, T* obj)
{
	assert(thread_idx >= 0 && thread_idx < static_cast<int>(m_freelists.size()));

	FreeNode* freenode = reinterpret_cast<FreeNode*>(obj);
	auto& freelist = m_freelists[thread_idx];
	if (freelist.list) {
		freenode->next = freelist.list;
	} else {
		freenode->next = nullptr;
	}
	freelist.list = freenode;
#ifdef S2_THREAD_OBJ_POOL_STAT
	++freelist.free_count;
#endif // S2_THREAD_OBJ_POOL_STAT
}

}

#endif // _SPRITE2_THREAD_OBJECT_POOL_INL_