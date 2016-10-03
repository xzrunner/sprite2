#include "MeshJoint.h"
#include "MeshNode.h"
#include "S2_RVG.h"
#include "Color.h"
#include "RenderParams.h"

#include <SM_Calc.h>

#include <algorithm>
#include <queue>

namespace s2
{

const float MeshJoint::RADIUS = 3.5f;

MeshJoint::MeshJoint(MeshJoint* parent, const sm::vec2& pos)
	: m_parent(parent)
	, m_world_pose(pos, 0)
{
	if (m_parent) {
		m_parent->AddReference();
		m_parent->m_children.push_back(this);
		this->AddReference();
	}

	m_world_pose.trans = pos;
	if (m_parent) {
		const sm::vec2& base = m_parent->m_world_pose.trans;
		m_world_pose.rot = m_parent->m_world_pose.rot + sm::get_angle(pos - base, sm::vec2(0, 0), base);
		m_local_pose = world2local(m_parent->m_world_pose, m_world_pose);
	} else {
		m_world_pose.rot = 0;
	}
}

MeshJoint::MeshJoint(MeshJoint* parent, const JointPose& world)
	: m_parent(parent)
	, m_world_pose(world)
{
	if (m_parent) {
		m_parent->AddReference();
		m_parent->m_children.push_back(this);
		this->AddReference();
		m_local_pose = world2local(m_parent->m_world_pose, m_world_pose);		
	}
}

MeshJoint::~MeshJoint()
{
// 	if (m_parent) {
// 		m_parent->RemoveReference();
// 	}
// 	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFunctor<MeshJoint>());
}

void MeshJoint::Rotate(float rot)
{
	m_local_pose.rot += rot;
	m_world_pose.rot += rot;
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_children[i]->Update();
	}
}

void MeshJoint::Draw(const RenderParams& params) const
{
	RVG::SetColor(Color(51, 204, 51, 128));
	RVG::Circle(params.mt * m_world_pose.trans, RADIUS, true);
	RVG::SetColor(Color(204, 51, 51, 128));
	RVG::Circle(params.mt * m_world_pose.trans, RADIUS, false);

	RVG::SetColor(Color(51, 204, 51, 128));
	if (m_parent) {
		RVG::Line(m_parent->GetWorldPose().trans, m_world_pose.trans);
		RVG::Arrow(m_parent->GetWorldPose().trans, m_world_pose.trans, RADIUS * 2);
	} else {
		RVG::Cross(params.mt * m_world_pose.trans, 25, 25);
	}
}

void MeshJoint::Update()
{
	if (m_parent) {
		m_world_pose = local2world(m_parent->GetWorldPose(), m_local_pose);
		for (int i = 0, n = m_nodes.size(); i < n; ++i) {
			Node& node = m_nodes[i];
			node.node->xy = local2world(m_world_pose, node.local).trans;
		}
	}
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_children[i]->Update();
	}
}

void MeshJoint::Deconnect()
{
	if (m_parent) {
		m_parent->RemoveReference();
		for (int i = 0, n = m_parent->m_children.size(); i < n; ++i) {
			if (m_parent->m_children[i] == this) {
				m_parent->m_children.erase(m_parent->m_children.begin() + i);
				this->RemoveReference();
				break;
			}
		}
	}

	std::queue<MeshJoint*> buf;
	this->AddReference();
	buf.push(this);
	while (!buf.empty()) 
	{
		MeshJoint* joint = buf.front(); buf.pop();
		const std::vector<MeshJoint*>& children = joint->GetChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			children[i]->AddReference();
			buf.push(children[i]);
		}
		delete joint;
	}
}

void MeshJoint::BindNode(MeshNode* node, float weight)
{
	JointPose local = world2local(m_world_pose, JointPose(node->ori_xy, 0));
	m_nodes.push_back(Node(node, weight, local));
}

/************************************************************************/
/* class MeshJoint::Node                                                */
/************************************************************************/

MeshJoint::Node::Node(MeshNode* node, float weight, JointPose local)
	: node(node)
	, weight(weight)
	, local(local)
{
// 	if (node) {
// 		node->AddReference();
// 	}
}

MeshJoint::Node::~Node()
{
// 	if (node) {
// 		node->RemoveReference();
// 	}	
}

}