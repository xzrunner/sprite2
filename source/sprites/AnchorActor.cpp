#include "AnchorActor.h"
#include "S2_Sprite.h"

#include <S2_Sprite.h>
#include <S2_Symbol.h>

namespace s2
{

AnchorActor::AnchorActor(const Sprite* spr, const Actor* parent)
	: Actor(spr, parent) 
	, m_anchor(nullptr)
{
}

AnchorActor::~AnchorActor() 
{
	if (m_anchor) {
		m_anchor->GetSpr()->RemoveReference();
	}
}

void AnchorActor::SetAnchor(const Actor* anchor) 
{
	if (anchor == this) {
		Clear();
		return;
	}

	if (m_anchor != anchor) {
		if (m_anchor) {
			// disconnect
			const_cast<Actor*>(m_anchor)->SetParent(nullptr);
			m_anchor->GetSpr()->RemoveReference();
		}
		m_anchor = anchor;
		if (anchor) {
			anchor->GetSpr()->AddReference();
		}
	}

	if (anchor) 
	{
		GetAABB().SetRect(anchor->GetAABB().GetRect());
		anchor->GetSpr()->ConnectActors(this);
		if (const Actor* parent = anchor->GetParent()) {
			const_cast<ActorAABB&>(parent->GetAABB()).SetRect(sm::rect());	// make it empty
		}
		const_cast<Actor*>(anchor)->GetAABB().UpdateParent(anchor);
	} 
	else 
	{
		GetAABB().Update(this);
	}
}

const Actor* AnchorActor::GetAnchor() const
{
	return m_anchor;
}

void AnchorActor::Clear()
{
	if (m_anchor) 
	{
		// disconnect
		const_cast<Actor*>(m_anchor)->SetParent(nullptr);
		m_anchor->GetSpr()->RemoveReference();
		m_anchor = nullptr;
	}

	// make it empty
	GetAABB().SetRect(sm::rect());
}

}