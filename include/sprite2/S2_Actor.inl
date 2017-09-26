#ifndef _SPRITE2_ACTOR_INL_
#define _SPRITE2_ACTOR_INL_

#include "sprite2/RenderParams.h"

#include <flatten/FTList.h>

namespace s2
{

inline
void Actor::SetFlatten(const std::shared_ptr<ft::FTList>& ft, int pos)
{ 
	m_flatten.list = ft;
	m_flatten.pos = pos;
}

inline
bool Actor::SetFlattenDirty()
{
	if (m_flatten.list) {
		m_flatten.list->SetDirty();
		return true;
	} else {
		return false;
	}
}

inline
bool Actor::BuildFlatten()
{
	if (!m_flatten.list || IsFlattenDirty()) {
		m_flatten.list = std::make_shared<ft::FTList>(this);
		m_flatten.pos = 0;
		return true;
	} else {
		return false;
	}
}

inline
bool Actor::HasFlatten() const 
{ 
	return m_flatten.list != nullptr;
}

inline
void Actor::FlattenUpdate(bool force) 
{
	if (m_flatten.list) {
		m_flatten.list->Update(m_flatten.pos, force, m_dlist);
	}
}

inline
void Actor::FlattenDraw(const s2::RenderParams& rp) const
{
	if (m_flatten.list) {
		if (rp.IsEnableDrawlist()) {
			m_flatten.list->DrawDeferred(m_flatten.pos, rp, m_dlist);
		} else {
			m_flatten.list->DrawForward(m_flatten.pos, rp);
		}
	}
}

inline
void Actor::FlattenSetFrame(int frame)
{
	if (m_flatten.list) {
		m_flatten.list->SetFrame(m_flatten.pos, false, frame, m_dlist);
	}
}

inline
void Actor::SetDisplayList(const std::shared_ptr<cooking::DisplayList>& dlist)
{
	m_dlist = dlist;
}

}

#endif // _SPRITE2_ACTOR_INL_