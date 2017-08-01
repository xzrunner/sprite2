#include "AnimSprite.h"
#include "AnimSymbol.h"
#include "AnimFlattenCurr.h"
#include "AnimActor.h"
#include "UpdateParams.h"
#include "SprVisitorParams.h"
#include "AnimCurrCreator.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

#include <stdlib.h>
#include <assert.h>

namespace s2
{

AnimSprite::AnimSprite() 
	: m_loop(true)
	, m_interval(0)
	, m_fps(30)
	, m_start_random(false)
	, m_curr(NULL)
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
	m_curr = AnimCurrCreator::Create(anim_sym, this);
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
		m_curr = NULL;
	}
	return *this;
}

AnimSprite::~AnimSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_ANIMATION);
#endif // S2_DISABLE_STATISTICS

	if (m_curr) {
		m_curr->RemoveReference();
	}
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

	AnimCurr* curr = const_cast<AnimCurr*>(GetAnimCurr(up.GetActor()));
	assert(curr);
	curr->OnMessage(up, this, msg);
	switch (msg)
	{
	case MSG_START: case MSG_TRIGGER:
		curr->Start(up, this);
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

	AnimCurr* curr = const_cast<AnimCurr*>(GetAnimCurr(up.GetActor()));
	assert(curr);
	return curr->Update(up, GetSymbol(), this, m_loop, m_interval, m_fps);
}

Sprite* AnimSprite::FetchChildByName(int name, const Actor* actor) const
{
	const AnimCurr* curr = GetAnimCurr(actor);
	assert(curr);
	return curr->FetchChildByName(name, actor);
}

Sprite* AnimSprite::FetchChildByIdx(int idx, const Actor* actor) const
{
	const AnimCurr* curr = GetAnimCurr(actor);
	assert(curr);
	return curr->FetchChildByIdx(idx);
}

VisitResult AnimSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	const Actor* actor = params.actor;
	AnimCurr* curr = const_cast<AnimCurr*>(GetAnimCurr(actor));
	assert(curr);
	return curr->Traverse(visitor, params);
}

const AnimCurr* AnimSprite::GetAnimCurr(const Actor* actor) const 
{ 
	const AnimCurr* curr = m_curr;
// 	if (ActorCount() > 1)
// 	{
		const AnimActor* anim_actor = static_cast<const AnimActor*>(actor);
		if (anim_actor && anim_actor->GetCurr()) {
			curr = anim_actor->GetCurr();
		}
//	}
	return curr;
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
	const AnimCurr* curr = GetAnimCurr(actor);
	assert(curr);
	return curr->GetFrame();
}

void AnimSprite::SetFrame(const UpdateParams& up, int frame)
{
	if (!up.IsForce() && !IsInheritUpdate()) {
		return;
	}

	AnimCurr* curr = const_cast<AnimCurr*>(GetAnimCurr(up.GetActor()));
	assert(curr);
	return curr->SetFrame(up, this, frame, m_fps);
}

void AnimSprite::SetActive(bool active, const Actor* actor)
{
	AnimCurr* curr = const_cast<AnimCurr*>(GetAnimCurr(actor));
	assert(curr);
	curr->SetActive(active);
}

void AnimSprite::RandomStartTime(const UpdateParams& up)
{
	int start = VI_DOWNCASTING<const AnimSymbol*>(m_sym)->GetMaxFrameIdx();
	float p = (rand() / static_cast<float>(RAND_MAX));
	start *= p;

	AnimCurr* curr = const_cast<AnimCurr*>(GetAnimCurr(up.GetActor()));
	assert(curr);
//	curr.SetTime(start / m_fps);
	curr->SetFrame(up, this, start, m_fps);
}

}