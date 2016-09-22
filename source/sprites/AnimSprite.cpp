#include "AnimSprite.h"
#include "AnimSymbol.h"

namespace s2
{

AnimSprite::AnimSprite() 
	: m_loop(true)
	, m_interval(0)
	, m_fps(30)
{
}

AnimSprite::AnimSprite(Symbol* sym) 
	: Sprite(sym)
	, m_curr(VI_DOWNCASTING<AnimSymbol*>(sym))
	, m_loop(true)
	, m_interval(0)
	, m_fps(VI_DOWNCASTING<AnimSymbol*>(sym)->GetFPS())
{
	m_curr.Start();
}

AnimSprite* AnimSprite::Clone() const
{
	return new AnimSprite(*this);
}

void AnimSprite::OnMessage(Message msg)
{
	m_curr.OnMessage(msg);

	if (msg == MSG_START) {
		m_curr.Start();
	}
}

bool AnimSprite::Update(const RenderParams& params)
{
	return m_curr.Update(params, m_loop, m_interval, m_fps);
}

Sprite* AnimSprite::FetchChild(const std::string& name) const
{
	return m_curr.FetchChild(name);
}

void AnimSprite::SetActive(bool active)
{
	m_curr.SetActive(active);
}

}