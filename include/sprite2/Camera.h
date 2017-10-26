#ifndef _SPRITE2_CAMERA_H_
#define _SPRITE2_CAMERA_H_

#include "sprite2/CameraType.h"

namespace s2
{

class Camera
{
public:
	virtual ~Camera() {}
	virtual CameraType Type() const = 0;
	virtual void OnSize(int width, int height) = 0;
	virtual void Reset() = 0;
	virtual void Bind() const = 0;

}; // Camera

}

#endif // _SPRITE2_CAMERA_H_
