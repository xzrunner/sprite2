#pragma once

#include "sprite2/SprComponent.h"

#include <painting2/RenderShader.h>

namespace s2
{

class CompShader : public SprComponent
{
public:
	virtual ~CompShader();

	virtual CompShader* Clone() const;

	const pt2::RenderShader& GetShader() const { return m_shader; }
	pt2::RenderShader& GetShader() { return m_shader; }

	void SetShader(const pt2::RenderShader& shader) { m_shader = shader; }

	static CompShader* Create();

private:
	pt2::RenderShader m_shader;

}; // CompShader

}