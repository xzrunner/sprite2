#include "AnimSprite.h"
#include "AnimSymbol.h"
#include "AnimCurr.h"
#include "AnimActor.h"
#include "UpdateParams.h"
#include "SprVisitorParams.h"

#include <stdlib.h>

namespace s2
{

AnimSprite::AnimSprite() 
	: m_loop(true)
	, m_interval(0)
	, m_fps(30)
	, m_start_random(false)
	, m_curr(new AnimCurr)
{
}

AnimSprite::AnimSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
	, m_loop(true)
	, m_interval(0)
	, m_fps(VI_DOWNCASTING<AnimSymbol*>(sym)->GetFPS())
	, m_start_random(false)
	, m_curr(new AnimCurr)
{
	m_curr->SetAnimCopy(&VI_DOWNCASTING<AnimSymbol*>(m_sym)->GetCopy());
	m_curr->Start(UpdateParams(), this);
}

AnimSprite::AnimSprite(const AnimSprite& spr)
	: Sprite(spr)
{
	this->operator = (spr);
}

AnimSprite& AnimSprite::operator = (const AnimSprite& spr)
{
	m_loop = spr.m_loop;
	m_interval = spr.m_interval;
	m_fps = spr.m_fps;
	m_start_random = spr.m_start_random;
	m_curr = new AnimCurr(*spr.m_curr);
	m_curr->Start(UpdateParams(), this);
	return *this;
}

AnimSprite::~AnimSprite()
{
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
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(up.GetActor()));
	curr.OnMessage(up, this, msg);
	switch (msg)
	{
	case MSG_START:
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
	const Actor* actor = up.GetActor();
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(actor));
	return curr.Update(up, this, m_loop, m_interval, m_fps);
}

bool AnimSprite::SetFrame(const UpdateParams& up, int frame, bool force)
{
	if (!force && !IsInheritUpdate()) {
		return false;
	}
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(up.GetActor()));
	curr.SetFrame(up, this, frame, m_fps);
	return true;
}

Sprite* AnimSprite::FetchChild(const std::string& name, const Actor* actor) const
{
	return GetAnimCurr(actor).FetchChild(name);
}

Sprite* AnimSprite::FetchChild(int idx, const Actor* actor) const
{
	return GetAnimCurr(actor).FetchChild(idx);
}

// use TraverseChildren instead?
void AnimSprite::SetChildAction(const Actor* actor, int symid, const char* action)
{
	const_cast<AnimCurr&>(GetAnimCurr(actor)).SetChildAction(actor, symid, action);
}

VisitResult AnimSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	const Actor* actor = params.actor;
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(actor));
	return curr.Traverse(visitor, params);
}

const AnimCurr& AnimSprite::GetAnimCurr(const Actor* actor) const 
{ 
	const AnimCurr* curr = m_curr;
// 	if (ActorCount() > 1)
// 	{
		const AnimActor* anim_actor = static_cast<const AnimActor*>(actor);
		if (anim_actor && anim_actor->GetCurr()) {
			curr = anim_actor->GetCurr();
		}
//	}
	return *curr;
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
	return GetAnimCurr(actor).GetFrame(); 
}

void AnimSprite::SetActive(bool active, const Actor* actor)
{
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(actor));
	curr.SetActive(active);
}

void AnimSprite::RandomStartTime(const UpdateParams& up)
{
	int start = VI_DOWNCASTING<const AnimSymbol*>(m_sym)->GetMaxFrameIdx();
	float p = (rand() / static_cast<float>(RAND_MAX));
	start *= p;
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(up.GetActor()));
//	curr.SetTime(start / m_fps);
	curr.SetFrame(up, this, start, m_fps);
}

}