#ifndef _SPRITE2_OBJECT_POOL3_H_
#define _SPRITE2_OBJECT_POOL3_H_

namespace s2
{

template <typename T>
class ObjectPool3
{
public:
	T* Pop();
	void Push(T* obj);

	static ObjectPool3<T>* Instance();

private:
	ObjectPool3();
	~ObjectPool3();

private:
	T* m_freelist;

	static ObjectPool3<T>* m_instance;

}; // ObjectPool3

}

#include "ObjectPool3.inl"

#endif // _SPRITE2_OBJECT_POOL3_H_