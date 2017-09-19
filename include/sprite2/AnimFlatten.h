#ifndef _SPRITE2_ANIM_FLATTEN_H_
#define _SPRITE2_ANIM_FLATTEN_H_

#include "SymbolFlatten.h"
#include "sprite2/Flatten.h"

namespace s2
{

class AnimFlatten : public SymbolFlatten
{
public:
	virtual void UpdateTexcoords();

	void Clear();

	RenderReturn Draw(const RenderParams& rp, int frame) const;
	bool Update(const UpdateParams& up, int frame);
	void SetFrame(const UpdateParams& up, int frame);

	void SetFrames(const std::vector<Flatten>& frames) { m_frames = frames; }
	
private:
	std::vector<Flatten> m_frames;

}; // AnimFlatten

}

#endif // _SPRITE2_ANIM_FLATTEN_H_