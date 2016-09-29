#ifndef _SPRITE2_JOINT_H_
#define _SPRITE2_JOINT_H_

#include "JointPose.h"

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

#include <SM_Rect.h>

#include <string>
#include <vector>

namespace s2
{

class Skin;
class Sprite;
class RenderParams;
class BoundingBox;

class Joint : public cu::RefCountObj, private cu::Uncopyable
{
public:
	Joint(Sprite* spr, const sm::vec2& offset);
	~Joint();

	void Draw(const RenderParams& params) const;

	void Connect(Joint* child);

	const Joint* GetParent() const { return m_parent; }

	void SetWorldPos(const sm::vec2& pos) { m_world_pose.trans = pos; }
	const sm::vec2& GetWorldPos() const { return m_world_pose.trans; }

	const BoundingBox* GetBoundingBox() const;

private:
	struct Skin : private cu::Uncopyable
	{
		Sprite* spr;
		JointPose pose;

		Skin(Sprite* spr, const sm::vec2& pos);
		~Skin();

		void Update(const Joint* joint);
		void Draw(const RenderParams& params) const;

	}; // Skin

private:
	std::string m_name;

	Joint* m_parent;
	std::vector<Joint*> m_children;

	JointPose m_world_pose;

	Skin m_skin;

}; // Joint

}

#endif // _SPRITE2_JOINT_H_