#include "AnimLerp.h"
#include "Color.h"
#include "S2_Sprite.h"
#include "Scale9Sprite.h"
#include "IconSprite.h"
#include "MeshSprite.h"
#include "SkeletonSprite.h"
#include "Particle3dSprite.h"
#include "LerpCircle.h"
#include "LerpSpiral.h"

#include <SM_Vector.h>

#include <assert.h>

namespace s2
{

void AnimLerp::Lerp(const std::vector<Sprite*>& begin, const std::vector<Sprite*>& end, 
					std::vector<Sprite*>& tween, float process, const std::vector<std::pair<SprData, ILerp*> >& lerps)
{
	for (int i = 0, n = begin.size(); i < n; ++i)
	{
		Sprite *start_spr = begin[i], 
			   *end_spr = NULL;
		for (int j = 0, m = end.size(); j < m; ++j)
		{
			Sprite* spr = end[j];
			if (IsMatched(start_spr, spr))
			{
				end_spr = spr;
				break;
			}
		}
		Sprite* tween_spr = VI_CLONE(Sprite, start_spr);
		if (end_spr) {
			Lerp(start_spr, end_spr, tween_spr, process, lerps);
		}
		tween.push_back(tween_spr);
	}
}

static inline
Color color_interpolate(const Color& begin, const Color& end, float scale)
{
	Color ret;
	ret.r = begin.r + (end.r - begin.r) * scale;
	ret.g = begin.g + (end.g - begin.g) * scale;
	ret.b = begin.b + (end.b - begin.b) * scale;
	ret.a = begin.a + (end.a - begin.a) * scale;
	return ret;
}

void AnimLerp::Lerp(Sprite* begin, Sprite* end, Sprite* tween, float process,
					const std::vector<std::pair<SprData, ILerp*> >& lerps)
{
	sm::vec2 shear;
	shear.x = (end->GetShear().x - begin->GetShear().x) * process + begin->GetShear().x;
	shear.y = (end->GetShear().y - begin->GetShear().y) * process + begin->GetShear().y;
	tween->SetShear(shear);

	float xscale = (end->GetScale().x - begin->GetScale().x) * process + begin->GetScale().x,
		  yscale = (end->GetScale().y - begin->GetScale().y) * process + begin->GetScale().y;
	tween->SetScale(sm::vec2(xscale, yscale));

	sm::vec2 offset = (end->GetOffset() - begin->GetOffset()) * process + begin->GetOffset();
	tween->SetOffset(offset);
	tween->SetPosition(sm::vec2(0, 0));
	tween->SetAngle(0);

	sm::vec2 center_s = begin->GetCenter(), center_e = end->GetCenter();

	float angle = (end->GetAngle() - begin->GetAngle()) * process + begin->GetAngle();
	tween->SetAngle(angle);

	sm::vec2 base_s = begin->GetPosition() + begin->GetOffset(),
		     base_e = end->GetPosition() + end->GetOffset();
	sm::vec2 base_t = (base_e - base_s) * process + base_s;
	tween->SetPosition(base_t -  offset);

	s2::RenderColor rc = tween->GetColor();
	rc.add = color_interpolate(begin->GetColor().add, end->GetColor().add, process);
	rc.mul = color_interpolate(begin->GetColor().mul, end->GetColor().mul, process);
	tween->SetColor(rc);

	if (Scale9Sprite* s9_b = dynamic_cast<Scale9Sprite*>(begin))
	{
		Scale9Sprite* s9_e = dynamic_cast<Scale9Sprite*>(end);
		Scale9Sprite* s9_t = dynamic_cast<Scale9Sprite*>(tween);
		assert(s9_e && s9_t);
		sm::vec2 s_sz = s9_b->GetScale9().GetSize(),
			     e_sz = s9_e->GetScale9().GetSize();
		float t_w = (e_sz.x - s_sz.x) * process + s_sz.x,
			  t_h = (e_sz.y - s_sz.y) * process + s_sz.y;
		s9_t->Resize(t_w, t_h);
	}
	else if (IconSprite* icon_b = dynamic_cast<IconSprite*>(begin))
	{
		IconSprite* icon_e = dynamic_cast<IconSprite*>(end);
		IconSprite* icon_t = dynamic_cast<IconSprite*>(tween);
		assert(icon_e && icon_t);
		float proc = (icon_e->GetProcess() - icon_b->GetProcess()) * process + icon_b->GetProcess();
		icon_t->SetProcess(proc);
	}
	else if (MeshSprite* mesh_b = dynamic_cast<MeshSprite*>(begin))
	{
		MeshSprite* mesh_e = dynamic_cast<MeshSprite*>(end);
		MeshSprite* mesh_t = dynamic_cast<MeshSprite*>(tween);
		assert(mesh_e && mesh_t);
		mesh_t->Lerp(mesh_b, mesh_e, process);
	}
	else if (SkeletonSprite* sk_b = dynamic_cast<SkeletonSprite*>(begin))
	{
		SkeletonSprite* sk_e = dynamic_cast<SkeletonSprite*>(end);
		SkeletonSprite* sk_t = dynamic_cast<SkeletonSprite*>(tween);
		assert(sk_e && sk_t);
		sk_t->GetPose().Lerp(sk_b->GetPose(), sk_e->GetPose(), process);
	}
	else if (Particle3dSprite* p3d_b = dynamic_cast<Particle3dSprite*>(begin))
	{
		Particle3dSprite* p3d_e = dynamic_cast<Particle3dSprite*>(end);
		Particle3dSprite* p3d_t = dynamic_cast<Particle3dSprite*>(tween);
		assert(p3d_e && p3d_t);
		float start_radius = (p3d_e->GetStartRadius() - p3d_b->GetStartRadius()) * process + p3d_b->GetStartRadius();
		p3d_t->SetStartRadius(start_radius);
	}

	for (int i = 0, n = lerps.size(); i < n; ++i) 
	{
		SprData data = lerps[i].first;
		ILerp* lerp = lerps[i].second;
		switch (lerp->Type())
		{
		case LERP_CIRCLE:
			if (data == SPR_POS) {
				sm::vec2 base_t = static_cast<LerpCircle*>(lerp)->Lerp(base_s, base_e, process);
				tween->SetPosition(base_t - offset);
			}
			break;
		case LERP_SPIRAL:
			if (data == SPR_POS) {
				sm::vec2 base_t = static_cast<LerpSpiral*>(lerp)->Lerp(base_s, base_e, process);
				tween->SetPosition(base_t - offset);
			}
			break;
		}
	}
}

bool AnimLerp::IsMatched(const Sprite* s0, const Sprite* s1)
{
	const std::string& name0 = s0->GetName(),
		               name1 = s1->GetName();

	bool auto_named = false;
	if (!name0.empty() && name0[0] == '_' && !name1.empty() && name1[0] == '_') {
		auto_named = true;
	}

	if (auto_named && name0 == name1) {
		return true;
	} else if (name0.empty() && name1.empty()) {
//		return s0->getSymbol().getFilepath() == s1->getSymbol().getFilepath();
		return false;
	} else {
		return name0 == name1;
	}
}

}