#pragma once

#include "sprite2/config.h"

#ifndef S2_DISABLE_CAMERA25

#include "sprite2/Camera.h"
#include "sprite2/CameraType.h"

#include <SM_Vector.h>
#include <SM_Matrix.h>

struct c25_camera;
struct sm_vec3;
union sm_mat4;

namespace s2
{

class Pseudo3DCamera : public Camera
{
public:
	Pseudo3DCamera();
	Pseudo3DCamera(const Pseudo3DCamera& cam);
	Pseudo3DCamera& operator = (const Pseudo3DCamera& cam);
	virtual ~Pseudo3DCamera();

	virtual CameraType Type() const override { return CAM_PSEUDO3D; }
	virtual void OnSize(int width, int height) override;
	virtual void Reset() override;
	virtual void Bind() const override;

	sm::vec2 TransPosScreenToProject(int x, int y, int width, int height) const;
	sm::vec2 TransPosProjectToScreen(const sm::vec3& proj, int width, int height) const;

	void Translate(const sm::vec3& offset);
	void Rotate(float da);

	float GetAngle() const;
	const sm_vec3* GetPos() const;

 	const sm_mat4* GetModelViewMat() const;
 	const sm_mat4* GetProjectMat() const;

private:
	void Init(const Pseudo3DCamera& cam);

	void UpdateRender() const;

private:
	c25_camera* m_cam;

}; // Pseudo3DCamera

}

#endif // S2_DISABLE_CAMERA25