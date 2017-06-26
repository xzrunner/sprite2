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
#include "AnimCurr.h"

#include "ComplexSymbol.h"
#include "ComplexSprite.h"
#include "ComplexActor.h"
#include "AnimSymbol.h"
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

Sprite* ProxyHelper::BuildGroup(const std::vector<std::pair<const Actor*, Sprite*> >& items)
{
	Sprite* ret = NULL;
	if (items.size() == 1) 
	{
		ret = items[0].second;
	} 
	else if (items.size() > 1) 
	{
		ProxySymbol* sym = new ProxySymbol(items);
		ProxySprite* spr = new ProxySprite(sym);
		sym->RemoveReference();
		assert(sym->GetRefCount() == 1 && spr->GetRefCount() == 1);
		ret = spr;
	}
	return ret;
}

/************************************************************************/
/* sprite                                                               */
/************************************************************************/

bool ProxyHelper::SprGetPos(const Sprite* spr, sm::vec2& pos)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		sm::vec2 ret;
		if (!SprGetPos(items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			sm::vec2 cpos;
			if (!SprGetPos(items[i].second, cpos) || cpos != ret) {
				return false;
			}
		}
		pos = ret;
		return true;
	} 
	else 
	{
		pos = spr->GetPosition();
		return true;
	}
}

bool ProxyHelper::SprGetAngle(const Sprite* spr, float& angle)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		float ret;
		if (!SprGetAngle(items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			float cangle;
			if (!SprGetAngle(items[i].second, cangle) || cangle != ret) {
				return false;
			}
		}
		angle = ret;
		return true;
	} 
	else 
	{
		angle = spr->GetAngle();
		return true;
	}
}

bool ProxyHelper::SprGetScale(const Sprite* spr, sm::vec2& scale)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		sm::vec2 ret;
		if (!SprGetScale(items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			sm::vec2 cscale;
			if (!SprGetScale(items[i].second, cscale) || cscale != ret) {
				return false;
			}
		}
		scale = ret;
		return true;
	} 
	else 
	{
		scale = spr->GetScale();
		return true;
	}
}

bool ProxyHelper::SprGetSymID(const Sprite* spr, int& id)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		int ret;
		if (!SprGetSymID(items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			int cid;
			if (!SprGetSymID(items[i].second, cid) || cid != ret) {
				return false;
			}
		}
		id = ret;
		return true;
	} 
	else 
	{
		id = spr->GetSymbol()->GetID();
		return true;
	}
}

bool ProxyHelper::SprGetSymType(const Sprite* spr, int& type)
{
	const Symbol* sym = spr->GetSymbol();
	type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		int ret;
		if (!SprGetSymType(items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			int ctype;
			if (!SprGetSymType(items[i].second, ctype) || ctype != ret) {
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

bool ProxyHelper::SprPointTest(const Sprite* spr, const sm::vec2& pos)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			if (SprPointTest(items[i].second, pos)) {
				return true;
			}
		}
		return false;
	}
	else
	{
		return spr->GetBounding()->IsContain(pos);
	}
}

const Actor* ProxyHelper::SprPointQuery(const Sprite* spr, const sm::vec2& pos, float mat[6])
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			if (const Actor* actor = SprPointQuery(items[i].second, pos, mat)) {
				return actor;
			}
		}
		return NULL;
	}
	else
	{
		PointQueryVisitor visitor(pos);

		SprVisitorParams params;
		spr->Traverse(visitor, params);
		const Actor* ret = visitor.GetSelectedActor();
		if (!ret) {
			return NULL;
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

bool ProxyHelper::SprGetForceUpdate(const Sprite* spr, bool& force)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		bool ret;
		if (!SprGetForceUpdate(items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			bool cforce;
			if (!SprGetForceUpdate(items[i].second, cforce) || cforce != ret) {
				return false;
			}
		}
		force = ret;
		return true;
	} 
	else
	{
		force = spr->IsForceUpdate();
		return true;
	}
}

void ProxyHelper::SprSetForceUpdate(Sprite* spr, bool force)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprSetForceUpdate(items[i].second, force);
		}
	}
	else
	{
		spr->SetForceUpdate(force);
	}
}

void ProxyHelper::SprSetInheritUpdate(const Sprite* spr, bool inherit)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprSetInheritUpdate(items[i].second, inherit);
		}
	}
	else
	{
		spr->SetInheritUpdate(inherit);
	}
}

int ProxyHelper::SprGetInheritUpdate(const Sprite* spr)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
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
		return spr->IsInheritUpdate() ? 1 : 0;
	}
}

bool ProxyHelper::SprGetFrameCount(const Sprite* spr, int& count)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		int ret;
		if (!SprGetFrameCount(items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			int ccount;
			if (!SprGetFrameCount(items[i].second, ccount) || ccount != ret) {
				return false;
			}
		}
		count = ret;
		return true;
	} 
	else if (type == SYM_ANIMATION)
	{
		const AnimSymbol* sym = VI_DOWNCASTING<const AnimSymbol*>(spr->GetSymbol());
		count = sym->GetMaxFrameIdx();
		return true;
	}
	else
	{
		return false;
	}
}

bool ProxyHelper::SprHasAction(const Sprite* spr, const std::string& action)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			if (SprHasAction(items[i].second, action)) {
				return true;
			}
		}
		return false;
	}
	else if (type == SYM_COMPLEX)
	{
		const ComplexSymbol* sym_complex = VI_DOWNCASTING<const ComplexSymbol*>(sym);
		int idx = sym_complex->GetActionIdx(action);
		return idx != -1;
	}
	else
	{
		return false;
	}
}

bool ProxyHelper::SprGetScissor(const Sprite* spr, sm::rect& rect)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		sm::rect ret;
		if (!SprGetScissor(items[0].second, ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			sm::rect crect;
			if (!SprGetScissor(items[i].second, crect) || crect != ret) {
				return false;
			}
		}
		rect = ret;
		return true;
	} 
	else if (type == SYM_COMPLEX)
	{
		const ComplexSymbol* sym = VI_DOWNCASTING<const ComplexSymbol*>(spr->GetSymbol());
		rect = sym->GetScissor();
		return true;
	}
	else
	{
		return false;
	}
}

void ProxyHelper::SprSetScissor(Sprite* spr, const sm::rect& rect)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprSetScissor(items[i].second, rect);
		}
	}
	else if (type == SYM_COMPLEX)
	{
		const ComplexSymbol* sym_complex = VI_DOWNCASTING<const ComplexSymbol*>(sym);
		const_cast<ComplexSymbol*>(sym_complex)->SetScissor(rect);
	}
}

void ProxyHelper::SprTextboxResetTime(Sprite* spr)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprTextboxResetTime(items[i].second);
		}
	}
	else if (type == SYM_TEXTBOX)
	{
		TextboxSprite* tb_spr = VI_DOWNCASTING<TextboxSprite*>(spr);
		tb_spr->ResetTime();
	}
}

void ProxyHelper::SprAnim2SetStaticTime(Sprite* spr, int time)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprAnim2SetStaticTime(items[i].second, time);
		}
	}
	else if (type == SYM_ANIM2)
	{
		Anim2Sprite* a2_spr = VI_DOWNCASTING<Anim2Sprite*>(spr);
		a2_spr->SetStaticTime(s2::UpdateParams(), time);
	}
}

void ProxyHelper::SprP3dSetLocal(Sprite* spr, bool local)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprP3dSetLocal(items[i].second, local);
		}
	}
	else if (type == SYM_PARTICLE3D)
	{
		Particle3dSprite* p3d_spr = VI_DOWNCASTING<Particle3dSprite*>(spr);
		p3d_spr->SetLocal(local);
	}
}

void ProxyHelper::SprP3dSetLoop(Sprite* spr, bool loop)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprP3dSetLoop(items[i].second, loop);
		}
	}
	else if (type == SYM_PARTICLE3D)
	{
		Particle3dSprite* p3d_spr = VI_DOWNCASTING<Particle3dSprite*>(spr);
		p3d_spr->SetLoop(loop);
	}
}

bool ProxyHelper::SprP3dIsFinished(const Sprite* spr)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			if (!SprP3dIsFinished(items[i].second)) {
				return false;
			}
		}
		return true;
	} 
	else if (type == SYM_PARTICLE3D)
	{
		const Particle3dSprite* p3d_spr = VI_DOWNCASTING<const Particle3dSprite*>(spr);
		if (const Particle3dEmitter* et = p3d_spr->GetEmitter()) {
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

void ProxyHelper::SprP3dUpdate(Sprite* spr, float dt)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SprP3dUpdate(items[i].second, dt);
		}
	}
	else if (type == SYM_PARTICLE3D)
	{
		Particle3dSprite* p3d_spr = VI_DOWNCASTING<Particle3dSprite*>(spr);
		if (const Particle3dEmitter* et = p3d_spr->GetEmitter()) {
			const_cast<Particle3dEmitter*>(et)->Update(et->GetTime() + dt);
		}
	}
}

void ProxyHelper::SprGetProxyChildren(const Sprite* spr, std::vector<const Actor*>& actors)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type != SYM_PROXY) {
		return;
	}

	const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
	const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
	actors.reserve(actors.size() + items.size());
	for (int i = 0, n = items.size(); i < n; ++i) 
	{
		const Actor* child_actor = items[i].second->QueryActor(items[i].first);
		if (child_actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
			SprGetProxyChildren(child_actor->GetSpr(), actors);
		} else {
			actors.push_back(child_actor);
		}
	}
}

void ProxyHelper::SetDTexEnable(Sprite* spr, bool enable)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SetDTexEnable(items[i].second, enable);
		}
	}
	else
	{
		VI_DOWNCASTING<Sprite*>(spr)->SetDTexDisable(!enable);
	}
}

void ProxyHelper::SetDTexForceCached(Sprite* spr, bool cache)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SetDTexForceCached(items[i].second, cache);
		}
	}
	else
	{
		spr->SetDTexForceCached(cache);
		if (cache) {
			spr->SetDTexForceCachedDirty(true);
		}
	}
}

void ProxyHelper::SetDTexForceCachedDirty(Sprite* spr, bool dirty)
{
	const Symbol* sym = spr->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			SetDTexForceCachedDirty(items[i].second, dirty);
		}
	}
	else
	{
		spr->SetDTexForceCachedDirty(dirty);
	}
}

/************************************************************************/
/* actor                                                                */
/************************************************************************/	

bool ProxyHelper::ActorGetAABB(const Actor* actor, sm::rect& aabb)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		sm::rect ret;
		if (!ActorGetAABB(items[0].second->QueryActor(items[0].first), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			sm::rect caabb;
			if (!ActorGetAABB(items[i].second->QueryActor(items[i].first), caabb) || caabb != ret) {
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

bool ProxyHelper::ActorGetPos(const Actor* actor, sm::vec2& pos)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		sm::vec2 ret;
		if (!ActorGetPos(items[0].second->QueryActor(items[0].first), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			sm::vec2 cpos;
			if (!ActorGetPos(items[i].second->QueryActor(items[i].first), cpos) || cpos != ret) {
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

void ProxyHelper::ActorSetPos(Actor* actor, const sm::vec2& pos)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			ActorSetPos(const_cast<Actor*>(child_actor), pos);
		}
	} 
	else
	{
		actor->SetPosition(pos);
	}
}

bool ProxyHelper::ActorGetAngle(const Actor* actor, float& angle)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		float ret;
		if (!ActorGetAngle(items[0].second->QueryActor(items[0].first), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			float cangle;
			if (!ActorGetAngle(items[i].second->QueryActor(items[i].first), cangle) || cangle != ret) {
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

void ProxyHelper::ActorSetAngle(Actor* actor, float angle)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			ActorSetAngle(const_cast<Actor*>(child_actor), angle);
		}
	} 
	else
	{
		actor->SetAngle(angle);
	}
}

bool ProxyHelper::ActorGetScale(const Actor* actor, sm::vec2& scale)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		sm::vec2 ret;
		if (!ActorGetScale(items[0].second->QueryActor(items[0].first), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			sm::vec2 cscale;
			if (!ActorGetScale(items[i].second->QueryActor(items[i].first), cscale) || cscale != ret) {
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

void ProxyHelper::ActorSetScale(Actor* actor, const sm::vec2& scale)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			ActorSetScale(const_cast<Actor*>(child_actor), scale);
		}
	} 
	else
	{
		actor->SetScale(scale);
	}
}

static S2_MAT 
_get_actor_world_mat(const Actor* actor) 
{
	S2_MAT mat;
	while (actor) {
		mat = mat * actor->GetSpr()->GetLocalMat();
		mat = actor->GetLocalMat() * mat;
		actor = actor->GetParent();
	}
	return mat;
}

bool ProxyHelper::ActorGetWorldPos(const Actor* actor, sm::vec2& pos)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		sm::vec2 ret;
		if (!ActorGetWorldPos(items[0].second->QueryActor(items[0].first), ret)) {
			return false;
		}
		// FIXME
		// for (int i = 1, n = items.size(); i < n; ++i) {
		// 	sm::vec2 cpos;
		// 	if (!ActorGetWorldPos(items[i].second->QueryActor(items[i].first), cpos) || cpos != ret) {
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

bool ProxyHelper::ActorGetWorldAngle(const Actor* actor, float& angle)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		float ret;
		if (!ActorGetWorldAngle(items[0].second->QueryActor(items[0].first), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			float cangle;
			if (!ActorGetWorldAngle(items[i].second->QueryActor(items[i].first), cangle) || cangle != ret) {
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

bool ProxyHelper::ActorGetWorldScale(const Actor* actor, sm::vec2& scale)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		sm::vec2 ret;
		if (!ActorGetWorldScale(items[0].second->QueryActor(items[0].first), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			sm::vec2 cscale;
			if (!ActorGetWorldScale(items[i].second->QueryActor(items[i].first), cscale) || cscale != ret) {
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

bool ProxyHelper::ActorGetVisible(const Actor* actor, bool& visible)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		bool ret;
		if (!ActorGetVisible(items[0].second->QueryActor(items[0].first), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			bool cvisible;
			if (!ActorGetVisible(items[i].second->QueryActor(items[i].first), cvisible) || cvisible != ret) {
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

void ProxyHelper::ActorSetVisible(Actor* actor, bool visible)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			ActorSetVisible(const_cast<Actor*>(child_actor), visible);
		}
	} 
	else
	{
		if (visible != actor->IsVisible()) {
			actor->SetFlattenDirtyToRoot();
			actor->SetVisible(visible, true);
		}
	}
}

bool ProxyHelper::ActorGetEditable(const Actor* actor, bool& editable)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		bool ret;
		if (!ActorGetEditable(items[0].second->QueryActor(items[0].first), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			bool ceditable;
			if (!ActorGetEditable(items[i].second->QueryActor(items[i].first), ceditable) || ceditable != ret) {
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

void ProxyHelper::ActorSetEditable(Actor* actor, bool editable)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			ActorSetEditable(const_cast<Actor*>(child_actor), editable);
		}
	} 
	else
	{
		actor->SetEditable(editable);
	}
}

bool ProxyHelper::ActorGetColMul(const Actor* actor, uint32_t& mul)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		uint32_t ret;
		if (!ActorGetColMul(items[0].second->QueryActor(items[0].first), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			uint32_t cmul;
			if (!ActorGetColMul(items[i].second->QueryActor(items[i].first), cmul) || cmul != ret) {
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

void ProxyHelper::ActorSetColMul(Actor* actor, uint32_t mul)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			ActorSetColMul(const_cast<Actor*>(child_actor), mul);
		}
	} 
	else
	{
		if (actor->GetColor().GetMulABGR() != mul) 
		{
			RenderColor rc = actor->GetColor();
			rc.SetMulABGR(mul);
			const_cast<Actor*>(actor)->SetColor(rc);
		}
	}
}

bool ProxyHelper::ActorGetColAdd(const Actor* actor, uint32_t& add)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		uint32_t ret;
		if (!ActorGetColAdd(items[0].second->QueryActor(items[0].first), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			uint32_t cadd;
			if (!ActorGetColAdd(items[i].second->QueryActor(items[i].first), cadd) || cadd != ret) {
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

void ProxyHelper::ActorSetColAdd(Actor* actor, uint32_t add)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			ActorSetColAdd(const_cast<Actor*>(child_actor), add);
		}
	} 
	else
	{
		if (actor->GetColor().GetAddABGR() != add) 
		{
			RenderColor rc = actor->GetColor();
			rc.SetAddABGR(add);
			const_cast<Actor*>(actor)->SetColor(rc);
		}
	}
}

bool ProxyHelper::ActorGetColMap(const Actor* actor, uint32_t& rmap, uint32_t& gmap, uint32_t& bmap)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		uint32_t ret_r, ret_g, ret_b;
		if (!ActorGetColMap(items[0].second->QueryActor(items[0].first), ret_r, ret_g, ret_b)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			uint32_t crmap, cgmap, cbmap;
			if (!ActorGetColMap(items[i].second->QueryActor(items[i].first), crmap, cgmap, cbmap) || 
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

void ProxyHelper::ActorSetColMap(Actor* actor, uint32_t rmap, uint32_t gmap, uint32_t bmap)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			ActorSetColMap(const_cast<Actor*>(child_actor), rmap, gmap, bmap);
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
			const_cast<Actor*>(actor)->SetColor(rc);
		}
	}
}

void ProxyHelper::ActorSetFilter(Actor* actor, int mode)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			ActorSetFilter(const_cast<Actor*>(child_actor), mode);
		}
	} 
	else
	{
		const RenderFilter* filter = actor->GetShader().GetFilter();
		FilterMode ori = FM_NULL;
		if (filter) {
			ori = filter->GetMode();
		}
		if (ori == mode) {
			return;
		}

		RenderShader shader = actor->GetShader();
		shader.SetFilter(FilterMode(mode));
		const_cast<Actor*>(actor)->SetShader(shader);
	}
}

bool ProxyHelper::ActorGetFrame(const Actor* actor, int& frame)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		int ret;
		if (!ActorGetFrame(items[0].second->QueryActor(items[0].first), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			int cframe;
			if (!ActorGetFrame(items[i].second->QueryActor(items[i].first), cframe) || cframe != ret) {
				return false;
			}
		}
		frame = ret;
		return true;
	} 
	else if (type == SYM_ANIMATION)
	{
		const AnimSprite* anim = VI_DOWNCASTING<const AnimSprite*>(actor->GetSpr());
		frame = anim->GetFrame(actor);
		return true;
	}
	else
	{
		return false;
	}
}

void ProxyHelper::ActorSetFrame(Actor* actor, int frame)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			ActorSetFrame(const_cast<Actor*>(child_actor), frame);
		}
	} 
	else
	{
		const Sprite* spr = actor->GetSpr();

		SetStaticFrameVisitor visitor(frame);
		SprVisitorParams vp;
		vp.actor = actor;
		bool old_inherit_update = spr->IsInheritUpdate();
		spr->SetInheritUpdate(true);
		spr->Traverse(visitor, vp);
		spr->SetInheritUpdate(old_inherit_update);
	}
}

bool ProxyHelper::ActorGetComponentCount(const Actor* actor, int& count)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		int ret;
		if (!ActorGetComponentCount(items[0].second->QueryActor(items[0].first), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			int ccount;
			if (!ActorGetComponentCount(items[i].second->QueryActor(items[i].first), ccount) || ccount != ret) {
				return false;
			}
		}
		count = ret;
		return true;
	} 
	else if (type == SYM_COMPLEX)
	{
		const ComplexSprite* complex = VI_DOWNCASTING<const ComplexSprite*>(actor->GetSpr());
		const ComplexSymbol* sym = VI_DOWNCASTING<const ComplexSymbol*>(complex->GetSymbol());
		count = sym->GetActionChildren(complex->GetAction()).size();
		return true;
	}
	else if (type == SYM_ANIMATION)
	{
		const AnimSprite* anim = VI_DOWNCASTING<const AnimSprite*>(actor->GetSpr());
		const AnimCurr* curr = anim->GetAnimCurr(actor);
		if (curr) {
			count = curr->GetSlotSize();
			return true;
		} else {
			return false;
		}
	}
	else
	{
		return false;
	}
}

void ProxyHelper::ActorSetAction(Actor* actor, const std::string& action)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			ActorSetAction(const_cast<Actor*>(child_actor), action);
		}
	} 
	else if (type == SYM_COMPLEX)
	{
		const ComplexSymbol* sym_complex = VI_DOWNCASTING<const ComplexSymbol*>(sym);
		ComplexActor* actor_complex = static_cast<ComplexActor*>(actor);
		int action_idx = sym_complex->GetActionIdx(action);
		actor_complex->SetAction(action_idx);
	}
}

bool ProxyHelper::ActorGetText(const Actor* actor, std::string& text)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		if (items.empty()) {
			return false;
		}
		std::string ret;
		if (!ActorGetText(items[0].second->QueryActor(items[0].first), ret)) {
			return false;
		}
		for (int i = 1, n = items.size(); i < n; ++i) {
			std::string ctext;
			if (!ActorGetText(items[i].second->QueryActor(items[i].first), ctext) || ctext != ret) {
				return false;
			}
		}
		text = ret;
		return true;
	} 
	else if (type == SYM_TEXTBOX)
	{
		const TextboxActor* textbox = VI_DOWNCASTING<const TextboxActor*>(actor);
		text = textbox->GetText().c_str();
		return true;
	}
	else
	{
		return false;
	}
}

void ProxyHelper::ActorSetText(Actor* actor, const std::string& text)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			ActorSetText(const_cast<Actor*>(child_actor), text);
		}
	} 
	else if (type == SYM_TEXTBOX)
	{
		const TextboxActor* textbox = static_cast<const TextboxActor*>(actor);
		const_cast<TextboxActor*>(textbox)->SetText(text);
	}
}

void ProxyHelper::ActorScale9Resize(Actor* actor, int w, int h)
{
	const Symbol* sym = actor->GetSpr()->GetSymbol();
	int type = sym->Type();
	if (type == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(sym);
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			ActorScale9Resize(const_cast<Actor*>(child_actor), w, h);
		}
	} 
	else if (type == SYM_SCALE9)
	{
		const Scale9Actor* s9_actor = static_cast<const Scale9Actor*>(actor);
		const_cast<Scale9Actor*>(s9_actor)->Resize(w, h);	
	}
}

}