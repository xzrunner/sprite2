#ifndef _SPRITE2_PARTICLE3D_BUFFER_H_
#define _SPRITE2_PARTICLE3D_BUFFER_H_

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>
#include <SM_Vector.h>

#include <memory>

namespace s2
{

class Particle3dEmitter;

class Particle3dBuffer
{
public:
	void Insert(const std::shared_ptr<Particle3dEmitter>& emitter);
	void Remove(const std::shared_ptr<Particle3dEmitter>& emitter);
	void Clear();

	bool Update(float time);
	void Draw(const sm::vec2& pos, float scale) const;

	static void InitCB(void (*update_srt_func)(void* params, float x, float y, float scale), void (*remove_func)(Particle3dEmitter*));

private:
	CU_LIST<std::weak_ptr<Particle3dEmitter>> m_emitters;

	CU_SINGLETON_DECLARATION(Particle3dBuffer)

}; // Particle3dBuffer

}

#endif // _SPRITE2_PARTICLE3D_BUFFER_H_