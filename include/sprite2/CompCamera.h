#pragma once

#include "sprite2/Component.h"

#include <painting2/RenderCamera.h>

namespace s2
{

class CompCamera : public Component
{
public:
	virtual ~CompCamera();

	virtual CompCamera* Clone() const;

	const pt2::RenderCamera& GetCamera() const { return m_cam; }
	pt2::RenderCamera& GetCamera() { return m_cam; }

	void SetCamera(const pt2::RenderCamera& cam) { m_cam = cam; }

	static CompCamera* Create();

private:
	pt2::RenderCamera m_cam;

}; // CompCamera

}