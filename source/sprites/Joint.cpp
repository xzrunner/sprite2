#include "Joint.h"
#include "S2_Sprite.h"
#include "JointMath.h"
#include "S2_RVG.h"
#include "RenderParams.h"

#include <SM_Calc.h>
#include <shaderlab.h>

#include <algorithm>

namespace s2
{

const float RADIUS = 10;

Joint::Joint(Sprite* spr, const sm::vec2& offset)
	: m_parent(NULL)
	, m_skin(spr, -offset)
{
}

Joint::~Joint()
{
	if (m_parent) {
		m_parent->RemoveReference();
	}
	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFonctor<Joint>());
}

void Joint::Draw(const RenderParams& params) const
{
//	m_skin->Draw();

	RVG::SetColor(Color(51, 204, 51, 128));
	RVG::Circle(params.mt * m_world_pose.trans, RADIUS, true);
	RVG::SetColor(Color(204, 51, 51, 128));
	RVG::Circle(params.mt * m_world_pose.trans, RADIUS, false);

	if (m_parent)
	{
		sm::vec2 s = params.mt * m_world_pose.trans;
		sm::vec2 e = params.mt * (m_skin.spr->GetCenter() * 2 - s);

		const float w = 0.1f;
		sm::vec2 mid = s + (e-s)*w;
		sm::vec2 left = mid + sm::rotate_vector_right_angle(s - mid, false);
		sm::vec2 right = mid + sm::rotate_vector_right_angle(s - mid, true);

		RVG::SetColor(Color(204, 51, 51, 128));
		RVG::Line(s, left);
		RVG::Line(left, e);
		RVG::Line(e, right);
		RVG::Line(right, s);

		RVG::SetColor(Color(204, 204, 51, 128));
		std::vector<sm::vec2> face;
		face.push_back(s);
		face.push_back(left);
		face.push_back(right);
		face.push_back(e);
		RVG::TriangleStrip(face);
	}

	sl::Shader* shader = sl::ShaderMgr::Instance()->GetShader();
	if (shader) {
		shader->Commit();
	}
}

void Joint::Connect(Joint* child)
{
	for (int i = 0, n = child->m_children.size(); i < n; ++i) {
		if (child->m_children[i] == this) {
			return;
		}
	}
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (m_children[i] == child) {
			return;
		}
	}

	child->AddReference();
	m_children.push_back(child);

	cu::RefCountObjAssign(child->m_parent, (Joint*)this);
}

/************************************************************************/
/* class Joint::Skin                                                    */
/************************************************************************/

Joint::Skin::Skin(Sprite* spr, const sm::vec2& pos)
	: spr(spr)
	, pose(pos, 0)
{
	if (this->spr) {
		this->spr->AddReference();
	}
}

Joint::Skin::~Skin()
{
	if (spr) {
		spr->RemoveReference();
	}
}

void Joint::Skin::Update(const Joint* joint)
{
	JointPose dst = JointMath::Local2World(joint->m_world_pose, pose);
	spr->SetAngle(dst.rot);
	spr->Translate(dst.trans - spr->GetCenter());
}

}