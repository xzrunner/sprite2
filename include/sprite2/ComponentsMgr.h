#pragma once

#include "sprite2/Component.h"

#include <vector>
#include <bitset>
#include <array>
#include <memory>

namespace s2
{

template <size_t CAP>
class ComponentsMgr
{
public:
	ComponentsMgr();
	ComponentsMgr(const ComponentsMgr&);
	ComponentsMgr& operator = (const ComponentsMgr&);

	template <typename T>
	bool HasComponent() const;

	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... args) const;

	template <typename T>
	T& GetComponent() const;

	const std::vector<ComponentPtr>& GetAllComponents() const {
		return m_components;
	}

protected:
	mutable std::vector<ComponentPtr> m_components;

	mutable std::array<uint8_t, CAP> m_component_array;
	mutable std::bitset<CAP>         m_component_bitset;

}; // ComponentsMgr

}

#include "sprite2/ComponentsMgr.inl"