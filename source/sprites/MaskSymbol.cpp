#include "MaskSymbol.h"
#include "SymType.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawMask.h"
#include "DrawNode.h"
#include "BoundingBox.h"

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

void MaskSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	RenderParams p = DrawNode::Prepare(params, spr);
	if (m_base && m_mask) {
		DrawMask::Draw(m_base, m_mask, p);
	} else {
		if (m_base) {
			DrawNode::Draw(m_base, p);
		} else if (m_mask) {
			DrawNode::Draw(m_mask, p);
		}
	}
}

sm::rect MaskSymbol::GetBounding(const Sprite* spr) const
{
	if (m_mask) {
		sm::rect b;
		m_mask->GetBounding()->CombineTo(b);
		return b;
	} else {
		return sm::rect(sm::vec2(0, 0), 100, 100);
	}
}

void MaskSymbol::SetBase(Sprite* base)
{
	cu::RefCountObjAssign(m_base, base);
}

void MaskSymbol::SetMask(Sprite* mask)
{
	cu::RefCountObjAssign(m_mask, mask);
}

}