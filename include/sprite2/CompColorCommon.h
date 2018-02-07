#pragma once

#include "sprite2/Component.h"

#include <painting2/RenderColorCommon.h>

namespace s2
{

class CompColorCommon : public Component
{
public:
	virtual CompColorCommon* Clone() const;

	virtual size_t GetSize() const { return sizeof(CompColorCommon); }

	const pt2::RenderColorCommon& GetColor() const { return m_col; }
	pt2::RenderColorCommon& GetColor() { return m_col; }
	
	void SetColor(const pt2::RenderColorCommon& col) { m_col = col; }

	void Reset();

	static CompColorCommon* Create();

private:
	pt2::RenderColorCommon m_col;
	
}; // CompColorCommon

}