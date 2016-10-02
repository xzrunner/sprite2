#ifndef _SPRITE2_MESH_JOINT_H_
#define _SPRITE2_MESH_JOINT_H_

#include "JointPose.h"

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

#include <vector>

namespace s2
{

class RenderParams;
class MeshNode;

class MeshJoint : public cu::RefCountObj, private cu::Uncopyable
{
public:
	MeshJoint(MeshJoint* parent, const sm::vec2& pos);
	MeshJoint(MeshJoint* parent, const JointPose& world);
	~MeshJoint();

	void Draw(const RenderParams& params) const;

	void Deconnect();
	
	const JointPose& GetWorldPose() const { return m_world_pose; }
	void SetWorldPos(const sm::vec2& pos) { m_world_pose.trans = pos; }

	const std::vector<MeshJoint*>& GetChildren() const { return m_children; }
	const MeshJoint* GetParent() const { return m_parent; } 

public:
	static const float RADIUS;

private:
	struct Node
	{
		MeshNode* node;
		float weight;
		sm::vec2 local;

		Node(MeshNode* node, float weight, sm::vec2 local);
		~Node();

	}; // Node

private:
	MeshJoint* m_parent;
	std::vector<MeshJoint*> m_children;

	JointPose m_world_pose, m_local_pose;

	std::vector<Node> m_nodes;

}; // MeshJoint

}

#endif // _SPRITE2_MESH_JOINT_H_