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
	, m_curr(nullptr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_ANIMATION);
#endif // S2_DISABLE_STATISTICS
}

AnimSprite::AnimSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
	, m_loop(true)
	, m_interval(0)
	, m_fps(VI_DOWNCASTING<AnimSymbol*>(sym)->GetFPS())
	, m_start_random(false)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_ANIMATION);
#endif // S2_DISABLE_STATISTICS

	AnimSymbol* anim_sym = VI_DOWNCASTING<AnimSymbol*>(m_sym);
	m_curr = std::make_unique<AnimCurr>();
	m_curr->SetAnimCopy(anim_sym->GetCopy());
	m_curr->Start(UpdateParams(), this);
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
	m_loop = spr.m_loop;
	m_interval = spr.m_interval;
	m_fps = spr.m_fps;
	m_start_random = spr.m_start_random;
	if (spr.m_curr) {
		m_curr = spr.m_curr->Clone();
		m_curr->Start(UpdateParams(), this);
	} else {
		m_curr = nullptr;
	}
	return *this;
}

AnimSprite::~AnimSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_ANIMATION);
#endif // S2_DISABLE_STATISTICS
}

AnimSprite* AnimSprite::Clone() const
{
	return new AnimSprite(*this);
}

void AnimSprite::OnMessage(const UpdateParams& up, Message msg)
{
	// update inherit
	if (!up.IsForce() && !IsInheritUpdate()) {
		return;
	}

	AnimCurr& curr = GetAnimCurr(up.GetActor());
	curr.OnMessage(up, this, msg);
	switch (msg)
	{
	case MSG_START: case MSG_TRIGGER:
		curr.Start(up, this);
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
	const Actor* actor = up.GetActor();
	bool visible = actor ? actor->IsVisible() : IsVisible();
	if (!visible) {
		return false;
	}

	AnimCurr& curr = GetAnimCurr(up.GetActor());
	return curr.Update(up, GetSymbol(), this, m_loop, m_interval, m_fps);
}

bool AnimSprite::AutoUpdate(const Actor* actor)
{
	AnimCurr& curr = GetAnimCurr(actor);
	return curr.Update(UpdateParams(), GetSymbol(), this, m_loop, m_interval, m_fps);
}

Sprite* AnimSprite::FetchChildByName(int name, const Actor* actor) const
{
	AnimCurr& curr = GetAnimCurr(actor);
	return curr.FetchChildByName(name, actor);
}

Sprite* AnimSprite::FetchChildByIdx(int idx, const Actor* actor) const
{
	AnimCurr& curr = GetAnimCurr(actor);
	return curr.FetchChildByIdx(idx);
}

VisitResult AnimSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	const Actor* actor = params.actor;
	AnimCurr& curr = GetAnimCurr(actor);
	return curr.Traverse(visitor, params);
}

AnimCurr& AnimSprite::GetAnimCurr(const Actor* actor) const 
{ 
	const AnimActor* anim_actor = static_cast<const AnimActor*>(actor);
	if (anim_actor && anim_actor->GetCurr()) {
		return *anim_actor->GetCurr();
	} else {
		assert(m_curr);
		return *m_curr;		
	}
}

void AnimSprite::SetStartRandom(const UpdateParams& up, bool random) 
{ 
	m_start_random = random; 
	if (m_start_random) {
		RandomStartTime(up);
	}
}

int AnimSprite::GetFrame(const Actor* actor) const 
{ 
	AnimCurr& curr = GetAnimCurr(actor);
	return curr.GetFrame();
}

bool AnimSprite::SetFrame(const UpdateParams& up, int frame)
{
	if (!up.IsForce() && !IsInheritUpdate()) {
		return false;
	}

	AnimCurr& curr = GetAnimCurr(up.GetActor());
	return curr.SetFrame(up, this, frame, m_fps);
}

void AnimSprite::SetActive(bool active, const Actor* actor)
{
	AnimCurr& curr = GetAnimCurr(actor);
	curr.SetActive(active);
}

void AnimSprite::RandomStartTime(const UpdateParams& up)
{
	int start = VI_DOWNCASTING<const AnimSymbol*>(m_sym)->GetMaxFrameIdx();
	float p = (rand() / static_cast<float>(RAND_MAX));
	start = static_cast<int>(start * p);

	AnimCurr& curr = GetAnimCurr(up.GetActor());
	curr.SetFrame(up, this, start, m_fps);
}

}