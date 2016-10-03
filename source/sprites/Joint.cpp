#include "Joint.h"
#include "S2_Sprite.h"
#include "DrawNode.h"

namespace s2
{

const float Joint::RADIUS = 10;

Joint::Joint(Sprite* spr, const JointPose& joint_local)
	: m_parent(NULL)
	, m_skin(spr, -joint_local)
{
	if (Sprite* spr = m_skin.spr) {
		JointPose src(spr->GetCenter(), spr->GetAngle());
		m_world_pose = local2world(src, joint_local);
	}
}

Joint::~Joint()
{
	if (m_parent) {
		m_parent->RemoveReference();
	}
	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFonctor<Joint>());
}

void Joint::Translate(const sm::vec2& trans)
{
	m_world_pose.trans += trans;
	if (m_parent) {
		m_local_pose = world2local(m_parent->m_world_pose, m_world_pose);
	}
	m_skin.Update(this);

	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_children[i]->Translate(trans);
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

void Joint::Draw(const RenderParams& params) const
{
	if (m_skin.spr) {
		DrawNode::Draw(m_skin.spr, params);
	}
}

void Joint::Update()
{
	if (m_parent) {
		m_world_pose = local2world(m_parent->GetWorldPose(), m_local_pose);
	}
	m_skin.Update(this);

	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_children[i]->Update();
	}
}

bool Joint::ConnectChild(Joint* child)
{
	for (int i = 0, n = child->m_children.size(); i < n; ++i) {
		if (child->m_children[i] == this) {
			return false;
		}
	}
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (m_children[i] == child) {
			return true;
		}
	}

	child->AddReference();
	m_children.push_back(child);

	cu::RefCountObjAssign(child->m_parent, (Joint*)this);

	return true;
}

void Joint::DeconnectParent()
{
	if (!m_parent) {
		return;
	}

	for (int i = 0, n = m_parent->m_children.size(); i < n; ++i) {
		if (m_parent->m_children[i] == this) {
			this->RemoveReference();
			m_parent->m_children.erase(m_parent->m_children.begin() + i);
			break;
		}
	}

	m_parent->RemoveReference();
	m_parent = NULL;
}

/************************************************************************/
/* class Joint::Skin                                                    */
/************************************************************************/

Joint::Skin::
Skin(Sprite* spr, const JointPose& skin_local)
	: spr(spr)
	, skin_local(skin_local)
{
	if (this->spr) {
		this->spr->AddReference();
	}
}

Joint::Skin::
~Skin()
{
	if (spr) {
		spr->RemoveReference();
	}
}

void Joint::Skin::
Update(const Joint* joint)
{
	if (spr) {
		JointPose dst = local2world(joint->m_world_pose, skin_local);
		spr->SetAngle(dst.rot);
		spr->Translate(dst.trans - spr->GetCenter());
		spr->UpdateBounding();
	}
}

}