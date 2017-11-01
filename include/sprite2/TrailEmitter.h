#ifndef _SPRITE2_TRAIL_EMITTER_H_
#define _SPRITE2_TRAIL_EMITTER_H_

#include <cu/uncopyable.h>
#include <SM_Vector.h>

#include <memory>

struct t2d_emitter;

namespace s2
{

class TrailEmitterCfg;
class TrailRenderParams;

class TrailEmitter : private cu::Uncopyable
{
public:
	TrailEmitter();
	~TrailEmitter();

	bool Update(float time, const sm::vec2& pos);
	void Draw(const TrailRenderParams& rp) const;

	void Start();
	void Stop();

	void Clear();

	float GetTime() const;

	void CreateEmitter(const std::shared_ptr<const TrailEmitterCfg>& cfg);

	const std::shared_ptr<const TrailEmitterCfg>& GetEmitterCfg() const { return m_cfg; }

private:
	std::shared_ptr<const TrailEmitterCfg> m_cfg = nullptr;
	t2d_emitter* m_et;

}; // TrailEmitter

}

#endif // _SPRITE2_TRAIL_EMITTER_H_
