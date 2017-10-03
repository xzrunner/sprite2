#include "AnchorActor.h"
#include "S2_Sprite.h"

#include <S2_Sprite.h>
#include <S2_Symbol.h>

namespace s2
{

void AnchorActor::SetAnchor(const ActorPtr& anchor)
{
	// anchor mount itself mean clean
	if (anchor.get() == this) {
		Clear();
		return;
	}

	if (m_anchor.actor != anchor)
	{
		// disconnect
		if (m_anchor.actor) {
			m_anchor.actor->SetParent(nullptr);
		}
		m_anchor.actor = anchor;
		// cache spr
		m_anchor.spr = anchor->GetSpr();
	}

	if (anchor) 
	{
		GetAABB().SetRect(anchor->GetAABB().GetRect());
		anchor->GetSpr()->ConnectActors(shared_from_this());

		auto& parent = anchor->GetParent();
		if (parent) {
			const_cast<ActorAABB&>(parent->GetAABB()).SetRect(sm::rect());	// make it empty
		}
		anchor->GetAABB().UpdateParent(anchor);
	} 
	else 
	{
		GetAABB().Update(shared_from_this());
	}
}

void AnchorActor::Clear()
{
	if (m_anchor.actor) 
	{
		// disconnect
		m_anchor.actor->SetParent(nullptr);
		m_anchor.actor.reset();
		m_anchor.spr.reset();
	}

	// make it empty
	GetAABB().SetRect(sm::rect());
}

}