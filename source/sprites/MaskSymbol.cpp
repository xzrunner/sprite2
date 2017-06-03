#include "MaskSymbol.h"
#include "SymType.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawMask.h"
#include "DrawNode.h"
#include "BoundingBox.h"
#include "SymbolVisitor.h"

namespace s2
{

MaskSymbol::MaskSymbol()
	: m_base(NULL)
	, m_mask(NULL)
{
}

MaskSymbol::MaskSymbol(uint32_t id)
	: Symbol(id)
	, m_base(NULL)
	, m_mask(NULL)
{
}

MaskSymbol::~MaskSymbol()
{
	if (m_base) {
		m_base->RemoveReference();
	}
	if (m_mask) {
		m_mask->RemoveReference();
	}
}

int MaskSymbol::Type() const 
{ 
	return SYM_MASK; 
}

void MaskSymbol::Traverse(const SymbolVisitor& visitor)
{
	if (m_base) {
		visitor.Visit(m_base);
	}
	if (m_mask) {
		visitor.Visit(m_mask);
	}
}

RenderReturn MaskSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		RenderParamsPool::Instance()->Push(rp_child); 
		return RENDER_INVISIBLE;
	}
	RenderReturn ret = RENDER_OK;
	if (m_base && m_mask) {
		ret = DrawMask::Draw(m_base, m_mask, *rp_child);
//		ret = DrawMask::DrawByStencil(m_base, m_mask, *rp_child);
	} else {
		if (m_base) {
			rp_child->actor = m_base->QueryActor(rp.actor);
			ret = DrawNode::Draw(m_base, *rp_child);
		} else if (m_mask) {
			rp_child->actor = m_mask->QueryActor(rp.actor);
			ret = DrawNode::Draw(m_mask, *rp_child);
		}
	}
	RenderParamsPool::Instance()->Push(rp_child); 
	return ret;
}

void MaskSymbol::SetBase(Sprite* base)
{
	cu::RefCountObjAssign(m_base, base);
}

void MaskSymbol::SetMask(Sprite* mask)
{
	cu::RefCountObjAssign(m_mask, mask);
}

sm::rect MaskSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	if (m_mask) {
		sm::rect b;
		m_mask->GetBounding()->CombineTo(b);
		return b;
	} else {
		return sm::rect(sm::vec2(0, 0), 100, 100);
	}
}

}