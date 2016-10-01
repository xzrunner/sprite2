#include "Skeleton.h"
#include "Joint.h"
#include "RenderParams.h"
#include "BoundingBox.h"
#include "S2_Sprite.h"

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
	for_each(m_all_joints.begin(), m_all_joints.end(), cu::AddRefFonctor<Joint>());
}

Skeleton::~Skeleton()
{
	if (m_root) {
		m_root->RemoveReference();
	}
	for_each(m_all_joints.begin(), m_all_joints.end(), cu::RemoveRefFonctor<Joint>());
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
	for (int i = 0, n = m_all_joints.size(); i < n; ++i) {
		const BoundingBox* bb = m_all_joints[i]->GetSkinSpr()->GetBounding();
		bb->CombineTo(b);
	}
	return b;
}

const Joint* Skeleton::QueryByPos(const sm::vec2& pos) const
{
	for (int i = 0, n = m_all_joints.size(); i < n; ++i) {
		Joint* joint = m_all_joints[i];
		const BoundingBox* bb = m_all_joints[i]->GetSkinSpr()->GetBounding();
		if (bb->IsContain(pos)) {
			return joint;
		}
	}
	return NULL;
}

}