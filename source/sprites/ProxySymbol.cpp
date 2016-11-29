#include "ProxySymbol.h"
#include "SymType.h"
#include "S2_Sprite.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "BoundingBox.h"

namespace s2
{

ProxySymbol::ProxySymbol()
	: m_spr(NULL)
{
}

ProxySymbol::~ProxySymbol()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

int ProxySymbol::Type() const 
{ 
	return SYM_PROXY; 
}

void ProxySymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (!m_spr) {
		return;
	}

	RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->GetColor() * params.color;			
	}

	DrawNode::Draw(m_spr, p);
}

sm::rect ProxySymbol::GetBounding(const Sprite* spr) const
{
	sm::rect b;
	if (m_spr) {
		m_spr->GetBounding()->CombineTo(b);
	}
	return b;
}

}