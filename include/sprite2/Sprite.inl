#ifndef _SPRITE2_SPRITE_INL_
#define _SPRITE2_SPRITE_INL_

#include "sprite2/SprNameMap.h"

#include <guard/check.h>
#include <painting2/OBB.h>

namespace s2
{

inline
void Sprite::SetName(const CU_STR& name) 
{
	m_name = SprNameMap::Instance()->StrToID(name);	
	UpdateInheritUpdate();
}

template <typename T>
bool Sprite::HasComponent() const
{
	return m_component_bitset[GetComponentTypeID<T>()];
}

template <typename T, typename... TArgs>
T& Sprite::AddComponent(TArgs&&... args)
{
	GD_ASSERT(!HasComponent<T>(), "already has the component");

	auto comp_ptr = std::unique_ptr<T>(static_cast<T*>(mm::AllocHelper::New<T>(std::forward<TArgs>(args)...)));
	auto& comp = *comp_ptr;
	size_t idx = m_components.size();
	GD_ASSERT(idx < MAX_COMPONENTS, "too many components");
	m_components.emplace_back(std::move(comp_ptr));

	m_component_array[GetComponentTypeID<T>()] = static_cast<uint8_t>(idx);
	m_component_bitset[GetComponentTypeID<T>()] = true;

	//	comp.Init();
	return comp;
}

template <typename T>
T& Sprite::GetComponent() const
{
	GD_ASSERT(HasComponent<T>(), "no component");
	auto ptr(m_components[m_component_array[GetComponentTypeID<T>()]].get());
	return *reinterpret_cast<T*>(ptr);
}

inline
void Sprite::UpdateInheritUpdate() const
{
	if (m_name == -1) {
		SetInheritUpdate(false);
	} else {
		CU_STR name;
		SprNameMap::Instance()->IDToStr(m_name, name);
		SetInheritUpdate(IsForceUpdate() || name.empty() || name[0] == '_');
	}
}

inline
void Sprite::CreateBounding() const
{
	void* ptr = mm::AllocHelper::Allocate(sizeof(pt2::OBB));
	pt2::BoundingBox* bb = new (ptr) pt2::OBB();
	m_bounding.reset(bb);
}

inline
void Sprite::SetForceUpdate(bool flag) const
{
	if (flag) {
		m_flags |= FLAG_FORCE_UPDATE;
	} else {
		m_flags &= ~FLAG_FORCE_UPDATE;
	}
	UpdateInheritUpdate();
}

}

#endif // _SPRITE2_SPRITE_INL_