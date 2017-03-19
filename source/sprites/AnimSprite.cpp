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
	m_curr->Start(SprTreePath());
}

AnimSprite::AnimSprite(const AnimSprite& spr)
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
	m_curr->Start(SprTreePath());
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

void AnimSprite::OnMessage(Message msg, const SprTreePath& path)
{
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(path));
	curr.OnMessage(msg, path);

	switch (msg)
	{
	case MSG_START:
		curr.Start(path);
		if (m_start_random) {
			RandomStartTime(path);
		}
		break;
	default:
		break;
	}
}
bool AnimSprite::Update(const RenderParams& rp)
{
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(rp.path));
	return curr.Update(rp, m_loop, m_interval, m_fps);
}

bool AnimSprite::SetFrame(int frame, const SprTreePath& parent_path, bool force)
{
	if (!force && !IsForceUpFrame() && !GetName().empty()) {
		return false;
	}
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(parent_path));
	curr.SetFrame(frame, m_fps, parent_path);
	return true;
}

Sprite* AnimSprite::FetchChild(const std::string& name, const SprTreePath& path) const
{
	return GetAnimCurr(path).FetchChild(name, path);
}

VisitResult AnimSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(params.path));
	return curr.Traverse(visitor, params);
}

const AnimCurr& AnimSprite::GetAnimCurr(const SprTreePath& path) const 
{ 
	const AnimCurr* curr = m_curr;
	if (ActorCount() > 1)
	{
		SprTreePath cpath = path;
		cpath.Push(GetID());
		const AnimActor* actor = static_cast<const AnimActor*>(QueryActor(cpath));
		if (actor && actor->GetCurr()) {
			curr = actor->GetCurr();
		}
	}
	return *curr;
}

void AnimSprite::SetStartRandom(bool random, const SprTreePath& path) 
{ 
	m_start_random = random; 
	if (m_start_random) {
		RandomStartTime(path);
	}
}

int AnimSprite::GetFrame(const SprTreePath& path) const 
{ 
	return GetAnimCurr(path).GetFrame(); 
}

void AnimSprite::SetActive(bool active, const SprTreePath& path)
{
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(path));
	curr.SetActive(active);
}

void AnimSprite::RandomStartTime(const SprTreePath& path)
{
	int start = VI_DOWNCASTING<const AnimSymbol*>(m_sym)->GetMaxFrameIdx();
	float p = (rand() / static_cast<float>(RAND_MAX));
	start *= p;
	AnimCurr& curr = const_cast<AnimCurr&>(GetAnimCurr(path));
//	curr.SetTime(start / m_fps);
	curr.SetFrame(start, m_fps, path);
}

}