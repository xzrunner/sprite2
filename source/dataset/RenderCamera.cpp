#include "RenderCamera.h"

#include <sm_const.h>
#include <SM_Math.h>

#include <string.h>
#include <vector>
#include <float.h>

namespace s2
{

static const float HEIGHT_VAL = 1.414f;

RenderCamera::RenderCamera()
{
	Init();
}

bool RenderCamera::operator == (const RenderCamera& cam) const
{
	return m_state.mode == cam.m_state.mode
		&& m_state.base_y == cam.m_state.base_y;
}

RenderCamera RenderCamera::operator * (const RenderCamera& rc) const
{
	RenderCamera ret;
	if (rc.m_state.mode != CM_ORTHO) {
		ret.m_state.mode = rc.m_state.mode;
	} else {
		ret.m_state.mode = m_state.mode;
	}
	if (ret.m_state.mode == CM_PERSPECTIVE_AUTO_HEIGHT) {
		if (rc.m_state.base_y == FLT_MAX) {
			ret.m_state.base_y = rc.m_state.base_y;
			// todo pass spr
// 			std::vector<sm::vec2> bound;
// 			spr->GetBounding()->GetBoundPos(bound);
// 			for (int i = 0, n = bound.size(); i < n; ++i) {
// 				if (bound[i].y < ret.base_y) {
// 					ret.base_y = bound[i].y;
// 				}
// 			}
		} else {
			ret.m_state.base_y = rc.m_state.base_y;
		}
	}
	return ret;
}

void RenderCamera::CalculateZ(float cam_angle, const float vertices[8], float z[4]) const
{
	if (m_state.mode == CM_ORTHO || m_state.mode == CM_PERSPECTIVE_NO_HEIGHT) {
		memset(z, 0, sizeof(float) * 4);
		return;
	}

	float zs = sm::sin(cam_angle * SM_DEG_TO_RAD);

	float ymin = FLT_MAX, ymax = -FLT_MAX;
	for (int i = 0; i < 4; ++i) {
		float y = vertices[i * 2 + 1];
		if (y < ymin) ymin = y;
		if (y > ymax) ymax = y;
	}

	//	float zoff = 0;
	if (m_state.base_y != FLT_MAX) {
		// 		assert(ymin >= base_y);
		// 		zoff = (ymin - base_y) * HEIGHT_VAL;

		ymin -= (ymin - m_state.base_y);
	}

	float height = (ymax - ymin) * HEIGHT_VAL;
	//	float height = ymax - ymin;
	for (int i = 0; i < 4; ++i) {
		float y = vertices[i * 2 + 1];
		z[i] = -(y - ymin) / (ymax - ymin) * height * zs;
	}
}

void RenderCamera::Init()
{
	m_state.mode = CM_ORTHO;
	m_state.base_y = 0;
}

}
