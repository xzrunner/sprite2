#ifndef _SPRITE2_JOINT_H_
#define _SPRITE2_JOINT_H_

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
	Joint(Sprite* spr, const sm::vec2& offset);
	~Joint();

	void Draw(const RenderParams& params) const;

	void Connect(Joint* child);

	const Joint* GetParent() const { return m_parent; }

	const sm::vec2& GetWorldPos() const { return m_world.pos; }

	void SetWorldPose(const WorldPose& pose) { m_world = pose; }
	void SetLocalPose(const LocalPose& pose) { m_local = pose; }

	const BoundingBox* GetBoundingBox() const;

	void Translate(const sm::vec2& trans);
	void Rotate(float rot);

	const Sprite* GetSkinSpr() const { return m_skin.spr; }

private:
	struct Skin : private cu::Uncopyable
	{
		Sprite* spr;
		sm::vec2 offset;

		Skin(Sprite* spr, const sm::vec2& offset);
		~Skin();

		void Update(const Joint* joint);
		void Draw(const RenderParams& params) const;

	}; // Skin

private:
	std::string m_name;

	Joint* m_parent;
	std::vector<Joint*> m_children;

	WorldPose m_world;
	LocalPose m_local;

	Skin m_skin;

}; // Joint

}

#endif // _SPRITE2_JOINT_H_