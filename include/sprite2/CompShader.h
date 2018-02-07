#pragma once

#include "sprite2/Component.h"

#include <painting2/RenderShader.h>

namespace s2
{

class CompShader : public Component
{
public:
	virtual CompShader* Clone() const;

	virtual size_t GetSize() const { return sizeof(CompShader); }

	const pt2::RenderShader& GetShader() const { return m_shader; }
	pt2::RenderShader& GetShader() { return m_shader; }

	void SetShader(const pt2::RenderShader& shader) { m_shader = shader; }

	static CompShader* Create();

private:
	pt2::RenderShader m_shader;

}; // CompShader

}