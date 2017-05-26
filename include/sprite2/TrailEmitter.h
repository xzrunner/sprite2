#ifndef _SPRITE2_TRAIL_EMITTER_H_
#define _SPRITE2_TRAIL_EMITTER_H_

#include "ObjectPool.h"

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>
#include <SM_Vector.h>

struct t2d_emitter;

namespace s2
{

class TrailEmitterCfg;
class TrailRenderParams;

class TrailEmitter : public cu::RefCountObj, private cu::Uncopyable
{
public:
	TrailEmitter();
	~TrailEmitter();

	virtual void RemoveReference() const;

	bool Update(float time, const sm::vec2& pos);
	void Draw(const TrailRenderParams& rp) const;

	void Start();
	void Stop();

	void Clear();

	float GetTime() const;

	void CreateEmitter(const TrailEmitterCfg* cfg);

	const TrailEmitterCfg* GetEmitterCfg() const { return m_state.cfg; }

	/**
	 *  @interface
	 *    ObjectPool
	 */
	void Init();
	void Term();
	TrailEmitter* GetNext() const { return m_state.next; }
	void SetNext(TrailEmitter* next) { m_state.next = next; }

private:
	union
	{
		struct
		{
			const TrailEmitterCfg* cfg;
			t2d_emitter* et;
		};

		TrailEmitter* next;

	} m_state;

}; // TrailEmitter

typedef ObjectPool<TrailEmitter> TrailEmitterPool;

}

#endif // _SPRITE2_TRAIL_EMITTER_H_
