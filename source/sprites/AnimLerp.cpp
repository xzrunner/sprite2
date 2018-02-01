#include "sprite2/AnimLerp.h"
#include "sprite2/Sprite.h"
#include "sprite2/Scale9Sprite.h"
#include "sprite2/IconSprite.h"
#include "sprite2/MeshSprite.h"
#include "sprite2/SkeletonSprite.h"
#include "sprite2/Particle3dSprite.h"
#include "sprite2/LerpCircle.h"
#include "sprite2/LerpSpiral.h"
#include "sprite2/LerpWiggle.h"
#include "sprite2/LerpEase.h"
#include "sprite2/Symbol.h"
#include "sprite2/SymType.h"

#include <SM_Vector.h>
#include <painting2/Color.h>

#include <assert.h>

namespace s2
{

void AnimLerp::Lerp(const CU_VEC<SprPtr>& begin, const CU_VEC<SprPtr>& end,
					CU_VEC<SprPtr>& tween, int time, int tot_time, const CU_VEC<std::pair<SprData, std::unique_ptr<ILerp>>>& lerps)
{
	for (auto& start_spr : begin)
	{
		SprPtr end_spr;
		for (auto& spr : end)
		{
			if (IsMatched(*start_spr, *spr))
			{
				end_spr = spr;
				break;
			}
		}
		SprPtr tween_spr = start_spr->Clone();
		if (end_spr) {
			Lerp(*start_spr, *end_spr, tween_spr, time, tot_time, lerps);
		}
		tween.push_back(tween_spr);
	}
}

static inline
pt2::Color color_interpolate(const pt2::Color& begin, const pt2::Color& end, float scale)
{
	pt2::Color ret;
	ret.r = static_cast<uint8_t>(begin.r + (end.r - begin.r) * scale);
	ret.g = static_cast<uint8_t>(begin.g + (end.g - begin.g) * scale);
	ret.b = static_cast<uint8_t>(begin.b + (end.b - begin.b) * scale);
	ret.a = static_cast<uint8_t>(begin.a + (end.a - begin.a) * scale);
	return ret;
}

void AnimLerp::Lerp(const Sprite& begin, const Sprite& end, SprPtr& tween, int time, int tot_time,
					const CU_VEC<std::pair<SprData, std::unique_ptr<ILerp>>>& lerps)
{
	float process = static_cast<float>(time) / tot_time;

	sm::vec2 shear;
	shear.x = (end.GetShear().x - begin.GetShear().x) * process + begin.GetShear().x;
	shear.y = (end.GetShear().y - begin.GetShear().y) * process + begin.GetShear().y;
	tween->SetShear(shear);

	float xscale = (end.GetScale().x - begin.GetScale().x) * process + begin.GetScale().x,
		  yscale = (end.GetScale().y - begin.GetScale().y) * process + begin.GetScale().y;
	tween->SetScale(sm::vec2(xscale, yscale));

	sm::vec2 offset = (end.GetOffset() - begin.GetOffset()) * process + begin.GetOffset();
	tween->SetOffset(offset);
	tween->SetPosition(sm::vec2(0, 0));
	tween->SetAngle(0);

	float angle = (end.GetAngle() - begin.GetAngle()) * process + begin.GetAngle();
	tween->SetAngle(angle);

	sm::vec2 base_s = begin.GetPosition() + begin.GetOffset(),
		     base_e = end.GetPosition() + end.GetOffset();
	sm::vec2 base_t = (base_e - base_s) * process + base_s;
	tween->SetPosition(base_t - offset);

	auto color = tween->GetColorCommon();
	color.add = color_interpolate(begin.GetColorCommon().add, end.GetColorCommon().add, process);
	color.mul = color_interpolate(begin.GetColorCommon().mul, end.GetColorCommon().mul, process);
	tween->SetColorCommon(color);

	LerpSpecial(begin, end, tween, time, tot_time);
	LerpExpression(begin, end, tween, time, tot_time, lerps);
}

void AnimLerp::LerpSpecial(const Sprite& begin, const Sprite& end, SprPtr& tween, int time, int tot_time)
{
	float process = static_cast<float>(time) / tot_time;

	assert(begin.GetSymbol()->Type() == end.GetSymbol()->Type());
	switch (begin.GetSymbol()->Type())
	{
	case SYM_SCALE9:
		{
			sm::vec2 b_sz = S2_VI_DOWN_CAST<const Scale9Sprite&>(begin).GetScale9().GetSize(),
				     e_sz = S2_VI_DOWN_CAST<const Scale9Sprite&>(end).GetScale9().GetSize();
			float t_w = (e_sz.x - b_sz.x) * process + b_sz.x,
				  t_h = (e_sz.y - b_sz.y) * process + b_sz.y;
			S2_VI_PTR_DOWN_CAST<Scale9Sprite>(tween)->Resize(t_w, t_h);
		}
		break;
	case SYM_ICON:
		{
			float b_proc = S2_VI_DOWN_CAST<const IconSprite&>(begin).GetProcess(),
				  e_proc = S2_VI_DOWN_CAST<const IconSprite&>(end).GetProcess();
			float proc = (e_proc - b_proc) * process + b_proc;
			S2_VI_PTR_DOWN_CAST<IconSprite>(tween)->SetProcess(proc);
		}
		break;
	case SYM_MESH:
		{
			const MeshSprite& mesh_b = S2_VI_DOWN_CAST<const MeshSprite&>(begin);
			const MeshSprite& mesh_e = S2_VI_DOWN_CAST<const MeshSprite&>(end);
			S2_VI_PTR_DOWN_CAST<MeshSprite>(tween)->Lerp(mesh_b, mesh_e, process);
		}
		break;
	case SYM_SKELETON:
		{
			const SkeletonSprite& sk_b = S2_VI_DOWN_CAST<const SkeletonSprite&>(begin);
			const SkeletonSprite& sk_e = S2_VI_DOWN_CAST<const SkeletonSprite&>(end);
			S2_VI_PTR_DOWN_CAST<SkeletonSprite>(tween)->GetPose().Lerp(sk_b.GetPose(), sk_e.GetPose(), process);
		}
		break;
	case SYM_PARTICLE3D:
		{
			const Particle3dSprite& p3d_b = S2_VI_DOWN_CAST<const Particle3dSprite&>(begin);
			const Particle3dSprite& p3d_e = S2_VI_DOWN_CAST<const Particle3dSprite&>(end);
			float start_radius = (p3d_e.GetStartRadius() - p3d_b.GetStartRadius()) * process + p3d_b.GetStartRadius();
			S2_VI_PTR_DOWN_CAST<Particle3dSprite>(tween)->SetStartRadius(start_radius);
		}
		break;
	}
}

void AnimLerp::LerpExpression(const Sprite& begin, const Sprite& end, SprPtr& tween, int time, int tot_time,
							  const CU_VEC<std::pair<SprData, std::unique_ptr<ILerp>>>& lerps)
{
	float process = static_cast<float>(time) / tot_time;

	sm::vec2 offset = (end.GetOffset() - begin.GetOffset()) * process + begin.GetOffset();

	sm::vec2 base_s = begin.GetPosition() + begin.GetOffset(),
		     base_e = end.GetPosition() + end.GetOffset();
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
				const sm::vec2& b_scale = begin.GetScale();
				const sm::vec2& e_scale = end.GetScale();
				sm::vec2 s = static_cast<LerpEase*>(lerp.get())->Lerp(b_scale, e_scale, process);
				tween->SetScale(s);
			} else if (data == SPR_ROTATE) {
				float b_angle = begin.GetAngle();
				float e_angle = end.GetAngle();
				float angle = static_cast<LerpEase*>(lerp.get())->Lerp(b_angle, e_angle, process);
				tween->SetAngle(angle);
			}
			break;
		}
	}
}

bool AnimLerp::IsMatched(const Sprite& s0, const Sprite& s1)
{
	int name0 = s0.GetName(), name1 = s1.GetName();
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