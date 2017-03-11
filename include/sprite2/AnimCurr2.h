#ifndef _SPRITE2_ANIM_CURR2_H_
#define _SPRITE2_ANIM_CURR2_H_

#include "S2_Message.h"

#include <CU_Uncopyable.h>

#include <vector>
#include <string>

namespace s2
{

class AnimCopy;
class Sprite;
class AnimSymbol;
class SpriteVisitor;
class SprVisitorParams;
class RenderParams;

class AnimCurr2 : private cu::Uncopyable
{
public:
	AnimCurr2();
	AnimCurr2(const AnimCurr2& curr);
	AnimCurr2& operator = (const AnimCurr2& curr);
	~AnimCurr2();
	
	bool Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const;

	void OnMessage(Message msg);

	bool Update(const RenderParams& rp, bool loop = true, 
		float interval = 0, int fps = 30);
	void Draw(const RenderParams& rp) const;

	Sprite* FetchChild(const std::string& name) const;

	void Start();

	void SetTime(float time);
	void SetFrame(int frame, int fps);

	int GetFrame() const { return m_frame; }

	void SetAnimCopy(const AnimCopy* copy);

	void SetActive(bool active);
	bool IsActive() const { return m_active; }

	void Clear();

private:
	void ResetTime();

	void ResetLayerCursor();

	void LoadCurrSprites();

private:
	const AnimCopy* m_copy;

	std::vector<int> m_layer_cursor;

	std::vector<Sprite*> m_slots;

	std::vector<int> m_curr;

	int m_frame;

	float m_start_time, m_curr_time;
	float m_stop_time, m_stop_during;

	bool m_active;

}; // AnimCurr2

}

#endif // _SPRITE2_ANIM_CURR2_H_