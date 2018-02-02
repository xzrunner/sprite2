#pragma once

#include <cu/uncopyable.h>

namespace s2
{

using ComponentID = size_t;

namespace Internal
{
inline ComponentID GetUniqueComponentID() noexcept
{
    static ComponentID id{0u};
    return id++;
}
}

template <typename T>
inline ComponentID GetComponentTypeID() noexcept
{
    static_assert(std::is_base_of<Component, T>::value,
        "T must inherit from Component");

    static ComponentID type_id{Internal::GetUniqueComponentID()};
    return type_id;
}

class Component : private cu::Uncopyable
{
public:
	virtual ~Component() {}

	virtual Component* Clone() const = 0;

	//virtual const char* Type() const = 0;

	//virtual bool StoreToJson(rapidjson::Value& val, 
	//	rapidjson::MemoryPoolAllocator<>& alloc) const { return false; }
	//virtual void LoadFromJson(const rapidjson::Value& val) {}

}; // Component

}