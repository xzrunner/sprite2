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
#include "LerpWiggle.h"
#include "LerpEase.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "RenderColor.h"

#include <SM_Vector.h>

#include <assert.h>

namespace s2
{

void AnimLerp::Lerp(const std::vector<Sprite*>& begin, const std::vector<Sprite*>& end, 
					std::vector<Sprite*>& tween, int time, int tot_time, const std::vector<std::pair<SprData, std::unique_ptr<ILerp>>>& lerps)
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
			Lerp(start_spr, end_spr, tween_spr, time, tot_time, lerps);
		}
		tween.push_back(tween_spr);
	}
}

static inline
Color color_interpolate(const Color& begin, const Color& end, float scale)
{
	Color ret;
	ret.r = static_cast<uint8_t>(begin.r + (end.r - begin.r) * scale);
	ret.g = static_cast<uint8_t>(begin.g + (end.g - begin.g) * scale);
	ret.b = static_cast<uint8_t>(begin.b + (end.b - begin.b) * scale);
	ret.a = static_cast<uint8_t>(begin.a + (end.a - begin.a) * scale);
	return ret;
}

void AnimLerp::Lerp(const Sprite* begin, const Sprite* end, Sprite* tween, int time, int tot_time,
					const std::vector<std::pair<SprData, std::unique_ptr<ILerp>>>& lerps)
{
	float process = static_cast<float>(time) / tot_time;

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

	LerpSpecial(begin, end, tween, time, tot_time);
	LerpExpression(begin, end, tween, time, tot_time, lerps);
}

void AnimLerp::LerpSpecial(const Sprite* begin, const Sprite* end, Sprite* tween, int time, int tot_time)
{
	float process = static_cast<float>(time) / tot_time;

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

void AnimLerp::LerpExpression(const Sprite* begin, const Sprite* end, Sprite* tween, int time, int tot_time, 
							  const std::vector<std::pair<SprData, std::unique_ptr<ILerp>>>& lerps)
{
	float process = static_cast<float>(time) / tot_time;

	sm::vec2 offset = (end->GetOffset() - begin->GetOffset()) * process + begin->GetOffset();

	sm::vec2 base_s = begin->GetPosition() + begin->GetOffset(),
		     base_e = end->GetPosition() + end->GetOffset();
	sm::vec2 base_t = (base_e - base_s) * process + base_s;

	for (int i = 0, n = lerps.size(); i < n; ++i) 
	{
		const SprData& data = lerps[i].first;
		const std::unique_ptr<ILerp>& lerp = lerps[i].second;
		switch (lerp->Type())
		{
		case LERP_CIRCLE:
			if (data == SPR_POS) {
				sm::vec2 base_t = static_cast<LerpCircle*>(lerp.get())->Lerp(base_s, base_e, process);
				tween->SetPosition(base_t - offset);
			}
			break;
		case LERP_SPIRAL:
			if (data == SPR_POS) {
				sm::vec2 base_t = static_cast<LerpSpiral*>(lerp.get())->Lerp(base_s, base_e, process);
				tween->SetPosition(base_t - offset);
			}
			break;
		case LERP_WIGGLE:
			if (data == SPR_POS) {
				base_t = static_cast<LerpWiggle*>(lerp.get())->Lerp(base_t, time / 30.0f);
				tween->SetPosition(base_t - offset);
			}
			break;
		case LERP_EASE:
			if (data == SPR_POS) {
				sm::vec2 base_t = static_cast<LerpEase*>(lerp.get())->Lerp(base_s, base_e, process);
				tween->SetPosition(base_t - offset);
			} else if (data == SPR_SCALE) {
				const sm::vec2& b_scale = begin->GetScale();
				const sm::vec2& e_scale = end->GetScale();
				sm::vec2 s = static_cast<LerpEase*>(lerp.get())->Lerp(b_scale, e_scale, process);
				tween->SetScale(s);
			} else if (data == SPR_ROTATE) {
				float b_angle = begin->GetAngle();
				float e_angle = end->GetAngle();
				float angle = static_cast<LerpEase*>(lerp.get())->Lerp(b_angle, e_angle, process);
				tween->SetAngle(angle);
			}
			break;
		}
	}
}

bool AnimLerp::IsMatched(const Sprite* s0, const Sprite* s1)
{
	int name0 = s0->GetName(), name1 = s1->GetName();
	bool auto_named = SprNameMap::IsTmpName(name0) && SprNameMap::IsTmpName(name1);
	if (auto_named && name0 == name1) {
		return true;
	} else if (name0 == -1 && name1 == -1) {
//		return s0->getSymbol().getFilepath() == s1->getSymbol().getFilepath();
		return false;
	} else {
		return name0 == name1;
	}
}

}