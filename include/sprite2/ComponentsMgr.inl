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
		m_components.push_back(std::unique_ptr<Component>(comp->Clone()));
	}
}

template <size_t CAP>
ComponentsMgr<CAP>& ComponentsMgr<CAP>::operator = (const ComponentsMgr& mgr)
{
	m_components.clear();
	m_components.reserve(mgr.m_components.size());
	for (auto& comp : mgr.m_components) {
		m_components.push_back(std::unique_ptr<Component>(comp->Clone()));
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

	auto comp_ptr = std::unique_ptr<T>(static_cast<T*>(mm::AllocHelper::New<T>(std::forward<TArgs>(args)...)));
	auto& comp = *comp_ptr;
	size_t idx = m_components.size();
	GD_ASSERT(idx < CAP, "too many components");
	m_components.emplace_back(std::move(comp_ptr));

	m_component_array[GetComponentTypeID<T>()] = static_cast<uint8_t>(idx);
	m_component_bitset[GetComponentTypeID<T>()] = true;

	//	comp.Init();
	return comp;
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