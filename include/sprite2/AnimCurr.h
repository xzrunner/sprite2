#ifndef _SPRITE2_ANIM_CURR_H_
#define _SPRITE2_ANIM_CURR_H_

#include "S2_Message.h"
#include "AnimCopy.h"
#include "VisitResult.h"

#include <CU_RefCountObj.h>

#include <vector>
#include <string>

namespace s2
{

class Actor;
class AnimCopy;
class Sprite;
class AnimSymbol;
class SpriteVisitor;
class SprVisitorParams;
class RenderParams;
class UpdateParams;

class AnimCurr : public cu::RefCountObj
{
public:
	AnimCurr();
	AnimCurr(const AnimCurr& curr);
	AnimCurr& operator = (const AnimCurr& curr);
	~AnimCurr();
	
	VisitResult Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const;

	void OnMessage(const UpdateParams& up, const Sprite* spr, Message msg);

	bool Update(const UpdateParams& up, const Sprite* spr,
		bool loop = true, float interval = 0, int fps = 30);
	void Draw(const RenderParams& rp) const;

	Sprite* FetchChild(const std::string& name) const;
	Sprite* FetchChild(int idx) const;

	void Start(const UpdateParams& up, const Sprite* spr);

	void SetTime(float time);
	void SetFrame(const UpdateParams& up, const Sprite* spr, int frame, int fps);

	int GetFrame() const { return m_frame; }

	int GetSlotSize() const { return m_slots.size(); }

	void SetAnimCopy(const AnimCopy* copy);

	void SetActive(bool active);
	bool IsActive() const { return m_active; }

	void Clear();

private:
	void ResetTime();

	void ResetLayerCursor();

	void LoadCurrSprites(const UpdateParams& up, const Sprite* spr);
	void UpdateCursor(bool cursor_update);
	void LoadCurrSprites(const UpdateParams& up, const Sprite* spr, bool cursor_update);
	bool UpdateChildren(const UpdateParams& up, const Sprite* spr);

	static void LoadSprLerpData(Sprite* spr, const AnimCopy::Lerp& lerp, int time);
	
private:
	const AnimCopy* m_copy;

	std::vector<int> m_layer_cursor;

	std::vector<Sprite*> m_slots;

	int* m_curr;
	int  m_curr_num;

	int m_frame;

	float m_start_time, m_curr_time;
	float m_stop_time, m_stop_during;

	bool m_active;

}; // AnimCurr

}

#endif // _SPRITE2_ANIM_CURR_H_