#ifndef _SPRITE2_JOINT_H_
#define _SPRITE2_JOINT_H_

#include "pre_defined.h"
#include "JointCoords.h"

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
	Joint(Sprite* spr, const LocalPose& joint_pose);
	~Joint();

	VIRTUAL_INHERITANCE void Translate(const sm::vec2& trans);
	VIRTUAL_INHERITANCE void Rotate(float rot);

	void Draw(const RenderParams& params) const;

	bool ConnectChild(Joint* child);
	void DeconnectParent();

	const Joint* GetParent() const { return m_parent; }
	const std::vector<Joint*>& GetChildren() const { return m_children; }

	const WorldPose& GetWorldPose() const { return m_world; }
 	void SetWorldPose(const WorldPose& pose) { m_world = pose; }
	void SetLocalPose(const LocalPose& pose) { m_local = pose; }

	const Sprite* GetSkinSpr() const { return m_skin.spr; }

public:
	static const float RADIUS;

private:
	void Update();

private:
	struct Skin : private cu::Uncopyable
	{
		Sprite* spr;
		LocalPose pose;

		Skin(Sprite* spr, const LocalPose& skin_pose);
		~Skin();

		void Update(const Joint* joint);

	}; // Skin

protected:
	std::string m_name;

	Joint* m_parent;
	std::vector<Joint*> m_children;

	WorldPose m_world;
	LocalPose m_local;

	Skin m_skin;

}; // Joint

}

#endif // _SPRITE2_JOINT_H_