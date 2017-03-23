#include "AnimSprite.h"
#include "AnimSymbol.h"
#include "AnimCurr.h"
#include "AnimActor.h"
#include "RenderParams.h"
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
	m_curr->Start(NULL);
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
	m_curr->Start(NULL);
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

void AnimSprite::OnMessage(Message msg, const Actor* actor)
{
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(actor));
	curr.OnMessage(msg, actor);
	switch (msg)
	{
	case MSG_START:
		curr.Start(actor);
		if (m_start_random) {
			RandomStartTime(actor);
		}
		break;
	default:
		break;
	}
}

bool AnimSprite::Update(const RenderParams& rp)
{
	const Actor* actor = rp.actor;
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(actor));

	RenderParams rp_child = rp;
	rp_child.mt = GetLocalMat() * rp.mt;
	rp_child.shader = GetShader() * rp.shader;
	return curr.Update(rp_child, m_loop, m_interval, m_fps);
}

bool AnimSprite::SetFrame(int frame, const Actor* actor, bool force)
{
	if (!force && !IsForceUpFrame() && !GetName().empty()) {
		return false;
	}
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(actor));
	curr.SetFrame(frame, m_fps, actor);
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

void AnimSprite::SetStartRandom(bool random, const Actor* actor) 
{ 
	m_start_random = random; 
	if (m_start_random) {
		RandomStartTime(actor);
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

void AnimSprite::RandomStartTime(const Actor* actor)
{
	int start = VI_DOWNCASTING<const AnimSymbol*>(m_sym)->GetMaxFrameIdx();
	float p = (rand() / static_cast<float>(RAND_MAX));
	start *= p;
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(actor));
//	curr.SetTime(start / m_fps);
	curr.SetFrame(start, m_fps, actor);
}

}