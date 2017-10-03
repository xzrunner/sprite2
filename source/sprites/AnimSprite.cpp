#include "AnimSprite.h"
#include "AnimSymbol.h"
#include "AnimActor.h"
#include "UpdateParams.h"
#include "SprVisitorParams.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS
#include "sprite2/AnimCurr.h"

#include <stdlib.h>
#include <assert.h>

namespace s2
{

AnimSprite::AnimSprite() 
	: m_loop(true)
	, m_interval(0)
	, m_fps(30)
	, m_start_random(false)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_ANIMATION);
#endif // S2_DISABLE_STATISTICS
}

AnimSprite::AnimSprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id)
	, m_loop(true)
	, m_interval(0)
	, m_fps(S2_VI_PTR_DOWN_CAST<AnimSymbol>(sym)->GetFPS())
	, m_start_random(false)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_ANIMATION);
#endif // S2_DISABLE_STATISTICS

	auto& anim_sym = S2_VI_PTR_DOWN_CAST<AnimSymbol>(m_sym);
	m_state.Init(anim_sym->GetCopy());
}

AnimSprite::AnimSprite(const AnimSprite& spr)
	: Sprite(spr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_ANIMATION);
#endif // S2_DISABLE_STATISTICS

	this->operator = (spr);
}

AnimSprite& AnimSprite::operator = (const AnimSprite& spr)
{
	m_loop     = spr.m_loop;
	m_interval = spr.m_interval;

	m_fps = spr.m_fps;

	m_start_random = spr.m_start_random;

	m_state.Assign(spr.m_state, &GetSymbol() == &spr.GetSymbol());

	return *this;
}

AnimSprite::~AnimSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_ANIMATION);
#endif // S2_DISABLE_STATISTICS
}

void AnimSprite::OnMessage(const UpdateParams& up, Message msg)
{
	// update inherit
	if (!up.IsForce() && !IsInheritUpdate()) {
		return;
	}

	AnimCurr& curr = GetOriginCurr(up.GetActor());
	curr.OnMessage(up, shared_from_this(), msg);
	switch (msg)
	{
	case MSG_START: case MSG_TRIGGER:
		curr.Start(up, shared_from_this());
		if (m_start_random) {
			RandomStartTime(up);
		}
		break;
	default:
		break;
	}
}

bool AnimSprite::Update(const UpdateParams& up)
{
	// update inherit
	if (!up.IsForce() && !IsInheritUpdate()) {
		return false;
	}

	// visible
	auto& actor = up.GetActor();
	bool visible = actor ? actor->IsVisible() : IsVisible();
	if (!visible) {
		return false;
	}

	AnimCurr& curr = GetUpdateCurr(up.GetActor());
	return curr.Update(up, GetSymbol(), shared_from_this(), m_loop, m_interval, m_fps);
}

bool AnimSprite::AutoUpdate(const ActorConstPtr& actor)
{
	AnimCurr& curr = GetUpdateCurr(actor);
	return curr.Update(UpdateParams(), GetSymbol(), shared_from_this(), m_loop, m_interval, m_fps);
}

SprPtr AnimSprite::FetchChildByName(int name, const ActorConstPtr& actor) const
{
	const AnimCurr& curr = GetOriginCurr(actor);
	return curr.FetchChildByName(name, actor);
}

SprPtr AnimSprite::FetchChildByIdx(int idx, const ActorPtr& actor) const
{
	const AnimCurr& curr = GetOriginCurr(actor);
	return curr.FetchChildByIdx(idx);
}

VisitResult AnimSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	auto& actor = params.actor;
	const AnimCurr& curr = GetOriginCurr(actor);
	return curr.Traverse(visitor, params);
}

AnimCurr& AnimSprite::GetOriginCurr(const ActorConstPtr& actor)
{
	auto& anim_actor = std::const_pointer_cast<AnimActor>(
		S2_VI_PTR_DOWN_CAST<const AnimActor>(actor));
	return anim_actor ? anim_actor->GetState().GetOrigin() : m_state.GetOrigin();
}

AnimCurr& AnimSprite::GetUpdateCurr(const ActorConstPtr& actor)
{
	auto& anim_actor = std::const_pointer_cast<AnimActor>(
		S2_VI_PTR_DOWN_CAST<const AnimActor>(actor));
#ifdef S2_MULTITHREAD
	return anim_actor ? anim_actor->GetState().GetUpdate() : m_state.GetUpdate();
#else
	return anim_actor ? anim_actor->GetState().GetOrigin() : m_state.GetOrigin();
#endif // S2_MULTITHREAD
}

AnimCurr& AnimSprite::GetDrawCurr(const ActorConstPtr& actor)
{
	auto& anim_actor = std::const_pointer_cast<AnimActor>(
		S2_VI_PTR_DOWN_CAST<const AnimActor>(actor));
#ifdef S2_MULTITHREAD
	return anim_actor ? anim_actor->GetState().GetDraw() : m_state.GetDraw();
#else
	return anim_actor ? anim_actor->GetState().GetOrigin() : m_state.GetOrigin();
#endif // S2_MULTITHREAD
}

const AnimCurr& AnimSprite::GetOriginCurr(const ActorConstPtr& actor) const
{
	auto& anim_actor = std::const_pointer_cast<AnimActor>(
		S2_VI_PTR_DOWN_CAST<const AnimActor>(actor));
	return anim_actor ? anim_actor->GetState().GetOrigin() : m_state.GetOrigin();
}

const AnimCurr& AnimSprite::GetUpdateCurr(const ActorConstPtr& actor) const
{
	auto& anim_actor = std::const_pointer_cast<AnimActor>(
		S2_VI_PTR_DOWN_CAST<const AnimActor>(actor));
#ifdef S2_MULTITHREAD
	return anim_actor ? anim_actor->GetState().GetUpdate() : m_state.GetUpdate();
#else
	return anim_actor ? anim_actor->GetState().GetOrigin() : m_state.GetOrigin();
#endif // S2_MULTITHREAD
}

const AnimCurr& AnimSprite::GetDrawCurr(const ActorConstPtr& actor) const
{
	auto& anim_actor = std::const_pointer_cast<AnimActor>(
		S2_VI_PTR_DOWN_CAST<const AnimActor>(actor));
#ifdef S2_MULTITHREAD
	return anim_actor ? anim_actor->GetState().GetDraw() : m_state.GetDraw();
#else
	return anim_actor ? anim_actor->GetState().GetOrigin() : m_state.GetOrigin();
#endif // S2_MULTITHREAD
}

void AnimSprite::SetStartRandom(const UpdateParams& up, bool random) 
{ 
	m_start_random = random; 
	if (m_start_random) {
		RandomStartTime(up);
	}
}

int AnimSprite::GetFrame(const ActorConstPtr& actor) const 
{ 
	const AnimCurr& curr = GetOriginCurr(actor);
	return curr.GetFrame();
}

bool AnimSprite::SetFrame(const UpdateParams& up, int frame)
{
	if (!up.IsForce() && !IsInheritUpdate()) {
		return false;
	}

	AnimCurr& curr = GetUpdateCurr(up.GetActor());
	return curr.SetFrame(up, shared_from_this(), frame, m_fps);
}

void AnimSprite::SetActive(bool active, const ActorConstPtr& actor)
{
	AnimCurr& curr = GetOriginCurr(actor);
	curr.SetActive(active);
}

void AnimSprite::RandomStartTime(const UpdateParams& up)
{
	int start = S2_VI_PTR_DOWN_CAST<const AnimSymbol>(m_sym)->GetMaxFrameIdx();
	float p = (rand() / static_cast<float>(RAND_MAX));
	start = static_cast<int>(start * p);

	AnimCurr& curr = GetOriginCurr(up.GetActor());
	curr.SetFrame(up, shared_from_this(), start, m_fps);
}

}