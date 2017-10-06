#ifndef _SPRITE2_ANIM_CURR_H_
#define _SPRITE2_ANIM_CURR_H_

#include "S2_Message.h"
#include "AnimCopy.h"
#include "VisitResult.h"
#include "AnimCtrl.h"
#include "RenderReturn.h"

#include <vector>
#include <string>
#include <memory>

namespace s2
{

class Actor;
class AnimCopy;
class Symbol;
class Sprite;
class AnimSymbol;
class SpriteVisitor;
class SprVisitorParams;
class RenderParams;
class UpdateParams;

class AnimCurr
{
public:
	AnimCurr();
	AnimCurr(const AnimCurr& curr);
	AnimCurr& operator = (const AnimCurr& curr);
	~AnimCurr();

	std::unique_ptr<AnimCurr> Clone() const;

	void AssignSameStruct(const AnimCurr& src);

	bool Update(const UpdateParams& up, const SymConstPtr& sym, const SprConstPtr& spr,
		bool loop = true, float interval = 0, int fps = 30);
	bool SetFrame(const UpdateParams& up, const SprConstPtr& spr, int frame, int fps);

	void Start(const UpdateParams& up, const SprConstPtr& spr);

	void OnMessage(const UpdateParams& up, const SprConstPtr& spr, Message msg);

	SprPtr FetchChildByName(int name, const ActorConstPtr& actor) const;
	SprPtr FetchChildByIdx(int idx) const;

	VisitResult Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const;

	int GetSlotSize() const { return m_slots.size(); }

	RenderReturn Draw(const RenderParams& rp) const;

	void Clear();

	sm::rect CalcAABB(const Actor* actor) const;

	void SetAnimCopy(const std::shared_ptr<AnimCopy>& copy);

	int GetFrame() const { return m_ctrl.GetFrame(); }

	void SetTime(float time) { return m_ctrl.SetTime(time); }

	void SetActive(bool active) { m_ctrl.SetActive(active); }
	bool IsActive() const { return m_ctrl.IsActive(); }

	static void LoadSprLerpData(Sprite& spr, const AnimCopy::Lerp& lerp, int time);

private:
	int  UpdateFrameCursor(bool loop, float interval, int fps, bool reset_cursor);

	void ResetLayerCursor();

	void LoadCurrSprites(const UpdateParams& up, const SprConstPtr& spr);
	void UpdateCursor();
	void LoadCurrSpritesImpl(const UpdateParams& up, const SprConstPtr& spr);
	bool UpdateChildren(const UpdateParams& up, const SprConstPtr& spr);

	void SetChildrenFrame(const UpdateParams& up, const SprConstPtr& spr, int frame, int fps);

	void UpdateSlotsVisible();
		
private:
	std::shared_ptr<const AnimCopy> m_copy;

	AnimCtrl m_ctrl;

	std::vector<int> m_layer_cursor;
	std::vector<int> m_layer_cursor_update;

	std::vector<SprPtr> m_slots;

	int* m_curr;
	int  m_curr_num;

}; // AnimCurr

}

#endif // _SPRITE2_ANIM_CURR_H_