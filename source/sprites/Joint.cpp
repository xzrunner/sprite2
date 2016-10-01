#include "Joint.h"
#include "S2_Sprite.h"
#include "DrawNode.h"

namespace s2
{

const float Joint::RADIUS = 10;

Joint::Joint(Sprite* spr, const LocalPose& joint_pose)
	: m_parent(NULL)
	, m_skin(spr, -joint_pose)
{
	s2::WorldPose src(m_skin.spr->GetCenter(), m_skin.spr->GetAngle());
	m_world = s2::local2world(src, joint_pose);
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
	m_world.pos += trans;
	if (m_parent) {
		m_local = world2local(m_parent->m_world, m_world);
	}
	m_skin.Update(this);

	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_children[i]->Translate(trans);
	}
}

void Joint::Rotate(float rot)
{
	m_local.rot += rot;
	m_world.angle += rot;
	m_skin.Update(this);

	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_children[i]->Update();
	}
}

void Joint::Draw(const RenderParams& params) const
{
	DrawNode::Draw(m_skin.spr, params);
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

void Joint::Update()
{
	m_world = s2::local2world(m_parent->GetWorldPose(), m_local);
	m_skin.Update(this);

	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_children[i]->Update();
	}
}

/************************************************************************/
/* class Joint::Skin                                                    */
/************************************************************************/

Joint::Skin::
Skin(Sprite* spr, const LocalPose& skin_pose)
	: spr(spr)
	, pose(skin_pose)
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
	WorldPose dst = local2world(joint->m_world, pose);
	spr->SetAngle(dst.angle);
	spr->Translate(dst.pos - spr->GetCenter());
	spr->UpdateBounding();
}

}