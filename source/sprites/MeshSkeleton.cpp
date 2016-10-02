#include "MeshSkeleton.h"
#include "MeshJoint.h"

#include <SM_Calc.h>

#include <algorithm>
#include <queue>

namespace s2
{

MeshSkeleton::MeshSkeleton()
	: m_root(NULL)
{
}

MeshSkeleton::~MeshSkeleton()
{
	if (m_root) {
		m_root->RemoveReference();
	}
}

void MeshSkeleton::Draw(const RenderParams& params) const
{
	if (!m_root) {
		return;
	}

	std::queue<MeshJoint*> buf;
	buf.push(m_root);
	while (!buf.empty()) 
	{
		MeshJoint* joint = buf.front(); buf.pop();
		joint->Draw(params);
		const std::vector<MeshJoint*>& children = joint->GetChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			buf.push(children[i]);
		}
	}
}

void MeshSkeleton::SetRoot(const MeshJoint* joint) 
{ 
	cu::RefCountObjAssign(m_root, joint); 
}

const MeshJoint* MeshSkeleton::QueryByPos(const sm::vec2& pos) const
{
	if (!m_root) {
		return NULL;
	}

	std::queue<MeshJoint*> buf;
	buf.push(m_root);
	while (!buf.empty()) 
	{
		MeshJoint* joint = buf.front(); buf.pop();
		if (sm::dis_pos_to_pos(joint->GetWorldPos(), pos) < MeshJoint::RADIUS) {
			return joint;
		}
		const std::vector<MeshJoint*>& children = joint->GetChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			buf.push(children[i]);
		}
	}
	return NULL;
}

}