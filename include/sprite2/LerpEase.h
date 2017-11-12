#ifndef _SPRITE2_LERP_EASE_H_
#define _SPRITE2_LERP_EASE_H_

#include "sprite2/ILerp.h"
#include "sprite2/LerpType.h"

namespace s2
{

class LerpEase : public ILerp
{
public:
	enum EaseType
	{
		EASE_UNKNOWN = 0,

		EASE_LINEAR,

		EASE_IN_QUAD,
		EASE_IN_CUBIC,
		EASE_IN_QUART,
		EASE_IN_QUINT,
		EASE_IN_SINE,
		EASE_IN_EXPO,
		EASE_IN_CIRC,
		EASE_IN_ELASTIC,
		EASE_IN_BACK,
		EASE_IN_BOUNCE,

		EASE_OUT_QUAD,
		EASE_OUT_CUBIC,
		EASE_OUT_QUART,
		EASE_OUT_QUINT,
		EASE_OUT_SINE,
		EASE_OUT_EXPO,
		EASE_OUT_CIRC,
		EASE_OUT_ELASTIC,
		EASE_OUT_BACK,
		EASE_OUT_BOUNCE,

		EASE_IN_OUT_QUAD,
		EASE_IN_OUT_CUBIC,
		EASE_IN_OUT_QUART,
		EASE_IN_OUT_QUINT,
		EASE_IN_OUT_SINE,
		EASE_IN_OUT_EXPO,
		EASE_IN_OUT_CIRC,
		EASE_IN_OUT_ELASTIC,
		EASE_IN_OUT_BACK,
		EASE_IN_OUT_BOUNCE,
	};

public:
	LerpEase();
	LerpEase(const LerpEase& lerp);
	LerpEase& operator = (const LerpEase& lerp);
	LerpEase(int type);

	virtual std::unique_ptr<ILerp> Clone() const override { return std::make_unique<LerpEase>(*this); }

	virtual int Type() const override { return LERP_EASE; }

	template<typename T>
	T Lerp(const T& begin, const T& end, float t) const
	{
		t = GetRealTime(t);
		return begin + (end - begin) * t;
	}

	int GetEaseType() const { return m_type; }

private:
	float GetRealTime(float t) const;

private:
	int m_type;

}; // LerpEase

}

#endif // _SPRITE2_LERP_EASE_H_