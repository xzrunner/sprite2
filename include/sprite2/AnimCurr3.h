#ifndef _SPRITE2_ANIM_CURR3_H_
#define _SPRITE2_ANIM_CURR3_H_

#include "S2_Message.h"
#include "AnimCopy2.h"

#include <CU_Uncopyable.h>

#include <vector>

namespace s2
{

class AnimCopy2;
class Sprite;
class AnimSymbol;
class SprVisitor;
class SprVisitorParams;
class RenderParams;

class AnimCurr3 : private cu::Uncopyable
{
public:
	AnimCurr3();
	AnimCurr3(const AnimCurr3& curr);
	AnimCurr3& operator = (const AnimCurr3& curr);
	~AnimCurr3();
	
	bool Traverse(SprVisitor& visitor, const SprVisitorParams& params) const;

	void OnMessage(Message msg);

	bool Update(const RenderParams& params, bool loop = true, 
		float interval = 0, int fps = 30);
	void Draw(const RenderParams& params) const;

	Sprite* FetchChild(const std::string& name) const;

	void Start();

	void SetTime(float time);
	void SetFrame(int frame, int fps);

	int GetFrame() const { return m_frame; }

	void SetAnimCopy(const AnimCopy2* copy);

	void SetActive(bool active);
	bool IsActive() const { return m_active; }

	void Clear();

private:
	void ResetTime();

	void ResetLayerCursor();

	void LoadCurrSprites();

	static void LoadSprLerpData(Sprite* spr, const AnimCopy2::Lerp& lerp, int time);
	
private:
	const AnimCopy2* m_copy;

	std::vector<int> m_layer_cursor;

	std::vector<Sprite*> m_slots;

	int* m_curr;
	int  m_curr_num;

	int m_frame;

	float m_start_time, m_curr_time;
	float m_stop_time, m_stop_during;

	bool m_active;

}; // AnimCurr3

}

#endif // _SPRITE2_ANIM_CURR3_H_