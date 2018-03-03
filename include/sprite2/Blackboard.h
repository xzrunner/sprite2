#ifndef _SPRITE2_BLACKBOARD_H_
#define _SPRITE2_BLACKBOARD_H_

#include <cu/cu_macro.h>
#include <SM_Vector.h>

#include <memory>

namespace pt2 { class Camera; }

namespace s2
{

class Blackboard
{
public:
	int  GetScreenCacheTexID() const { return m_screen_cache_texid; }
	void SetScreenCacheTexID(int tex_id) { m_screen_cache_texid = tex_id; }

	const std::shared_ptr<pt2::Camera>& GetCamera() const { return m_cam; }
	void SetCamera(const std::shared_ptr<pt2::Camera>& cam) { m_cam = cam; }

	void SetScreenSize(int w, int h);

	void SetDlistEnable(bool enable) { m_dlist_enable = enable; }
	bool IsDlistEnable() const { return m_dlist_enable; }

	void Clear();

private:
	int m_screen_cache_texid;

	std::shared_ptr<pt2::Camera> m_cam = nullptr;

	bool m_dlist_enable;
	
	CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard
	
}

#endif // _SPRITE2_BLACKBOARD_H_