#ifndef _SPRITE2_RENDER_CAMERA_H_
#define _SPRITE2_RENDER_CAMERA_H_

#include "CameraMode.h"
#include "ObjectPool.h"

#include <SM_Vector.h>

namespace s2
{

class RenderCamera
{
public:
	RenderCamera();

	RenderCamera operator * (const RenderCamera& rc) const;

	void CalculateZ(float cam_angle, sm::vec2 vertices[4], float z[4]) const;

	CameraMode GetMode() const { return m_state.mode; }
	void SetMode(CameraMode mode) { m_state.mode = mode; }

	/**
	 *  @interface
	 *    ObjectPool
	 */
	void Init();
	void Term() {}
	RenderCamera* GetNext() const { return m_state.next; }
	void SetNext(RenderCamera* next) { m_state.next = next; }

private:
	union
	{
		struct 
		{
			CameraMode	mode;
			float		base_y;	
		};

		RenderCamera* next;

	} m_state;

}; // RenderCamera

typedef ObjectPool<RenderCamera> RenderCameraPool;

}

#endif // _SPRITE2_RENDER_CAMERA_H_