#include "sprite2/ModelSymbol.h"

#ifndef S2_DISABLE_MODEL

#include "sprite2/SymType.h"

#include <node3/RenderParams.h>

namespace s2
{

ModelSymbol::ModelSymbol()
{
}

int ModelSymbol::Type() const
{
	return SYM_MODEL;
}

RenderReturn ModelSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
// 	if (!spr) {
// 		return;
// 	}
//	const Sprite* s = dynamic_cast<const Sprite*>(spr);

// 	sm::mat4 mat = sm::mat4(s->GetOri3().ToMatrix()) * 
// 		sm::mat4::Translate(s->GetPos3().x, s->GetPos3().y, s->GetPos3().z);
// 	e3d::DrawCube(mat, m_aabb, ee::BLACK);

	if (m_model) {
		m_model->Draw(n3::RenderParams());
	}

	return RENDER_OK;
}

sm::rect ModelSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	// empty
	return sm::rect();
}

}

#endif // S2_DISABLE_MODEL