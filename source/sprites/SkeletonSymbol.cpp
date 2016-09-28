#include "SkeletonSymbol.h"
#include "Skeleton.h"
#include "RenderParams.h"
#include "S2_Sprite.h"

namespace s2
{

SkeletonSymbol::SkeletonSymbol()
	: m_skeleton(NULL)
{
}

SkeletonSymbol::~SkeletonSymbol()
{
	if (m_skeleton) {
		m_skeleton->RemoveReference();
	}
}

void SkeletonSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (!m_skeleton) {
		return;
	}

	RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
	}
	m_skeleton->Draw(p);
}

sm::rect SkeletonSymbol::GetBounding(const Sprite* spr) const
{
	return sm::rect(sm::vec2(0, 0), 200, 200);
}

void SkeletonSymbol::SetSkeleton(Skeleton* skeleton)
{
	cu::RefCountObjAssign((Skeleton*)m_skeleton, skeleton);
}

}