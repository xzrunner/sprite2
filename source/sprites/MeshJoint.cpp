#include "MeshJoint.h"
#include "MeshNode.h"
#include "S2_RVG.h"
#include "Color.h"
#include "RenderParams.h"

#include <algorithm>
#include <queue>

namespace s2
{

const float MeshJoint::RADIUS = 5;

MeshJoint::MeshJoint(MeshJoint* parent, const sm::vec2& pos)
	: m_parent(parent)
	, m_world_pose(pos, 0)
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
// 	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFonctor<MeshJoint>());
}

void MeshJoint::Draw(const RenderParams& params) const
{
	RVG::SetColor(Color(51, 204, 51, 128));
	RVG::Circle(params.mt * m_world_pose.trans, RADIUS, true);
	RVG::SetColor(Color(204, 51, 51, 128));
	RVG::Circle(params.mt * m_world_pose.trans, RADIUS, false);

	RVG::SetColor(Color(51, 204, 51, 128));
	if (m_parent) {
		RVG::Line(m_parent->GetWorldPos(), m_world_pose.trans);
		RVG::Arrow(m_parent->GetWorldPos(), m_world_pose.trans, RADIUS * 2);
	} else {
		RVG::Cross(params.mt * m_world_pose.trans, 25, 25);
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

/************************************************************************/
/* class MeshJoint::Node                                                */
/************************************************************************/

MeshJoint::Node::Node(MeshNode* node, float weight, sm::vec2 local)
	: node(node)
	, weight(weight)
	, local(local)
{
}

MeshJoint::Node::~Node()
{
	if (node) {
		node->RemoveReference();
	}	
}

}