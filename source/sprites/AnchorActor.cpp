#include "AnchorActor.h"
#include "S2_Sprite.h"

namespace s2
{

AnchorActor::AnchorActor(const Sprite* spr, const Actor* parent)
	: Actor(spr, parent) 
	, m_anchor(NULL)
{
}

AnchorActor::~AnchorActor() 
{
	if (m_anchor) {
		m_anchor->RemoveReference();
	}
}

void AnchorActor::SetAnchor(const Sprite* anchor) 
{
	cu::RefCountObjAssign(m_anchor, anchor);
}

const Sprite* AnchorActor::GetAnchor() const
{
	return m_anchor;
}

}