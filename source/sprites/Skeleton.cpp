#include "Skeleton.h"
#include "Joint.h"
#include "RenderParams.h"
#include "BoundingBox.h"
#include "S2_Sprite.h"

#include <SM_Calc.h>

#include <algorithm>

namespace s2
{

Skeleton::Skeleton(const Joint* root, const std::vector<Joint*>& all_joints)
	: m_root(root)
	, m_all_joints(all_joints)
{
	if (m_root) {
		m_root->AddReference();
	}
	for_each(m_all_joints.begin(), m_all_joints.end(), cu::AddRefFunctor<Joint>());
}

Skeleton::~Skeleton()
{
	if (m_root) {
		m_root->RemoveReference();
	}
	for_each(m_all_joints.begin(), m_all_joints.end(), cu::RemoveRefFunctor<Joint>());
}

void Skeleton::Draw(const RenderParams& params) const
{
	for (int i = 0, n = m_all_joints.size(); i < n; ++i) {
		m_all_joints[i]->Draw(params);
	}
}

sm::rect Skeleton::GetBounding() const
{
	sm::rect b;
	for (int i = 0, n = m_all_joints.size(); i < n; ++i) 
	{
		const Joint* joint = m_all_joints[i];
		const Sprite* spr = joint->GetSkinSpr();
		if (spr) {
			spr->GetBounding()->CombineTo(b);
		} else {
			b.Combine(joint->GetWorldPose().trans);
		}
	}
	return b;
}

const Joint* Skeleton::QueryByPos(const sm::vec2& pos) const
{
	for (int i = 0, n = m_all_joints.size(); i < n; ++i) 
	{
		const Joint* joint = m_all_joints[i];
		const Sprite* spr = joint->GetSkinSpr();
		if (spr) {
			if (spr->GetBounding()->IsContain(pos)) {
				return joint;
			}
		} else {
			if (sm::dis_pos_to_pos(pos, joint->GetWorldPose().trans) < Joint::RADIUS * 2) {
				return joint;
			}
		}
	}
	return NULL;
}

}