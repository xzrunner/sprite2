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
	void SetScreenCacheTexID(int tex_id) { m_screen_cache_texid = tex_id; }

	Camera* GetCamera() const { return m_cam; }
	void SetCamera(Camera* cam) { m_cam = cam; }

	const sm::ivec2& GetScreenSize() const { return m_screen_sz; }
	void SetScreenSize(int w, int h);

private:
	int m_screen_cache_texid;

	Camera* m_cam;

	sm::ivec2 m_screen_sz;
	
	SINGLETON_DECLARATION(Blackboard);

}; // Blackboard
	
}

#endif // _SPRITE2_BLACKBOARD_H_