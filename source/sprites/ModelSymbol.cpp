#include "sprite2/ModelSymbol.h"

#ifndef S2_DISABLE_MODEL

#include "sprite2/SymType.h"
#include "sprite2/ModelSprite.h"

#include <node3/RenderParams.h>
#include <node3/PrimitiveDraw.h>

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
 	if (!spr || !m_model) {
 		return RENDER_NO_DATA;
 	}
	auto spr_model = dynamic_cast<const ModelSprite*>(spr);

	auto& pos3 = spr_model->GetPos3();
 	sm::mat4 mat = sm::mat4(spr_model->GetOri3()) *
 		sm::mat4::Translated(pos3.x, pos3.y, pos3.z);
	m_model->Draw(n3::RenderParams(mat));

	n3::PrimitiveDraw::SetColor(0);
	n3::PrimitiveDraw::Cube(mat, m_aabb);

	return RENDER_OK;
}

sm::rect ModelSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	// empty
	return sm::rect();
}

}

#endif // S2_DISABLE_MODEL