#include "ProxyHelper.h"
#include "ProxySymbol.h"
#include "ProxySprite.h"
#include "SymType.h"
#include "S2_Sprite.h"
#include "S2_Actor.h"
#include "PointQueryVisitor.h"
#include "BoundingBox.h"
#include "UpdateParams.h"
#include "RenderColor.h"
#include "SetStaticFrameVisitor.h"

#include "ComplexSymbol.h"
#include "ComplexSprite.h"
#include "ComplexActor.h"
#include "AnimSymbol.h"
#include "AnimCurr.h"
#include "AnimSprite.h"
#include "Anim2Sprite.h"
#include "Particle3dSprite.h"
#include "Particle3dEmitter.h"
#include "TextboxSprite.h"
#include "TextboxActor.h"
#include "Scale9Actor.h"

#include <SM_Calc.h>

#include <assert.h>

namespace s2
{

SprPtr ProxyHelper::BuildGroup(const CU_VEC<std::pair<const ActorConstPtr, SprPtr>>& items, bool force_group)
{
	SprPtr ret = nullptr;
	if (!force_group && items.size() == 1) 
	{
		ret = items[0].second;
	} 
	else if (items.size() > 1) 
	{
		auto sym = mm::allocate_shared<ProxySymbol>(items);
		ret = mm::allocate_shared<ProxySprite>(sym);
	}
	return ret;
}

/************************************************************************/
/* sprite                                                               */
/************************************************************************/

#include "UpdateParams.h"

bool ProxyHelper::SprGetPos(const Sprite& spr, sm::vec2& pos)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		if (items.empty()) {
			return false;
		}
		sm::vec2 ret;
		if (!SprGetPos(*items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			sm::vec2 cpos;
			if (!SprGetPos(*items[i].second, cpos) || cpos != ret) {
				return false;
			}
		}
		pos = ret;
		return true;
	} 
	else 
	{
		pos = spr.GetPosition();
		return true;
	}
}

bool ProxyHelper::SprGetAngle(const Sprite& spr, float& angle)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		if (items.empty()) {
			return false;
		}
		float ret;
		if (!SprGetAngle(*items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			float cangle;
			if (!SprGetAngle(*items[i].second, cangle) || cangle != ret) {
				return false;
			}
		}
		angle = ret;
		return true;
	} 
	else 
	{
		angle = spr.GetAngle();
		return true;
	}
}

bool ProxyHelper::SprGetScale(const Sprite& spr, sm::vec2& scale)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		if (items.empty()) {
			return false;
		}
		sm::vec2 ret;
		if (!SprGetScale(*items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			sm::vec2 cscale;
			if (!SprGetScale(*items[i].second, cscale) || cscale != ret) {
				return false;
			}
		}
		scale = ret;
		return true;
	} 
	else 
	{
		scale = spr.GetScale();
		return true;
	}
}

bool ProxyHelper::SprGetSymID(const Sprite& spr, int& id)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		if (items.empty()) {
			return false;
		}
		int ret;
		if (!SprGetSymID(*items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			int cid;
			if (!SprGetSymID(*items[i].second, cid) || cid != ret) {
				return false;
			}
		}
		id = ret;
		return true;
	} 
	else 
	{
		id = spr.GetSymbol()->GetID();
		return true;
	}
}

bool ProxyHelper::SprGetSymType(const Sprite& spr, int& type)
{
	auto& sym = spr.GetSymbol();
	type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		if (items.empty()) {
			return false;
		}
		int ret;
		if (!SprGetSymType(*items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			int ctype;
			if (!SprGetSymType(*items[i].second, ctype) || ctype != ret) {
				return false;
			}
		}
		type = ret;
		return true;
	} 
	else 
	{
		type = sym->Type();
		return true;
	}
}

bool ProxyHelper::SprPointTest(const Sprite& spr, const sm::vec2& pos)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			if (SprPointTest(*items[i].second, pos)) {
				return true;
			}
		}
		return false;
	}
	else
	{
		return spr.GetBounding()->IsContain(pos);
	}
}

ActorConstPtr ProxyHelper::SprPointQuery(const Sprite& spr, const sm::vec2& pos, float mat[6])
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			if (auto actor = SprPointQuery(*items[i].second, pos, mat)) {
				return actor;
			}
		}
		return nullptr;
	}
	else
	{
		PointQueryVisitor visitor(pos);

		SprVisitorParams params;
		spr.Traverse(visitor, params);
		auto ret = visitor.GetSelectedActor();
		if (!ret) {
			return nullptr;
		}

		const S2_MAT& selected_mat = visitor.GetSelectedMat();
#ifdef S2_MATRIX_FIX
		mat[0] = selected_mat.x[0] * sm::MatrixFix::SCALE_INV;
		mat[1] = selected_mat.x[1] * sm::MatrixFix::SCALE_INV;
		mat[2] = selected_mat.x[2] * sm::MatrixFix::SCALE_INV;
		mat[3] = selected_mat.x[3] * sm::MatrixFix::SCALE_INV;
		mat[4] = selected_mat.x[4] * sm::MatrixFix::TRANSLATE_SCALE_INV;
		mat[5] = selected_mat.x[5] * sm::MatrixFix::TRANSLATE_SCALE_INV;
#else
		memcpy(mat, selected_mat.x, sizeof(selected_mat.x));
#endif // S2_MATRIX_FIX

		return ret;
	}
}

bool ProxyHelper::SprGetForceUpdate(const Sprite& spr, bool& force)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		bool ret;
		if (!SprGetForceUpdate(*items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			bool cforce;
			if (!SprGetForceUpdate(*items[i].second, cforce) || cforce != ret) {
				return false;
			}
		}
		force = ret;
		return true;
	} 
	else
	{
		force = spr.IsForceUpdate();
		return true;
	}
}

void ProxyHelper::SprSetForceUpdate(const Sprite& spr, bool force)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprSetForceUpdate(*items[i].second, force);
		}
	}
	else
	{
		spr.SetForceUpdate(force);
	}
}

void ProxyHelper::SprSetInheritUpdate(const Sprite& spr, bool inherit)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprSetInheritUpdate(*items[i].second, inherit);
		}
	}
	else
	{
		spr.SetInheritUpdate(inherit);
	}
}

int ProxyHelper::SprGetInheritUpdate(const Sprite& spr)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		if (items.empty()) {
			return -1;
		} else {
			int ret = -1;
			for (int i = 0, n = items.size(); i < n; ++i) {
				if (ret == -1) {
					ret = items[i].second->IsInheritUpdate() ? 1 : 0;
				} else {
					int _ret = items[i].second->IsInheritUpdate() ? 1 : 0;
					if (_ret != ret) {
						ret = -1;
						break;
					}
				}
			} 
			return ret;
		}
	}
	else
	{
		return spr.IsInheritUpdate() ? 1 : 0;
	}
}

bool ProxyHelper::SprGetFrameCount(const Sprite& spr, int& count)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		if (items.empty()) {
			return false;
		}
		int ret;
		if (!SprGetFrameCount(*items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			int ccount;
			if (!SprGetFrameCount(*items[i].second, ccount) || ccount != ret) {
				return false;
			}
		}
		count = ret;
		return true;
	} 
	else if (type == SYM_ANIMATION)
	{
		count = S2_VI_PTR_DOWN_CAST<const AnimSymbol>(spr.GetSymbol())->GetMaxFrameIdx();
		return true;
	}
	else
	{
		return false;
	}
}

void ProxyHelper::SprAnimSetLoop(const Sprite& spr, bool loop)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprAnimSetLoop(*items[i].second, loop);
		}
	}
	else if (type == SYM_ANIMATION)
	{
		auto& anim_spr = S2_VI_DOWN_CAST<const AnimSprite&>(spr);
		const_cast<AnimSprite&>(anim_spr).SetLoop(loop);
	}	
}

bool ProxyHelper::SprHasAction(const Sprite& spr, const CU_STR& action)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			if (SprHasAction(*items[i].second, action)) {
				return true;
			}
		}
		return false;
	}
	else if (type == SYM_COMPLEX)
	{
		int idx = S2_VI_PTR_DOWN_CAST<const ComplexSymbol>(sym)->GetActionIdx(action);
		return idx != -1;
	}
	else
	{
		return false;
	}
}

void ProxyHelper::SprTextboxResetTime(const Sprite& spr)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprTextboxResetTime(*items[i].second);
		}
	}
	else if (type == SYM_TEXTBOX)
	{
		auto& tb_spr = S2_VI_DOWN_CAST<const TextboxSprite&>(spr);
		const_cast<TextboxSprite&>(tb_spr).ResetTime();
	}
}

void ProxyHelper::SprTextboxSetFontColor(const Sprite& spr, uint32_t abgr)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprTextboxSetFontColor(*items[i].second, abgr);
		}
	}
	else if (type == SYM_TEXTBOX)
	{
		auto& tb_spr = S2_VI_DOWN_CAST<const TextboxSprite&>(spr);
		const_cast<TextboxSprite&>(tb_spr).GetTextbox().font_color.FromABGR(abgr);
	}
}

void ProxyHelper::SprTextboxSetEdgeColor(const Sprite& spr, uint32_t abgr)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprTextboxSetEdgeColor(*items[i].second, abgr);
		}
	}
	else if (type == SYM_TEXTBOX)
	{
		auto& tb_spr = S2_VI_DOWN_CAST<const TextboxSprite&>(spr);
		const_cast<TextboxSprite&>(tb_spr).GetTextbox().edge_color.FromABGR(abgr);
	}
}

void ProxyHelper::SprAnim2SetStaticTime(const Sprite& spr, int time)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprAnim2SetStaticTime(*items[i].second, time);
		}
	}
	else if (type == SYM_ANIM2)
	{
		auto& a2_spr = S2_VI_DOWN_CAST<const Anim2Sprite&>(spr);
		const_cast<Anim2Sprite&>(a2_spr).SetStaticTime(UpdateParams(), time);
	}
}

void ProxyHelper::SprP3dSetLocal(const Sprite& spr, bool local)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprP3dSetLocal(*items[i].second, local);
		}
	}
	else if (type == SYM_PARTICLE3D)
	{
		auto& p3d_spr = S2_VI_DOWN_CAST<const Particle3dSprite&>(spr);
		const_cast<Particle3dSprite&>(p3d_spr).SetLocal(local);
	}
}

void ProxyHelper::SprP3dSetLoop(const Sprite& spr, bool loop)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprP3dSetLoop(*items[i].second, loop);
		}
	}
	else if (type == SYM_PARTICLE3D)
	{
		auto& p3d_spr = S2_VI_DOWN_CAST<const Particle3dSprite&>(spr);
		const_cast<Particle3dSprite&>(p3d_spr).SetLoop(loop);
	}
}

bool ProxyHelper::SprP3dIsFinished(const Sprite& spr)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			if (!SprP3dIsFinished(*items[i].second)) {
				return false;
			}
		}
		return true;
	} 
	else if (type == SYM_PARTICLE3D)
	{
		auto& p3d_spr = S2_VI_DOWN_CAST<const Particle3dSprite&>(spr);
		if (auto et = const_cast<Particle3dSprite&>(p3d_spr).GetEmitter()) {
			return et->IsFinished();
		} else {
			return true;
		}
	}
	else
	{
		return true;
	}
}

void ProxyHelper::SprP3dUpdate(const Sprite& spr, float dt)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprP3dUpdate(*items[i].second, dt);
		}
	}
	else if (type == SYM_PARTICLE3D)
	{
		auto& p3d_spr = S2_VI_DOWN_CAST<const Particle3dSprite&>(spr);
		if (auto et = const_cast<Particle3dSprite&>(p3d_spr).GetEmitter()) {
			et->Update(et->GetTime() + dt);
		}
	}
}

void ProxyHelper::SprGetProxyChildren(const Sprite& spr, CU_VEC<ActorPtr>& actors)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type != SYM_PROXY) {
		return;
	}

	auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
	
	actors.reserve(actors.size() + items.size());
	for (int i = 0, n = items.size(); i < n; ++i) 
	{
		auto child_actor = items[i].second->QueryActorRef(items[i].first.get());
		if (child_actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
			SprGetProxyChildren(*child_actor->GetSpr(), actors);
		} else {
			actors.push_back(child_actor);
		}
	}
}

void ProxyHelper::SetDTexEnable(const Sprite& spr, bool enable)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SetDTexEnable(*items[i].second, enable);
		}
	}
	else
	{
		spr.SetDTexDisable(!enable);
	}
}

void ProxyHelper::SetDTexForceCached(const Sprite& spr, bool cache)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SetDTexForceCached(*items[i].second, cache);
		}
	}
	else
	{
		spr.SetDTexForceCached(cache);
		if (cache) {
			spr.SetDTexForceCachedDirty(true);
		}
	}
}

void ProxyHelper::SetDTexForceCachedDirty(const Sprite& spr, bool dirty)
{
	auto& sym = spr.GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SetDTexForceCachedDirty(*items[i].second, dirty);
		}
	}
	else
	{
		spr.SetDTexForceCachedDirty(dirty);
	}
}

/************************************************************************/
/* actor                                                                */
/************************************************************************/	

bool ProxyHelper::ActorGetAABB(const ActorPtr& actor, sm::rect& aabb)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		sm::rect ret;
		if (!ActorGetAABB(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			sm::rect caabb;
			if (!ActorGetAABB(items[i].second->QueryActorRef(items[i].first.get()), caabb) || caabb != ret) {
				return false;
			}
		}
		aabb = ret;
		return true;
	} 
	else
	{
		const sm::rect& src = actor->GetAABB().GetRect();

		sm::vec2 min(src.xmin, src.ymin),
			     max(src.xmax, src.ymax);
		S2_MAT mat = actor->GetLocalMat() * actor->GetSpr()->GetLocalMat();
		min = mat * min;
		max = mat * max;

		sm::rect ret;
		ret.Combine(min);
		ret.Combine(max);
		aabb = ret;

		return true;
	}
}

bool ProxyHelper::ActorGetPos(const ActorPtr& actor, sm::vec2& pos)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		sm::vec2 ret;
		if (!ActorGetPos(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			sm::vec2 cpos;
			if (!ActorGetPos(items[i].second->QueryActorRef(items[i].first.get()), cpos) || cpos != ret) {
				return false;
			}
		}
		pos = ret;
		return true;
	} 
	else
	{
		pos = actor->GetPosition();
		return true;
	}
}

void ProxyHelper::ActorSetPos(ActorPtr& actor, const sm::vec2& pos)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const ActorPtr& child_actor = items[i].second->QueryActorRef(items[i].first.get());
			ActorSetPos(const_cast<ActorPtr&>(child_actor), pos);
		}
	} 
	else
	{
		actor->SetPosition(pos);
	}
}

bool ProxyHelper::ActorGetAngle(const ActorPtr& actor, float& angle)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		float ret;
		if (!ActorGetAngle(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			float cangle;
			if (!ActorGetAngle(items[i].second->QueryActorRef(items[i].first.get()), cangle) || cangle != ret) {
				return false;
			}
		}
		angle = ret;
		return true;
	} 
	else
	{
		angle = actor->GetAngle();
		return true;
	}
}

void ProxyHelper::ActorSetAngle(ActorPtr& actor, float angle)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const ActorPtr& child_actor = items[i].second->QueryActorRef(items[i].first.get());
			ActorSetAngle(const_cast<ActorPtr&>(child_actor), angle);
		}
	} 
	else
	{
		actor->SetAngle(angle);
	}
}

bool ProxyHelper::ActorGetScale(const ActorPtr& actor, sm::vec2& scale)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		sm::vec2 ret;
		if (!ActorGetScale(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			sm::vec2 cscale;
			if (!ActorGetScale(items[i].second->QueryActorRef(items[i].first.get()), cscale) || cscale != ret) {
				return false;
			}
		}
		scale = ret;
		return true;
	} 
	else
	{
		scale = actor->GetScale();
		return true;
	}
}

void ProxyHelper::ActorSetScale(ActorPtr& actor, const sm::vec2& scale)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const ActorPtr& child_actor = items[i].second->QueryActorRef(items[i].first.get());
			ActorSetScale(const_cast<ActorPtr&>(child_actor), scale);
		}
	} 
	else
	{
		actor->SetScale(scale);
	}
}

static S2_MAT 
_get_actor_world_mat(const ActorPtr& actor) 
{
	S2_MAT mat;
	ActorConstPtr curr = actor;
	while (curr) {
		mat = mat * actor->GetSpr()->GetLocalMat();
		mat = curr->GetLocalMat() * mat;
		curr = curr->GetParent();
	}
	return mat;
}

bool ProxyHelper::ActorGetWorldPos(const ActorPtr& actor, sm::vec2& pos)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		sm::vec2 ret;
		if (!ActorGetWorldPos(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		// FIXME
		// for (int i = 1, n = items.size(); i < n; ++i) {
		// 	sm::vec2 cpos;
		// 	if (!ActorGetWorldPos(items[i].second->QueryActorRef(items[i].first.get()), cpos) || cpos != ret) {
		// 		return false;
		// 	}
		// }
		pos = ret;
		return true;
	} 
	else
	{
		S2_MAT mat = _get_actor_world_mat(actor);
		pos = mat * sm::vec2(0, 0);
		return true;
	}
}

bool ProxyHelper::ActorGetWorldAngle(const ActorPtr& actor, float& angle)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		float ret;
		if (!ActorGetWorldAngle(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			float cangle;
			if (!ActorGetWorldAngle(items[i].second->QueryActorRef(items[i].first.get()), cangle) || cangle != ret) {
				return false;
			}
		}
		angle = ret;
		return true;
	} 
	else
	{
		S2_MAT mat = _get_actor_world_mat(actor);
		sm::vec2 pos = mat * sm::vec2(0, 0);
		sm::vec2 dir = mat * sm::vec2(1, 0);
		angle = sm::get_line_angle(pos, dir);
		return true;
	}
}

bool ProxyHelper::ActorGetWorldScale(const ActorPtr& actor, sm::vec2& scale)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		sm::vec2 ret;
		if (!ActorGetWorldScale(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			sm::vec2 cscale;
			if (!ActorGetWorldScale(items[i].second->QueryActorRef(items[i].first.get()), cscale) || cscale != ret) {
				return false;
			}
		}
		scale = ret;
		return true;
	} 
	else
	{
		S2_MAT mat = _get_actor_world_mat(actor);
		scale = (mat * sm::vec2(1, 1)) - (mat * sm::vec2(0, 0));
		return true;
	}
}

bool ProxyHelper::ActorGetVisible(const ActorPtr& actor, bool& visible)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		bool ret;
		if (!ActorGetVisible(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			bool cvisible;
			if (!ActorGetVisible(items[i].second->QueryActorRef(items[i].first.get()), cvisible) || cvisible != ret) {
				return false;
			}
		}
		visible = ret;
		return true;
	} 
	else
	{
		visible = actor->IsVisible();
		return true;
	}
}

void ProxyHelper::ActorSetVisible(ActorPtr& actor, bool visible)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const ActorPtr& child_actor = items[i].second->QueryActorRef(items[i].first.get());
			ActorSetVisible(const_cast<ActorPtr&>(child_actor), visible);
		}
	} 
	else
	{
		if (visible != actor->IsVisible()) {
			actor->SetVisible(visible, true);
		}
	}
}

bool ProxyHelper::ActorGetEditable(const ActorPtr& actor, bool& editable)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		bool ret;
		if (!ActorGetEditable(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			bool ceditable;
			if (!ActorGetEditable(items[i].second->QueryActorRef(items[i].first.get()), ceditable) || ceditable != ret) {
				return false;
			}
		}
		editable = ret;
		return true;
	} 
	else
	{
		editable = actor->IsEditable();
		return true;
	}
}

void ProxyHelper::ActorSetEditable(ActorPtr& actor, bool editable)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const ActorPtr& child_actor = items[i].second->QueryActorRef(items[i].first.get());
			ActorSetEditable(const_cast<ActorPtr&>(child_actor), editable);
		}
	} 
	else
	{
		actor->SetEditable(editable);
	}
}

bool ProxyHelper::ActorGetColMul(const ActorPtr& actor, uint32_t& mul)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		uint32_t ret;
		if (!ActorGetColMul(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			uint32_t cmul;
			if (!ActorGetColMul(items[i].second->QueryActorRef(items[i].first.get()), cmul) || cmul != ret) {
				return false;
			}
		}
		mul = ret;
		return true;
	} 
	else
	{
		mul = actor->GetColor().GetMulABGR();
		return true;
	}
}

void ProxyHelper::ActorSetColMul(ActorPtr& actor, uint32_t mul)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const ActorPtr& child_actor = items[i].second->QueryActorRef(items[i].first.get());
			ActorSetColMul(const_cast<ActorPtr&>(child_actor), mul);
		}
	} 
	else
	{
		if (actor->GetColor().GetMulABGR() != mul) 
		{
			RenderColor rc = actor->GetColor();
			rc.SetMulABGR(mul);
			const_cast<ActorPtr&>(actor)->SetColor(rc);
		}
	}
}

bool ProxyHelper::ActorGetColAdd(const ActorPtr& actor, uint32_t& add)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		uint32_t ret;
		if (!ActorGetColAdd(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			uint32_t cadd;
			if (!ActorGetColAdd(items[i].second->QueryActorRef(items[i].first.get()), cadd) || cadd != ret) {
				return false;
			}
		}
		add = ret;
		return true;
	} 
	else
	{
		add = actor->GetColor().GetAddABGR();
		return true;
	}
}

void ProxyHelper::ActorSetColAdd(ActorPtr& actor, uint32_t add)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const ActorPtr& child_actor = items[i].second->QueryActorRef(items[i].first.get());
			ActorSetColAdd(const_cast<ActorPtr&>(child_actor), add);
		}
	} 
	else
	{
		if (actor->GetColor().GetAddABGR() != add) 
		{
			RenderColor rc = actor->GetColor();
			rc.SetAddABGR(add);
			const_cast<ActorPtr&>(actor)->SetColor(rc);
		}
	}
}

bool ProxyHelper::ActorGetColMap(const ActorPtr& actor, uint32_t& rmap, uint32_t& gmap, uint32_t& bmap)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		uint32_t ret_r, ret_g, ret_b;
		if (!ActorGetColMap(items[0].second->QueryActorRef(items[0].first.get()), ret_r, ret_g, ret_b)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			uint32_t crmap, cgmap, cbmap;
			if (!ActorGetColMap(items[i].second->QueryActorRef(items[i].first.get()), crmap, cgmap, cbmap) || 
				crmap != ret_r || cgmap != ret_g || cbmap != ret_b) {
				return false;
			}
		}
		rmap = ret_r;
		gmap = ret_g;
		bmap = ret_b;
		return true;
	}
	else
	{
		rmap = actor->GetColor().GetRMapABGR();
		gmap = actor->GetColor().GetGMapABGR();
		bmap = actor->GetColor().GetBMapABGR();
		return true;
	}
}

void ProxyHelper::ActorSetColMap(ActorPtr& actor, uint32_t rmap, uint32_t gmap, uint32_t bmap)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const ActorPtr& child_actor = items[i].second->QueryActorRef(items[i].first.get());
			ActorSetColMap(const_cast<ActorPtr&>(child_actor), rmap, gmap, bmap);
		}
	} 
	else
	{
		if (actor->GetColor().GetRMapABGR() != rmap ||
			actor->GetColor().GetGMapABGR() != gmap ||
			actor->GetColor().GetBMapABGR() != bmap) 
		{
			RenderColor rc = actor->GetColor();
			rc.SetRMapABGR(rmap);
			rc.SetGMapABGR(gmap);
			rc.SetBMapABGR(bmap);
			const_cast<ActorPtr&>(actor)->SetColor(rc);
		}
	}
}

void ProxyHelper::ActorSetFilter(ActorPtr& actor, int mode)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const ActorPtr& child_actor = items[i].second->QueryActorRef(items[i].first.get());
			ActorSetFilter(const_cast<ActorPtr&>(child_actor), mode);
		}
	} 
	else
	{
		auto& filter = actor->GetShader().GetFilter();
		FilterMode ori = FM_NULL;
#ifdef S2_FILTER_FULL
		if (filter) {
			ori = filter->GetMode();
		}
#else
		ori = filter;
#endif // S2_FILTER_FULL
		if (ori == mode) {
			return;
		}

		RenderShader shader = actor->GetShader();
		shader.SetFilter(FilterMode(mode));
		const_cast<ActorPtr&>(actor)->SetShader(shader);
	}
}

bool ProxyHelper::ActorGetFrame(const ActorPtr& actor, int& frame)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		int ret;
		if (!ActorGetFrame(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			int cframe;
			if (!ActorGetFrame(items[i].second->QueryActorRef(items[i].first.get()), cframe) || cframe != ret) {
				return false;
			}
		}
		frame = ret;
		return true;
	} 
	else if (type == SYM_ANIMATION)
	{
		auto anim = S2_VI_DOWN_CAST<AnimSprite*>(
			const_cast<Sprite*>(actor->GetSpr()));
		frame = anim->GetFrame(actor);
		return true;
	}
	else
	{
		return false;
	}
}

void ProxyHelper::ActorSetFrame(ActorPtr& actor, int frame)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const ActorPtr& child_actor = items[i].second->QueryActorRef(items[i].first.get());
			ActorSetFrame(const_cast<ActorPtr&>(child_actor), frame);
		}
	} 
	else
	{
		const auto& spr = actor->GetSpr();

		SetStaticFrameVisitor visitor(frame);
		SprVisitorParams vp;
		vp.actor = actor;
		bool old_inherit_update = spr->IsInheritUpdate();
		spr->SetInheritUpdate(true);
		spr->Traverse(visitor, vp);
		spr->SetInheritUpdate(old_inherit_update);
	}
}

bool ProxyHelper::ActorGetComponentCount(const ActorPtr& actor, int& count)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		int ret;
		if (!ActorGetComponentCount(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			int ccount;
			if (!ActorGetComponentCount(items[i].second->QueryActorRef(items[i].first.get()), ccount) || ccount != ret) {
				return false;
			}
		}
		count = ret;
		return true;
	} 
	else if (type == SYM_COMPLEX)
	{
		const auto& spr = S2_VI_DOWN_CAST<ComplexSprite*>(
			const_cast<Sprite*>(actor->GetSpr()));
		auto& sym = S2_VI_PTR_DOWN_CAST<const ComplexSymbol>(spr->GetSymbol());
		count = sym->GetActionChildren(spr->GetAction()).size();
		return true;
	}
	else if (type == SYM_ANIMATION)
	{
		const auto& spr = S2_VI_DOWN_CAST<AnimSprite*>(
			const_cast<Sprite*>(actor->GetSpr()));
		const AnimCurr& curr = spr->GetOriginCurr(actor.get());
		count = curr.GetSlotSize();
		return true;
	}
	else
	{
		return false;
	}
}

void ProxyHelper::ActorSetAction(ActorPtr& actor, const CU_STR& action)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const ActorPtr& child_actor = items[i].second->QueryActorRef(items[i].first.get());
			ActorSetAction(const_cast<ActorPtr&>(child_actor), action);
		}
	} 
	else if (type == SYM_COMPLEX)
	{
		auto& sym_complex = S2_VI_PTR_DOWN_CAST<const ComplexSymbol>(sym);
		auto& actor_complex = std::static_pointer_cast<ComplexActor>(actor);
		int action_idx = sym_complex->GetActionIdx(action);
		actor_complex->SetAction(action_idx);
	}
}

bool ProxyHelper::ActorGetScissor(const ActorPtr& actor, sm::rect& rect)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY)
	{
		auto& proxy_sym = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym);
		auto& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		sm::rect ret;
		if (!ActorGetScissor(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			sm::rect crect;
			if (!ActorGetScissor(items[i].second->QueryActorRef(items[i].first.get()), crect) || crect != ret) {
				return false;
			}
		}
		rect = ret;
		return true;
	}
	else if (type == SYM_COMPLEX)
	{
		auto& complex = S2_VI_PTR_DOWN_CAST<const ComplexSymbol>(sym);
		rect = complex->GetScissor();
		return true;
	}
	else
	{
		return false;
	}
}

void ProxyHelper::ActorSetScissor(ActorPtr& actor, const sm::rect& rect)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY)
	{
		auto& proxy_sym = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym);
		auto& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			ActorSetScissor(items[i].second->QueryActorRef(items[i].first.get()), rect);
		}
	}
	else if (type == SYM_COMPLEX)
	{
		auto& sym_complex = S2_VI_PTR_DOWN_CAST<const ComplexSymbol>(sym);
		std::const_pointer_cast<ComplexSymbol>(sym_complex)->SetScissor(rect);
		
		// update aabb
		ActorAABB& aabb = actor->GetAABB();
		aabb.SetStaticRect(rect);
		aabb.UpdateParent(actor);
	}
}

bool ProxyHelper::ActorGetText(const ActorPtr& actor, CU_STR& text)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		
		if (items.empty()) {
			return false;
		}
		CU_STR ret;
		if (!ActorGetText(items[0].second->QueryActorRef(items[0].first.get()), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			CU_STR ctext;
			if (!ActorGetText(items[i].second->QueryActorRef(items[i].first.get()), ctext) || ctext != ret) {
				return false;
			}
		}
		text = ret;
		return true;
	} 
	else if (type == SYM_TEXTBOX)
	{
		auto& textbox = S2_VI_PTR_DOWN_CAST<TextboxActor>(actor);
		text = textbox->GetText();
		return true;
	}
	else
	{
		return false;
	}
}

void ProxyHelper::ActorSetText(ActorPtr& actor, const CU_STR& text)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const ActorPtr& child_actor = items[i].second->QueryActorRef(items[i].first.get());
			ActorSetText(const_cast<ActorPtr&>(child_actor), text);
		}
	} 
	else if (type == SYM_TEXTBOX)
	{
		auto& textbox = std::static_pointer_cast<TextboxActor>(actor);
		textbox->SetText(text);
	}
}

void ProxyHelper::ActorScale9Resize(ActorPtr& actor, int w, int h)
{
	auto& sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(sym)->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const ActorPtr& child_actor = items[i].second->QueryActorRef(items[i].first.get());
			ActorScale9Resize(const_cast<ActorPtr&>(child_actor), w, h);
		}
	} 
	else if (type == SYM_SCALE9)
	{
		auto& s9_actor = std::static_pointer_cast<Scale9Actor>(actor);
		s9_actor->Resize(static_cast<float>(w), static_cast<float>(h));
	}
}

}