#include "Anim2Symbol.h"
#include "Anim2Sprite.h"
#include "SymType.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawNode.h"

#include <rigging.h>

#include <assert.h>
#include <string.h>

namespace s2
{

Anim2Symbol::Anim2Symbol()
	: m_anim(NULL)
{
}

Anim2Symbol::Anim2Symbol(uint32_t id)
	: Symbol(id)
	, m_anim(NULL)
{
}

Anim2Symbol::~Anim2Symbol()
{
	// todo release sk
}
	
int Anim2Symbol::Type() const
{
	return SYM_ANIM2;
}

void Anim2Symbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	if (!m_anim || !spr) {
		return;
	}

	RenderParams rp_child(rp);
	if (!DrawNode::Prepare(rp, spr, rp_child)) {
		return;
	}

	const Anim2Sprite* anim_spr = VI_DOWNCASTING<const Anim2Sprite*>(spr);
	const Anim2Curr& curr = const_cast<Anim2Sprite*>(anim_spr)->GetAnimCurr();
	rg_skeleton_draw(m_anim->sk, curr.GetSkPose(), curr.GetSkSkin(), &rp_child);
}

sm::rect Anim2Symbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	if (!m_anim) {
		return sm::rect(200, 200);
	}

	sm::rect b;
	for (int i = 0; i < m_anim->sk->slot_count; ++i) 
	{
		const struct rg_slot* slot = &m_anim->sk->slots[i];
		const struct rg_joint* joint = m_anim->sk->joints[slot->joint];
		assert(joint);
		if (slot->skin == RG_SKIN_UNKNOWN || slot->skin == RG_SKIN_NULL) {
			continue;
		}
		const struct rg_skin* skin = &m_anim->sk->skins[slot->skin];
		assert(skin && skin->ud);

		rg_pose_mat world;
		rg_local2worldmat(&joint->world_pose, &skin->local, &world);

		Symbol* sym = static_cast<Symbol*>(skin->ud);
		sm::rect sb = sym->GetBounding();
		if (sb.Width() == 0 || sb.Height() == 0) {
			continue;
		}

#ifdef S2_MATRIX_FIX
		sm::MatrixFix m;
		m.x[0] = world.m[0] * sm::MatrixFix::SCALE;
		m.x[1] = world.m[1] * sm::MatrixFix::SCALE;
		m.x[2] = world.m[2] * sm::MatrixFix::SCALE;
		m.x[3] = world.m[3] * sm::MatrixFix::SCALE;
		m.x[4] = world.m[4] * sm::MatrixFix::TRANSLATE_SCALE;
		m.x[5] = world.m[5] * sm::MatrixFix::TRANSLATE_SCALE;
#else
		sm::Matrix2D m;
		memcpy(m.x, world.m, sizeof(m.x));
#endif // S2_MATRIX_FIX

		sm::vec2 min(sb.xmin, sb.ymin),
			     max(sb.xmax, sb.ymax);
		min = m * min;
		max = m * max;

		b.Combine(min);
		b.Combine(max);
	}

	return b;	
}

}