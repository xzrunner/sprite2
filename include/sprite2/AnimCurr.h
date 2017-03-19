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

class AnimCopy;
class Sprite;
class AnimSymbol;
class SpriteVisitor;
class SprVisitorParams;
class RenderParams;
class SprTreePath;

class AnimCurr : public cu::RefCountObj
{
public:
	AnimCurr();
	AnimCurr(const AnimCurr& curr);
	AnimCurr& operator = (const AnimCurr& curr);
	~AnimCurr();
	
	VisitResult Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const;

	void OnMessage(Message msg, const SprTreePath& path);

	bool Update(const RenderParams& rp, bool loop = true, 
		float interval = 0, int fps = 30);
	void Draw(const RenderParams& rp) const;

	Sprite* FetchChild(const std::string& name, const SprTreePath& path) const;

	void Start(const SprTreePath& path);

	void SetTime(float time);
	void SetFrame(int frame, int fps, const SprTreePath& path);

	int GetFrame() const { return m_frame; }

	void SetAnimCopy(const AnimCopy* copy);

	void SetActive(bool active);
	bool IsActive() const { return m_active; }

	void Clear();

private:
	void ResetTime();

	void ResetLayerCursor();

	void LoadCurrSprites(const SprTreePath& path);
	void UpdateCursor(bool cursor_update);
	void LoadCurrSprites(bool cursor_update, const SprTreePath& path);

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