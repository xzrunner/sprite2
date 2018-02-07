#pragma once

#include <guard/check.h>

namespace s2
{

template <size_t CAP>
ComponentsMgr<CAP>::ComponentsMgr()
{
}

template <size_t CAP>
ComponentsMgr<CAP>::ComponentsMgr(const ComponentsMgr& mgr)
	: m_component_array(mgr.m_component_array)
	, m_component_bitset(mgr.m_component_bitset)
{
	m_components.reserve(mgr.m_components.size());
	for (auto& comp : mgr.m_components) {
		m_components.push_back(ComponentPtr(comp->Clone(), Component::deleter));
	}
}

template <size_t CAP>
ComponentsMgr<CAP>& ComponentsMgr<CAP>::operator = (const ComponentsMgr& mgr)
{
	m_components.clear();
	m_components.reserve(mgr.m_components.size());
	for (auto& comp : mgr.m_components) {
		m_components.push_back(ComponentPtr(comp->Clone(), Component::deleter));
	}
	m_component_bitset = mgr.m_component_bitset;
	m_component_array = mgr.m_component_array;

	return *this;
}

template <size_t CAP>
template <typename T>
bool ComponentsMgr<CAP>::HasComponent() const
{
	return m_component_bitset[GetComponentTypeID<T>()];
}

template <size_t CAP>
template <typename T, typename... TArgs>
T& ComponentsMgr<CAP>::AddComponent(TArgs&&... args) const
{
	GD_ASSERT(!HasComponent<T>(), "already has the component");

	void* buf = mm::AllocHelper::Allocate(sizeof(T));
	T* ptr_raw = new (buf) T(std::forward<TArgs>(args)...);
	auto ptr_unique = std::unique_ptr<T, decltype(&Component::deleter)>(ptr_raw, Component::deleter);

	//auto comp_ptr = std::unique_ptr<T, decltype(&Component::deleter)>(
	//	static_cast<T*>(mm::AllocHelper::New<T>(std::forward<TArgs>(args)...), Component::deleter));
	//auto& comp = *comp_ptr;

	size_t idx = m_components.size();
	GD_ASSERT(idx < CAP, "too many components");
	//m_components.emplace_back(std::move(comp_ptr));
	m_components.emplace_back(std::move(ptr_unique));

	m_component_array[GetComponentTypeID<T>()] = static_cast<uint8_t>(idx);
	m_component_bitset[GetComponentTypeID<T>()] = true;

	//	comp.Init();
	return *ptr_raw;
}

template <size_t CAP>
template <typename T>
T& ComponentsMgr<CAP>::GetComponent() const
{
	GD_ASSERT(HasComponent<T>(), "no component");
	auto ptr(m_components[m_component_array[GetComponentTypeID<T>()]].get());
	return *reinterpret_cast<T*>(ptr);
}

}