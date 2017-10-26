#ifndef _SPRITE2_ANIM_CURR_H_
#define _SPRITE2_ANIM_CURR_H_

#include "sprite2/Message.h"
#include "sprite2/AnimCopy.h"
#include "sprite2/VisitResult.h"
#include "sprite2/AnimCtrl.h"
#include "sprite2/RenderReturn.h"

#include <cu/cu_stl.h>

#include <memory>

namespace cooking { class DisplayList; }

namespace s2
{

class Actor;
class AnimCopy;
class Symbol;
class Sprite;
class AnimSymbol;
class SpriteVisitor;
class SprVisitorParams;
class SpriteVisitor2;
class SprVisitorParams2;
class RenderParams;
class UpdateParams;

class AnimCurr
{
public:
	AnimCurr();
	AnimCurr(const AnimCurr& curr);
	AnimCurr& operator = (const AnimCurr& curr);

	void AssignSameStruct(const AnimCurr& src);

	bool Update(const UpdateParams& up, const Symbol* sym, const Sprite* spr,
		bool loop = true, float interval = 0, int fps = 30);
	bool SetFrame(const UpdateParams& up, const Sprite* spr, int frame, int fps);

	void Start(const UpdateParams& up, const Sprite* spr);

	void OnMessage(const UpdateParams& up, const Sprite* spr, Message msg);

	SprPtr FetchChildByName(int name, const ActorConstPtr& actor) const;
	SprPtr FetchChildByIdx(int idx) const;

	VisitResult Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const;
	VisitResult Traverse2(SpriteVisitor2& visitor, const SprVisitorParams2& params) const;

	int GetSlotSize() const { return m_slots.size(); }

	RenderReturn Draw(cooking::DisplayList* dlist, const RenderParams& rp) const;

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

	void LoadCurrSprites(const UpdateParams& up, const Sprite* spr);
	void UpdateCursor();
	void LoadCurrSpritesImpl(const UpdateParams& up, const Sprite* spr);
	bool UpdateChildren(const UpdateParams& up, const Sprite* spr);

	void SetChildrenFrame(const UpdateParams& up, const Sprite* spr, int frame, int fps);

	void UpdateSlotsVisible();
		
private:
	std::shared_ptr<const AnimCopy> m_copy;

	AnimCtrl m_ctrl;

	CU_VEC<int> m_layer_cursor;
	CU_VEC<int> m_layer_cursor_update;

	CU_VEC<SprPtr> m_slots;

	CU_VEC<int> m_curr;
	int         m_curr_num;

	//int* m_curr;
	//int  m_curr_num;

}; // AnimCurr

using AnimCurrPtr = std::unique_ptr<AnimCurr, mm::alloc_deleter<mm::Allocator<AnimCurr>>>;

}

#endif // _SPRITE2_ANIM_CURR_H_