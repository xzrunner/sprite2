#include "AnchorActor.h"
#include "S2_Sprite.h"

#include <S2_Sprite.h>
#include <S2_Symbol.h>

namespace s2
{

void AnchorActor::SetAnchor(const ActorPtr& anchor)
{
	// fixme: ?
	if (anchor.get() == this) {
		Clear();
		return;
	}

	if (m_anchor != anchor)
	{
		// disconnect
		if (m_anchor) {
			m_anchor->SetParent(nullptr);
		}
		m_anchor = anchor;
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
	if (m_anchor) 
	{
		// disconnect
		m_anchor->SetParent(nullptr);
		m_anchor = nullptr;
	}

	// make it empty
	GetAABB().SetRect(sm::rect());
}

}