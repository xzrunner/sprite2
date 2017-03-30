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
#include "S2_Symbol.h"
#include "SymType.h"
#include "RenderColor.h"

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

void AnimLerp::Lerp(const Sprite* begin, const Sprite* end, Sprite* tween, float process,
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

	float angle = (end->GetAngle() - begin->GetAngle()) * process + begin->GetAngle();
	tween->SetAngle(angle);

	sm::vec2 base_s = begin->GetPosition() + begin->GetOffset(),
		     base_e = end->GetPosition() + end->GetOffset();
	sm::vec2 base_t = (base_e - base_s) * process + base_s;
	tween->SetPosition(base_t - offset);

	RenderColor rc = tween->GetColor();
	rc.SetAdd(color_interpolate(begin->GetColor().GetAdd(), end->GetColor().GetAdd(), process));
	rc.SetMul(color_interpolate(begin->GetColor().GetMul(), end->GetColor().GetMul(), process));
	tween->SetColor(rc);

	LerpSpecial(begin, end, tween, process);
	
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

void AnimLerp::LerpSpecial(const Sprite* begin, const Sprite* end, Sprite* tween, float process)
{
	assert(begin->GetSymbol()->Type() == end->GetSymbol()->Type());
	switch (begin->GetSymbol()->Type())
	{
	case SYM_SCALE9:
		{
			sm::vec2 b_sz = VI_DOWNCASTING<const Scale9Sprite*>(begin)->GetScale9().GetSize(),
				e_sz = VI_DOWNCASTING<const Scale9Sprite*>(end)->GetScale9().GetSize();
			float t_w = (e_sz.x - b_sz.x) * process + b_sz.x,
				t_h = (e_sz.y - b_sz.y) * process + b_sz.y;
			VI_DOWNCASTING<Scale9Sprite*>(tween)->Resize(t_w, t_h);
		}
		break;
	case SYM_ICON:
		{
			float b_proc = VI_DOWNCASTING<const IconSprite*>(begin)->GetProcess(),
				e_proc = VI_DOWNCASTING<const IconSprite*>(end)->GetProcess();
			float proc = (e_proc - b_proc) * process + b_proc;
			VI_DOWNCASTING<IconSprite*>(tween)->SetProcess(proc);
		}
		break;
	case SYM_MESH:
		{
			const MeshSprite* mesh_b = VI_DOWNCASTING<const MeshSprite*>(begin);
			const MeshSprite* mesh_e = VI_DOWNCASTING<const MeshSprite*>(end);
			MeshSprite* mesh_t = VI_DOWNCASTING<MeshSprite*>(tween);
			mesh_t->Lerp(mesh_b, mesh_e, process);
		}
		break;
	case SYM_SKELETON:
		{
			const SkeletonSprite* sk_b = VI_DOWNCASTING<const SkeletonSprite*>(begin);
			const SkeletonSprite* sk_e = VI_DOWNCASTING<const SkeletonSprite*>(end);
			SkeletonSprite* sk_t = VI_DOWNCASTING<SkeletonSprite*>(tween);
			sk_t->GetPose().Lerp(sk_b->GetPose(), sk_e->GetPose(), process);
		}
		break;
	case SYM_PARTICLE3D:
		{
			const Particle3dSprite* p3d_b = VI_DOWNCASTING<const Particle3dSprite*>(begin);
			const Particle3dSprite* p3d_e = VI_DOWNCASTING<const Particle3dSprite*>(end);
			Particle3dSprite* p3d_t = VI_DOWNCASTING<Particle3dSprite*>(tween);
			float start_radius = (p3d_e->GetStartRadius() - p3d_b->GetStartRadius()) * process + p3d_b->GetStartRadius();
			p3d_t->SetStartRadius(start_radius);
		}
		break;
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