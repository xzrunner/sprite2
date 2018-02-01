#include "sprite2/Skeleton.h"
#include "sprite2/Joint.h"
#include "sprite2/RenderParams.h"
#include "sprite2/Sprite.h"

#include <SM_Calc.h>

#include <algorithm>

namespace s2
{

Skeleton::Skeleton(const std::shared_ptr<Joint>& root, const CU_VEC<std::shared_ptr<Joint>>& all_joints)
	: m_root(root)
	, m_all_joints(all_joints)
{
}

RenderReturn Skeleton::Draw(cooking::DisplayList* dlist, const RenderParams& rp) const
{
	RenderReturn ret = RENDER_OK;
	for (int i = 0, n = m_all_joints.size(); i < n; ++i) {
		ret |= m_all_joints[i]->Draw(dlist, rp);
	}
	return ret;
}

sm::rect Skeleton::GetBounding() const
{
	sm::rect b;
	for (auto& joint : m_all_joints) 
	{
		auto& spr = joint->GetSkinSpr();
		if (spr) {
			spr->GetBounding().CombineTo(b);
		} else {
			b.Combine(joint->GetWorldPose().trans);
		}
	}
	return b;
}

std::shared_ptr<Joint> Skeleton::QueryByPos(const sm::vec2& pos) const
{
	for (auto& joint : m_all_joints) 
	{
		auto& spr = joint->GetSkinSpr();
		if (spr) {
			if (spr->GetBounding().IsContain(pos)) {
				return joint;
			}
		} else {
			if (sm::dis_pos_to_pos(pos, joint->GetWorldPose().trans) < Joint::RADIUS * 2) {
				return joint;
			}
		}
	}
	return nullptr;
}

}