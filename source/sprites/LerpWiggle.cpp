#include "LerpWiggle.h"

#include <cmath>

#include <assert.h>

namespace s2
{

LerpWiggle::LerpWiggle()
	: m_freq(0)
	, m_amp(0)
{
}

LerpWiggle::LerpWiggle(const LerpWiggle& lerp)
	: m_freq(lerp.m_freq)
	, m_amp(lerp.m_amp)
{
}

LerpWiggle& LerpWiggle::operator = (const LerpWiggle& lerp)
{
	m_freq = lerp.m_freq;
	m_amp  = lerp.m_amp;
	return *this;
}

LerpWiggle::LerpWiggle(float freq, float amp)
	: m_freq(freq)
	, m_amp(amp)
{
}

sm::vec2 LerpWiggle::Lerp(const sm::vec2& v, float time) const
{
	int iter = std::floor(time * m_freq);
	while (m_rands.size() <= (iter + 1) * 2) 
	{
		float px = (rand() / static_cast<float>(RAND_MAX));
		m_rands.push_back(px);

		float py = (rand() / static_cast<float>(RAND_MAX));
		m_rands.push_back(py);
	}

	sm::vec2 prev, next;
	prev.x = v.x + (-m_amp + m_amp * 2 * m_rands[iter * 2]);
	prev.y = v.y + (-m_amp + m_amp * 2 * m_rands[iter * 2 + 1]);
	next.x = v.x + (-m_amp + m_amp * 2 * m_rands[iter * 2 + 2]);
	next.y = v.y + (-m_amp + m_amp * 2 * m_rands[iter * 2 + 3]);

	float p = time * m_freq - iter;
	return prev + (next - prev) * p;
}

}