#ifndef _SPRITE2_ACTOR_INL_
#define _SPRITE2_ACTOR_INL_

namespace s2
{

inline
void Actor::SetFlatten(const std::shared_ptr<ft::Flatten>& ft, int pos)
{ 
	m_flatten.impl = ft;
	m_flatten.pos = pos;
}

inline
bool Actor::SetFlattenDirty()
{
	if (m_flatten.impl) {
		m_flatten.impl->SetDirty();
		return true;
	} else {
		return false;
	}
}

inline
bool Actor::BuildFlatten()
{
	if (!m_flatten.impl || IsFlattenDirty()) {
		m_flatten.impl = std::make_shared<ft::Flatten>(this);
		m_flatten.pos = 0;
		return true;
	} else {
		return false;
	}
}

inline
bool Actor::HasFlatten() const 
{ 
	return m_flatten.impl != nullptr;
}

inline
void Actor::FlattenUpdate(bool force) 
{
	if (m_flatten.impl) {
		m_flatten.impl->Update(m_flatten.pos, force);
	}
}

inline
void Actor::FlattenDraw(const s2::RenderParams& rp) const
{
	if (m_flatten.impl) {
		m_flatten.impl->Draw(m_flatten.pos, rp);
	}
}

inline
void Actor::FlattenSetFrame(int frame)
{
	if (m_flatten.impl) {
		m_flatten.impl->SetFrame(m_flatten.pos, false, frame);
	}
}

}

#endif // _SPRITE2_ACTOR_INL_