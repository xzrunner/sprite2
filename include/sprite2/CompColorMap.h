#pragma once

#include "sprite2/Component.h"

#include <painting2/RenderColorMap.h>

namespace s2
{

class CompColorMap : public Component
{
public:
	virtual ~CompColorMap();

	virtual CompColorMap* Clone() const;

	const pt2::RenderColorMap& GetColor() const { return m_col; }
	pt2::RenderColorMap& GetColor() { return m_col; }

	void SetColor(const pt2::RenderColorMap& col) { m_col = col; }

	void Reset();

	static CompColorMap* Create();

private:
	pt2::RenderColorMap m_col;
	
}; // CompColorMap

}