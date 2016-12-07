#include "Anim2Symbol.h"
#include "Anim2Sprite.h"
#include "SymType.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawNode.h"

#include <rigging.h>

#include <assert.h>

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

void Anim2Symbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (!m_anim) {
		return;
	}

	RenderParams p = params;
	if (spr) {
		p.mt = spr->GetLocalMat() * params.mt;
		p.color = spr->GetColor() * params.color;
	}

	const Anim2Sprite* anim_spr = VI_DOWNCASTING<const Anim2Sprite*>(spr);
	const Anim2Curr& curr = const_cast<Anim2Sprite*>(anim_spr)->GetAnimCurr();
	rg_skeleton_draw(m_anim->sk, curr.GetSkPose(), curr.GetSkSkin(), &p);
}

sm::rect Anim2Symbol::GetBounding(const Sprite* spr) const
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

		sm::mat4 m;
		m.c[0][0] = world.m[0]; m.c[0][1] = world.m[1];
		m.c[1][0] = world.m[2]; m.c[1][1] = world.m[3];
		m.c[3][0] = world.m[4]; m.c[3][1] = world.m[5];

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