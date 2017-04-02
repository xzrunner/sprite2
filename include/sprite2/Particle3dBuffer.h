#ifndef _SPRITE2_PARTICLE3D_BUFFER_H_
#define _SPRITE2_PARTICLE3D_BUFFER_H_

#include <SM_Vector.h>
#include <CU_Singleton.h>

#include <list>

namespace s2
{

class Particle3dEmitter;

class Particle3dBuffer
{
public:
	void Insert(Particle3dEmitter* emitter);
	void Remove(Particle3dEmitter* emitter);
	void Clear();

	bool Update(float time);
	void Draw(const sm::vec2& pos, float scale) const;

	static void InitCB(void (*update_srt_func)(void* params, float x, float y, float scale), void (*remove_func)(Particle3dEmitter*));

private:
	std::list<Particle3dEmitter*> m_emitters;

	SINGLETON_DECLARATION(Particle3dBuffer)

}; // Particle3dBuffer

}

#endif // _SPRITE2_PARTICLE3D_BUFFER_H_