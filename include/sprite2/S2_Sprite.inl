#ifndef _SPRITE2_SPRITE_INL_
#define _SPRITE2_SPRITE_INL_

#include "SprNameMap.h"
#include "OBB.h"

namespace s2
{

inline
void Sprite::SetName(const CU_STR& name) 
{
	m_name = SprNameMap::Instance()->StrToID(name);	
	UpdateInheritUpdate();
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
	void* ptr = mm::AllocHelper::Allocate(sizeof(OBB));
	BoundingBox* bb = new (ptr) OBB();
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