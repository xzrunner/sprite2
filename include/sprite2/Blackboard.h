#ifndef _SPRITE2_BLACKBOARD_H_
#define _SPRITE2_BLACKBOARD_H_

#include <CU_Singleton.h>
#include <SM_Vector.h>

namespace s2
{

class Camera;

class Blackboard
{
public:
	int  GetScreenCacheTexID() const { return m_screen_cache_texid; }
	void SetScreenCacheTexID(int texid) { m_screen_cache_texid = texid; }

	const Camera* GetCamera() const { return m_cam; }
	void SetCamera(const Camera* cam) { m_cam = cam; }

	const sm::ivec2& GetScreenSize() const { return m_screen_sz; }
	void SetScreenSize(int w, int h) { m_screen_sz.x = w; m_screen_sz.y = h; }

private:
	int m_screen_cache_texid;

	const Camera* m_cam;

	sm::ivec2 m_screen_sz;
	
	SINGLETON_DECLARATION(Blackboard);

}; // Blackboard
	
}

#endif // _SPRITE2_BLACKBOARD_H_