#ifndef _SPRITE2_JOINT_H_
#define _SPRITE2_JOINT_H_

#include "pre_defined.h"
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
	Joint(Sprite* spr, const JointPose& joint_local);
	~Joint();

	VIRTUAL_INHERITANCE void Translate(const sm::vec2& trans);
	VIRTUAL_INHERITANCE void Rotate(float rot);

	void Draw(const RenderParams& params) const;
	void Update();

	bool ConnectChild(Joint* child);
	void DeconnectParent();

	const Joint* GetParent() const { return m_parent; }
	const std::vector<Joint*>& GetChildren() const { return m_children; }

	const JointPose& GetWorldPose() const { return m_world_pose; }
	const JointPose& GetLocalPose() const { return m_local_pose; }
	const JointPose& GetSkinPose() const { return m_skin.skin_local; } 
 	void SetWorldPose(const JointPose& pose) { m_world_pose = pose; }
	void SetLocalPose(const JointPose& pose) { m_local_pose = pose; }
	void SetSkinPose(const JointPose& pose) { m_skin.skin_local = pose; }

	const Sprite* GetSkinSpr() const { return m_skin.spr; }

public:
	static const float RADIUS;

private:
	struct Skin : private cu::Uncopyable
	{
		Sprite* spr;
		JointPose skin_local;

		Skin(Sprite* spr, const JointPose& skin_local);
		~Skin();

		void Update(const Joint* joint);

	}; // Skin

protected:
	std::string m_name;

	Joint* m_parent;
	std::vector<Joint*> m_children;

	JointPose m_world_pose, m_local_pose;

	Skin m_skin;

}; // Joint

}

#endif // _SPRITE2_JOINT_H_