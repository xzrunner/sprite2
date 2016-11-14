#include "Anim2Symbol.h"
#include "Anim2Sprite.h"
#include "SymType.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawNode.h"

#include <rigging/rg_skeleton.h>
#include <rigging/rg_joint.h>
#include <rigging/rg_animation.h>
#include <rigging/rg_dopesheet.h>
#include <rigging/rg_skeleton_pose.h>

#include <assert.h>

namespace s2
{

Anim2Symbol::Anim2Symbol()
	: m_anim(NULL)
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
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
	}

	//////////////////////////////////////////////////////////////////////////

//	rg_skeleton_draw(m_anim->sk, &p);

	//////////////////////////////////////////////////////////////////////////

	const Anim2Sprite* anim_spr = VI_DOWNCASTING<const Anim2Sprite*>(spr);
	const rg_skeleton_pose* sk_pose = const_cast<Anim2Sprite*>(anim_spr)->GetAnimCurr().GetSkPose();
	for (int i = 0; i < m_anim->sk->joint_count; ++i) 
	{
		int skin_idx = 0xffff;
		if (sk_pose->poses[i].skin != 0xffff) {
			skin_idx = sk_pose->poses[i].skin;
		} else {
			const struct rg_joint* joint = m_anim->sk->joints[i];
			if (joint->skin >= 0 && joint->skin < m_anim->sk->skin_count) {
				skin_idx = joint->skin;
			}
		}
		if (skin_idx == 0xffff) {
			continue;
		}

		const struct rg_skin* skin = &m_anim->sk->skins[skin_idx];
		assert(skin->ud);

		struct rg_joint_pose world;
		rg_local2world(&sk_pose->poses[i].world, &skin->local, &world);

		Symbol* s2_sym = static_cast<Symbol*>(skin->ud);
		DrawNode::Draw(s2_sym, p, sm::vec2(world.trans[0], world.trans[1]), 
			world.rot, sm::vec2(world.scale[0], world.scale[1]), sm::vec2(0, 0));
	}

	//////////////////////////////////////////////////////////////////////////
}

sm::rect Anim2Symbol::GetBounding(const Sprite* spr) const
{
	if (!m_anim) {
		return sm::rect(200, 200);
	}

	sm::rect b;
	for (int i = 0; i < m_anim->sk->joint_count; ++i) 
	{
		const struct rg_joint* joint = m_anim->sk->joints[i];
		if (joint->skin == 0xffff) {
			continue;
		}
		assert(joint->skin >= 0 && joint->skin < m_anim->sk->skin_count);
		const struct rg_skin* skin = &m_anim->sk->skins[joint->skin];
		if (!skin->ud) {
			continue;
		}

		rg_joint_pose world;
		rg_local2world(&joint->world_pose, &skin->local, &world);
		
		Symbol* sym = static_cast<Symbol*>(skin->ud);
		sm::rect sb = sym->GetBounding();

		sm::mat4 t;
		t.SetTransformation(world.trans[0], world.trans[1], world.rot, world.scale[0], world.scale[1], 0, 0, 0, 0);
		sm::vec2 min(sb.xmin, sb.ymin),
			     max(sb.xmax, sb.ymax);
		min = t * min;
		max = t * max;

		b.Combine(min);
		b.Combine(max);
	}
	return b;	
}

}