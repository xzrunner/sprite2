#include "Joint.h"
#include "S2_Sprite.h"
#include "DrawNode.h"

namespace s2
{

const float Joint::RADIUS = 10;

Joint::Joint(const SprPtr& spr, const JointPose& joint_local)
	: m_skin(spr, -joint_local)
{
	if (spr) {
		JointPose src(spr->GetCenter(), spr->GetAngle(), spr->GetScale());
		m_world_pose = local2world(src, joint_local);
	}
}

void Joint::Translate(const sm::vec2& trans)
{
	m_world_pose.trans += trans;
	if (auto p = m_parent.lock()) {
		m_local_pose = world2local(p->m_world_pose, m_world_pose);
	}
	m_skin.Update(this);

	for (int i = 0, n = m_children.size(); i < n; ++i) {
		auto child = m_children[i];
		if (child) {
			child->Translate(trans);
		}
	} 
}

void Joint::Rotate(float rot)
{
	m_local_pose.rot += rot;
	m_world_pose.rot += rot;
	m_skin.Update(this);

	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_children[i]->Update();
	}
}

void Joint::Scale(const sm::vec2& scale)
{
	m_local_pose.scale *= scale;
	m_world_pose.scale *= scale;
	m_skin.Update(this);

	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_children[i]->Update();
	}
}

RenderReturn Joint::Draw(const RenderParams& rp) const
{
	if (m_skin.spr) {
		return DrawNode::Draw(m_skin.spr, rp);
	} else {
		return RENDER_NO_DATA;
	}
}

void Joint::Update()
{
	if (auto p = m_parent.lock()) {
		m_world_pose = local2world(p->GetWorldPose(), m_local_pose);
	}
	m_skin.Update(this);

	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_children[i]->Update();
	}
}

bool Joint::ConnectChild(const std::shared_ptr<Joint>& child)
{
	for (int i = 0, n = child->m_children.size(); i < n; ++i) {
		if (child->m_children[i].get() == this) {
			return false;
		}
	}
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (m_children[i] == child) {
			return true;
		}
	}

	m_children.push_back(child);

	return true;
}

void Joint::DeconnectParent()
{
	auto parent = m_parent.lock();
	if (!parent) {
		return;
	}

	for (int i = 0, n = parent->m_children.size(); i < n; ++i) {
		if (parent->m_children[i].get() == this) {
			parent->m_children.erase(parent->m_children.begin() + i);
			break;
		}
	}

	m_parent.reset();
}

/************************************************************************/
/* class Joint::Skin                                                    */
/************************************************************************/

Joint::Skin::
Skin(const SprPtr& spr, const JointPose& skin_local)
	: spr(spr)
	, skin_local(skin_local)
{
}

void Joint::Skin::
Update(const Joint* joint)
{
	if (!spr) {
		return;
	}
	JointPose dst = local2world(joint->m_world_pose, skin_local);
	spr->SetAngle(dst.rot);
	spr->SetScale(dst.scale);
	spr->SetCenter(dst.trans);
	spr->UpdateBounding();
}

}