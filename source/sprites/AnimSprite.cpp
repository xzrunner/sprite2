#include "AnimSprite.h"
#include "AnimSymbol.h"

namespace s2
{

AnimSprite::AnimSprite() 
	: m_loop(true)
	, m_interval(0)
	, m_fps(30)
	, m_start_random(false)
{
}

AnimSprite::AnimSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
	, m_loop(true)
	, m_interval(0)
	, m_fps(VI_DOWNCASTING<AnimSymbol*>(sym)->GetFPS())
	, m_start_random(false)
#ifdef S2_ANIM_CURR_V0
	, m_curr(VI_DOWNCASTING<AnimSymbol*>(m_sym))
#endif // S2_ANIM_CURR_V0
{
#ifndef S2_ANIM_CURR_V0
	m_curr.SetAnimCopy(&VI_DOWNCASTING<AnimSymbol*>(m_sym)->GetCopy());
#endif // S2_ANIM_CURR_V0
	m_curr.Start();
}

AnimSprite* AnimSprite::Clone() const
{
	return new AnimSprite(*this);
}

void AnimSprite::OnMessage(Message msg)
{
	m_curr.OnMessage(msg);

	switch (msg)
	{
	case MSG_START:
		m_curr.Start();
		if (m_start_random) {
			RandomStartTime();
		}
		break;
	}
}

bool AnimSprite::Update(const RenderParams& params)
{
	return m_curr.Update(params, m_loop, m_interval, m_fps);
}

bool AnimSprite::SetFrame(int frame)
{
	m_curr.SetFrame(frame, m_fps);
	return true;
}

Sprite* AnimSprite::FetchChild(const std::string& name) const
{
	return m_curr.FetchChild(name);
}

void AnimSprite::SetStartRandom(bool random) 
{ 
	m_start_random = random; 
	if (m_start_random) {
		RandomStartTime();
	}
}

void AnimSprite::SetActive(bool active)
{
	m_curr.SetActive(active);
}

bool AnimSprite::TraverseChildren(SprVisitor& visitor, const SprVisitorParams& params) const
{
	return m_curr.Traverse(visitor, params);
}

void AnimSprite::RandomStartTime()
{
	int start = VI_DOWNCASTING<const AnimSymbol*>(m_sym)->GetMaxFrameIdx();
	float p = (rand() / static_cast<float>(RAND_MAX));
	start *= p;
//	m_curr.SetTime(start / m_fps);
	m_curr.SetFrame(start, m_fps);
}

}