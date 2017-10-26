#ifndef _SPRITE2_JOINT_H_
#define _SPRITE2_JOINT_H_

#include "pre_defined.h"
#include "JointPose.h"
#include "RenderReturn.h"
#include "s2_typedef.h"

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>
#include <SM_Rect.h>

#include <memory>

namespace cooking { class DisplayList; }

namespace s2
{

class Skin;
class Sprite;
class RenderParams;

class Joint : private cu::Uncopyable
{
public:
	Joint(const SprPtr& spr, const JointPose& joint_local);

	VIRTUAL_INHERITANCE void Translate(const sm::vec2& trans);
	VIRTUAL_INHERITANCE void Rotate(float rot);
	VIRTUAL_INHERITANCE void Scale(const sm::vec2& scale);

	RenderReturn Draw(cooking::DisplayList* dlist, const RenderParams& rp) const;
	void Update();

	bool ConnectChild(const std::shared_ptr<Joint>& child);
	void DeconnectParent();

	std::shared_ptr<Joint> GetParent() const { return m_parent.lock(); }
	const CU_VEC<std::shared_ptr<Joint>>& GetChildren() const { return m_children; }

	const JointPose& GetWorldPose() const { return m_world_pose; }
	const JointPose& GetLocalPose() const { return m_local_pose; }
	const JointPose& GetSkinPose() const { return m_skin.skin_local; } 
 	void SetWorldPose(const JointPose& pose) { m_world_pose = pose; }
	void SetLocalPose(const JointPose& pose) { m_local_pose = pose; }
	void SetSkinPose(const JointPose& pose) { m_skin.skin_local = pose; }

	const SprPtr& GetSkinSpr() const { return m_skin.spr; }

	void SetName(const CU_STR& name) { m_name = name; }
	const CU_STR& GetName() const { return m_name; }

public:
	static const float RADIUS;

private:
	struct Skin : private cu::Uncopyable
	{
		SprPtr spr;
		JointPose skin_local;

		Skin(const SprPtr& spr, const JointPose& skin_local);

		void Update(const Joint* joint);

	}; // Skin

protected:
	CU_STR m_name;

	std::weak_ptr<Joint> m_parent;
	CU_VEC<std::shared_ptr<Joint>> m_children;

	JointPose m_world_pose, m_local_pose;

	Skin m_skin;

}; // Joint

}

#endif // _SPRITE2_JOINT_H_